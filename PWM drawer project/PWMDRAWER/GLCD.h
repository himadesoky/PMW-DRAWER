/*
 * GLCD.h
 *
 *  Created on: Nov 23, 2024
 *      Author: Ibrahim Mahmoud Ibrahim
 */

#ifndef GLCD_H
#define GLCD_H

#include <avr/io.h>
#include <util/delay.h>
#include "KS0108.h"  // External library for GLCD (add this to your project)

// Initialize GLCD
void GLCD_Init(void)
{
    GLCD_Initialize();  // Initialize KS0108 GLCD
    GLCD_ClearScreen(); // Clear GLCD screen
}

// Clear the GLCD screen
/*void GLCD_Clear(void)
{
    GLCD_ClearScreen();
}
*/
// Draw a bar representing the PWM duty cycle
void GLCD_DrawBar(uint8_t duty_cycle)
{
    char buffer[16];
    uint8_t bar_length = (duty_cycle * 128) / 100; // Map duty cycle to screen width (128px)

    // Draw the bar
    for (uint8_t i = 0; i < bar_length; i++)
    {
        GLCD_WriteData(0xFF); // Fill the bar
    }

    // Display text
    sprintf(buffer, "Duty: %d%%", duty_cycle);
    GLCD_GoTo(0, 1);
    GLCD_WriteString(buffer);
}

#endif

