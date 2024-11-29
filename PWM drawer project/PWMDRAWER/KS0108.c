

#include "KS0108.h"
#include "Font5x8.h"
#include <util/delay.h>

// Initialize the GLCD
void GLCD_Initialize(void)
{
    // Set data and control pins as output
    GLCD_DDR_DATA = 0xFF;  // Data pins D0-D7 as output
    GLCD_DDR_CTRL |= (1 << GLCD_DI) | (1 << GLCD_RW) | (1 << GLCD_EN) | (1 << GLCD_CS1) | (1 << GLCD_CS2) | (1 << GLCD_RST);

    // Reset the GLCD
    GLCD_CTRL_PORT &= ~(1 << GLCD_RST);  // Pull RESET low
    _delay_ms(10);                       // Wait 10 ms
    GLCD_CTRL_PORT |= (1 << GLCD_RST);   // Release RESET

    // Turn on both halves of the GLCD
    GLCD_WriteCommand(0x3F, 0);  // Turn on the left chip
    GLCD_WriteCommand(0x3F, 1);  // Turn on the right chip
}

// Write a command to the GLCD
void GLCD_WriteCommand(uint8_t command, uint8_t chip)
{
    // Select the chip
    if (chip == 0)
    {
        GLCD_CTRL_PORT &= ~(1 << GLCD_CS1); // Enable left chip
        GLCD_CTRL_PORT |= (1 << GLCD_CS2);  // Disable right chip
    }
    else if (chip == 1)
    {
        GLCD_CTRL_PORT &= ~(1 << GLCD_CS2); // Enable right chip
        GLCD_CTRL_PORT |= (1 << GLCD_CS1);  // Disable left chip
    }

    // Set control lines for command mode
    GLCD_CTRL_PORT &= ~(1 << GLCD_DI); // DI = 0 (Command mode)
    GLCD_CTRL_PORT &= ~(1 << GLCD_RW); // RW = 0 (Write mode)

    // Write the command to the data port
    GLCD_DATA_PORT = command;

    // Generate enable pulse
    GLCD_CTRL_PORT |= (1 << GLCD_EN);  // EN = 1
    _delay_us(1);                      // Small delay
    GLCD_CTRL_PORT &= ~(1 << GLCD_EN); // EN = 0

    // Deselect chips
    GLCD_CTRL_PORT |= (1 << GLCD_CS1) | (1 << GLCD_CS2);
}

// Write data to the GLCD
void GLCD_WriteData(uint8_t data)
{
    // Enable both chips (depending on where you are writing)
    GLCD_CTRL_PORT &= ~(1 << GLCD_CS1); // Enable left chip
    GLCD_CTRL_PORT |= (1 << GLCD_CS2);  // Disable right chip (if single-side operation)

    // Set control lines for data mode
    GLCD_CTRL_PORT |= (1 << GLCD_DI);  // DI = 1 (Data mode)
    GLCD_CTRL_PORT &= ~(1 << GLCD_RW); // RW = 0 (Write mode)

    // Write the data to the data port
    GLCD_DATA_PORT = data;

    // Generate enable pulse
    GLCD_CTRL_PORT |= (1 << GLCD_EN);  // EN = 1
    _delay_us(1);                      // Small delay
    GLCD_CTRL_PORT &= ~(1 << GLCD_EN); // EN = 0

    // Deselect chips
    GLCD_CTRL_PORT |= (1 << GLCD_CS1) | (1 << GLCD_CS2);
}

// Set the page (row) on the GLCD
void GLCD_SetPage(uint8_t page, uint8_t chip)
{
    GLCD_WriteCommand(0xB8 | page, chip); // Set page address command
}

// Set the column (position) on the GLCD
void GLCD_SetColumn(uint8_t column, uint8_t chip)
{
    GLCD_WriteCommand(0x40 | column, chip); // Set column address command
}

void GLCD_GoTo(uint8_t x, uint8_t y)
{
    uint8_t chip = 0;

    // Determine which chip to use based on the X coordinate
    if (x >= 64)
    {
        chip = 1;  // Use the right chip
        x -= 64;  // Adjust X coordinate for the right chip
    }
    else
    {
        chip = 0;  // Use the left chip
    }

    // Set the page (Y) and column (X) for the selected chip
    GLCD_SetPage(y, chip);
    GLCD_SetColumn(x, chip);
}

void GLCD_WriteChar(char character)
{
    if (character < 32 || character > 126)
        return; // Ignore characters outside the font table

    // Get the font data for the character
    uint8_t index = character - 32; // Adjust for ASCII offset
    for (uint8_t i = 0; i < 5; i++)
    {
        uint8_t column = pgm_read_byte(&font5x8[index][i]); // Read font data from program memory
        GLCD_WriteData(column); // Write column data to GLCD
    }

    // Add a 1-column space after each character
    GLCD_WriteData(0x00);
}


void GLCD_WriteString(const char *str)
{
    while (*str)
    {
        GLCD_WriteChar(*str); // Write each character
        str++; // Move to the next character
    }
}

void GLCD_Clear(void)
{
    for (uint8_t page = 0; page < 8; page++) // Iterate through all pages (rows)
    {
        GLCD_SetPage(page, 0); // Set the page on the left chip
        GLCD_SetColumn(0, 0); // Set column to 0 on the left chip

        for (uint8_t column = 0; column < 64; column++) // Clear all 64 columns on left chip
        {
            GLCD_WriteData(0x00);
        }

        GLCD_SetPage(page, 1); // Set the page on the right chip
        GLCD_SetColumn(0, 1); // Set column to 0 on the right chip

        for (uint8_t column = 0; column < 64; column++) // Clear all 64 columns on right chip
        {
            GLCD_WriteData(0x00);
        }
    }
}

// Clear the GLCD screen
void GLCD_ClearScreen(void)
{
    for (uint8_t page = 0; page < 8; page++) // Loop through all 8 pages
    {
        GLCD_SetPage(page, 0); // Set page on left chip
        GLCD_SetColumn(0, 0); // Start at column 0 on left chip
        for (uint8_t i = 0; i < 64; i++) // Clear 64 columns on the left chip
        {
            GLCD_WriteData(0x00);
        }

        GLCD_SetPage(page, 1); // Set page on right chip
        GLCD_SetColumn(0, 1); // Start at column 0 on right chip
        for (uint8_t i = 0; i < 64; i++) // Clear 64 columns on the right chip
        {
            GLCD_WriteData(0x00);
        }
    }
}

