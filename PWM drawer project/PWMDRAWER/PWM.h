/*
 * PWM.h
 *
 *  Created on: Nov 23, 2024
 *      Author: Ibrahim Mahmoud Ibrahim
 */

#ifndef PWM_H
#define PWM_H

#include <avr/io.h>

// Initialize Timer1 for Fast PWM
void PWM_Init(void)
{
    DDRD |= (1 << PD5); // Set OC1A (PD5) as output
    TCCR1A = (1 << WGM11) | (1 << COM1A1); // Fast PWM, non-inverting mode
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler = 8
    ICR1 = 19999; // Set TOP value for 20ms period (50Hz)
}

// Set PWM duty cycle
void PWM_SetDutyCycle(uint8_t duty_cycle)
{
    if (duty_cycle > 100) duty_cycle = 100; // Limit duty cycle to 100%
    OCR1A = (duty_cycle * ICR1) / 100; // Set compare match value
}

#endif
