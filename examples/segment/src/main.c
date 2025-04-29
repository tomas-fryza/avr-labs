/*
 * Decimal counter using 7-segment displays.
 * (c) 2018-2024 Tomas Fryza, MIT license
 *
 * Developed using PlatformIO v3.3.3 and Atmel AVR platform 5.0.0
 * Tested on Arduino Uno board and ATmega328P, 16 MHz.
 */

// -- Includes -------------------------------------------------------
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <stdlib.h>         // C library. Needed for number conversions
#include <segment.h>


// -- Global variables -----------------------------------------------
volatile uint16_t cnt;


// -- Function definitions -------------------------------------------
void timer1_init()
{
    tim2_ovf_16ms();
    tim2_ovf_enable();
}

void timer0_init()
{
    tim0_ovf_4ms();
    tim0_ovf_enable();
}


/*
 * Function: Main function where the program execution begins
 * Purpose:  Use 7-segment displays for decimal counter.
 * Returns:  none
 */
int main(void)
{
    // Initialize SSD signals
    seg_init();

    // Decimal counter main clock
    timer1_init();
    // SSD multiplexing
    timer0_init();

    // Init counter value
    cnt = 950;

    sei();

    // Infinite loop
    while (1);

    // Will never reach this
    return 0;
}


// -- Interrupt service routines -------------------------------------
/*
 * Function: Timer/Counter2 overflow interrupt
 * Purpose:  Increment counter value every 100 ms.
 */
ISR(TIMER2_OVF_vect)
{
    static uint8_t n_ovfs = 0;

    n_ovfs++;

    // Do this every 6 x 16 ms = 100 ms
    if (n_ovfs >= 6)
    {
        n_ovfs = 0;
        cnt++;
        if (cnt > 9999)
            cnt = 0;
    }
    // Else do nothing and exit the ISR
}


/*
 * Function: Timer/Counter0 overflow interrupt
 * Purpose:  Multiplex display positions.
 */
ISR(TIMER0_OVF_vect)
{
    static uint8_t pos = 0;
    uint8_t digits[4];  // Array to hold up to 4 digits

    // Select one digit position
    pos++;
    if (pos > 3)
        pos = 0;

    // Extract up to 4 digits from the counter
    switch (pos)
    {
        case 0:
            // Extract units digit (1's place)
            digits[0] = cnt % 10;
            break;

        case 1:
            // Extract tens digit (10's place)
            digits[1] = (cnt / 10) % 10;
            break;

        case 2:
            // Extract hundreds digit (100's place)
            digits[2] = (cnt / 100) % 10;
            break;

        default:
            // Extract thousands digit (1000's place)
            digits[3] = (cnt / 1000) % 10;
    }

    seg_show_digit(digits[pos], pos);
}
