/* 
 * Blink a LED using Timer interrupts.
 * (c) 2018-2024 Tomas Fryza, MIT license
 *
 * Developed using PlatformIO and AVR 8-bit Toolchain 3.6.2.
 * Tested on Arduino Uno board and ATmega328P, 16 MHz.
 */

// -- Includes -------------------------------------------------------
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC


// -- Defines --------------------------------------------------------
#define LED_BUILTIN PB5  // On-board LED
#define LED_EXT PB0      // External LED


// -- Function definitions -------------------------------------------
/*
 * Function: Main function where the program execution begins
 * Purpose:  Toggle LEDs using 8- and 16-bit Timers.
 * Returns:  none
 */
int main(void)
{
    // Set output pins
    gpio_mode_output(&DDRB, LED_BUILTIN);
    gpio_mode_output(&DDRB, LED_EXT);

    // Set Timer1 overflow to 262 ms and enable interrupt
    tim1_ovf_262ms();
    tim1_ovf_enable();

    // Set Timer0 overflow to 16 ms and enable interrupt
    tim0_ovf_16ms();
    tim0_ovf_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines, ISRs */
    }

    // Will never reach this
    return 0;
}


// -- Interrupt service routines -------------------------------------
/*
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Toggle on-board LED.
 */
ISR(TIMER1_OVF_vect)
{
    gpio_toggle(&PORTB, LED_BUILTIN);
}


/*
 * Function: Timer/Counter0 overflow interrupt
 * Purpose:  Toggle external LED.
 */
ISR(TIMER0_OVF_vect)
{
    static uint8_t n_ovfs = 0;

    n_ovfs++;
    if (n_ovfs >= 6)
    {
        // Do this every 6 x 16 ms = 100 ms
        n_ovfs = 0;
        gpio_toggle(&PORTB, LED_EXT);
    }
    // Else do nothing and exit the ISR
}


/*
    static uint8_t n_ovfs = 0;

    TCNT0 = 128;

    n_ovfs++;
    if (n_ovfs >= 30)
    {
        n_ovfs = 0;
    }
    // t_ovf = 1/16e6 * 1024 * 256 = 16 ms
    // t_ovf = 1/16e6 * 1024 * (256-128) = 8 ms
*/
