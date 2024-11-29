

#ifndef KS0108_H
#define KS0108_H

#include <avr/io.h>

// Define GLCD ports and pins
#define GLCD_DATA_PORT   PORTA
#define GLCD_DDR_DATA    DDRA
#define GLCD_CTRL_PORT   PORTB
#define GLCD_DDR_CTRL    DDRB
#define GLCD_DI          PB0
#define GLCD_RW          PB1
#define GLCD_EN          PB2
#define GLCD_CS1         PB3
#define GLCD_CS2         PB4
#define GLCD_RST         PB5

// Function prototypes
void GLCD_Initialize(void);
void GLCD_ClearScreen(void);
void GLCD_WriteCommand(uint8_t command, uint8_t chip);
void GLCD_WriteData(uint8_t data);
void GLCD_SetPage(uint8_t page, uint8_t chip);
void GLCD_SetColumn(uint8_t column, uint8_t chip);
void GLCD_WriteChar(char character);
void GLCD_WriteString(const char *str);
void GLCD_GoTo(uint8_t x, uint8_t y);
void GLCD_Clear(void);
#endif

