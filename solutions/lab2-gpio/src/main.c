/*
 * Blink a LED using GPIO library and delay.
 * (c) 2018-2024 Tomas Fryza, MIT license
 *
 * Developed using PlatformIO and AVR 8-bit Toolchain 3.6.2.
 * Tested on Arduino Uno board and ATmega328P, 16 MHz.
 */

// -- Defines --------------------------------------------------------
#define LED_BUILTIN PB5  // On-board LED
#define LED_EXT PB0      // External LED
#define BTN PD2
#define SHORT_DELAY 250  // Delay in milliseconds


// -- Includes -------------------------------------------------------
#include <avr/io.h>      // AVR device-specific IO definitions
#include <util/delay.h>  // Functions for busy-wait delay loops
#include <gpio.h>        // GPIO library for AVR-GCC


// -- Function definitions -------------------------------------------
/*
 * Function: Main function where the program execution begins
 * Purpose:  Toggle LED(s) using delay.
 * Returns:  none
 */
int main(void)
{
    // Set output pin(s) in Data Direction Register
    gpio_mode_output(&DDRB, LED_BUILTIN);
    gpio_mode_output(&DDRB, LED_EXT);

    // Set pin(s) LOW in Data Register
    gpio_write_low(&PORTB, LED_BUILTIN);
    gpio_write_low(&PORTB, LED_EXT);

    // Infinite loop
    while (1)
    {
        if (gpio_read(&PIND, BTN) == 0)
        {
            // Pause several milliseconds
            _delay_ms(SHORT_DELAY);
            // Invert LED in Data Register
            gpio_toggle(&PORTB, LED_BUILTIN);
            gpio_toggle(&PORTB, LED_EXT);
        }
    }

    // Will never reach this
    return 0;
}
