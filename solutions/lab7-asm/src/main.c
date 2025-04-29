/*
 * Implementation of LFSR-based (Linear Feedback Shift Register) 
 * pseudo-random generator in AVR assembly.
 * (c) 2017-2024 Tomas Fryza, MIT license
 *
 * Developed using PlatformIO and AVR 8-bit Toolchain 3.6.2.
 * Tested on Arduino Uno board and ATmega328P, 16 MHz.
 * 
 * NOTE:
 *   To see assembly listing, run the following command in Terminal
 *   after the compilation.
 * 
 *   Windows:
 *   C:\Users\YOUR-LOGIN\.platformio\packages\toolchain-atmelavr\bin\avr-objdump -S -d -m avr .pio/build/uno/firmware.elf > firmware.lst
 * 
 *   Linux, Mac:
 *   ~/.platformio/packages/toolchain-atmelavr/bin/avr-objdump -S -d -m avr .pio/build/uno/firmware.elf > firmware.lst
 * 
 * SEE ALSO:
 *   https://five-embeddev.com/baremetal/platformio/
 */

// Settings for main page of Doxygen manual
/**
 * @mainpage
 * 
 * Collection of AVR libraries used in bachelor course Digital 
 * Electronics 2 at Brno University of Technology, Czechia.
 * 
 * @author Tomas Fryza, Peter Fleury
 * @copyright (c) 2017-2024 Tomas Fryza, MIT license
 */

// -- Includes -------------------------------------------------------
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <uart.h>           // Peter Fleury's UART library
#include <stdio.h>          // C library for `sprintf`
#include <lfsr.h>


// -- Defines --------------------------------------------------------
#define LFSR_SEED 7  // Starting value of the shift register


// -- Function definitions -------------------------------------------
void timer1_init(void)
{
    tim1_ovf_33ms();
    tim1_ovf_enable();
}


/*
 * Function: Main function where the program execution begins
 * Purpose:  Generate a new pseudo-random value using 4- and/or 8-bit
 *           LFSR structure and send it to UART.
 * Returns:  none
 */
int main(void)
{
    // NOTE: Add `monitor_speed = 115200` to `platformio.ini`
    uart_init(UART_BAUD_SELECT(115200, F_CPU));
    timer1_init();

    sei();

    // Infinite empty loop
    while (1)
    {
    }

    // Will never reach this
    return 0;
}


// -- Interrupt service routines -------------------------------------
/*
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Generate one pseudo-random value using 4- and/or 8-bit
 *           LFSR structure.
 *
 * Note: (4,3): 0, 1, 3, 7, 14, 13, 11, 6, 12, 9, 2, 5, 10, 4, 8
 *       (4,2): 0, 1, 3, 6, 12, 8
 *       (4,1): 0, 1, 2, 5, 10, 4, 9, 3, 6, 13, 11, 7, 14, 12, 8
 */
ISR(TIMER1_OVF_vect)
{
    static uint8_t n_vals = 0;
    static uint8_t value = LFSR_SEED;
    char uart_msg[30];

    // Multiply-and-accumulate in Assembly
    //                                 c + (a*b)
    // value = multiply_accumulate_asm(value, 3, 7);

    // Send value to UART
    sprintf(uart_msg, "%d, ", value);
    uart_puts(uart_msg);

    // WRITE YOUR CODE HERE
    // value = lfsr4_fibonacci_asm(value);
    value = lfsr4_fibonacci_asm(value);
    n_vals++;
    if (value == 0)
    {
        // NOTE: Add `monitor_raw = 1` to `platformio.ini`
        sprintf(uart_msg, "\r\n\x1b[32;1mLength = %d\x1b[0m\r\n", n_vals);
        uart_puts(uart_msg);
        n_vals = 0;
    }
}
