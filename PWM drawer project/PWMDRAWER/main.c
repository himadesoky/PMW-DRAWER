/*
 * main.c
 *
 *  Created on: Nov 23, 2024
 *      Author: Ibrahim Mahmoud
 */

#include <avr/io.h>
#include <util/delay.h>
#include "PWM.h"
#include "GLCD.h"

// Define buttons
#define BTN_INC PA0
#define BTN_DEC PA1

int main(void)
{
    uint8_t duty_cycle = 50; // Start with 50% duty cycle

    // Initialize PWM, GLCD, and buttons
    PWM_Init();
    GLCD_Init();
    DDRA &= ~((1 << BTN_INC) | (1 << BTN_DEC)); // Set buttons as input
    PORTA |= (1 << BTN_INC) | (1 << BTN_DEC);  // Enable pull-up resistors

    while (1)
    {
        // Check button states
        if (!(PINA & (1 << BTN_INC))) // Increase duty cycle
        {
            if (duty_cycle < 100)
            {
                duty_cycle += 10;
                _delay_ms(200); // Debounce delay
            }
        }
        if (!(PINA & (1 << BTN_DEC))) // Decrease duty cycle
        {
            if (duty_cycle > 0)
            {
                duty_cycle -= 10;
                _delay_ms(200); // Debounce delay
            }
        }

        // Update PWM duty cycle
        PWM_SetDutyCycle(duty_cycle);

        // Clear GLCD and display duty cycle
        GLCD_Clear();
        GLCD_DrawBar(duty_cycle); // Display a bar representing duty cycle
        _delay_ms(100);
    }
}

