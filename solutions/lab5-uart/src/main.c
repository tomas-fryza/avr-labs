/*
 * Use USART unit and transmit data between ATmega328P and computer.
 * (c) 2018-2024 Tomas Fryza, MIT license
 *
 * Developed using PlatformIO and AVR 8-bit Toolchain 3.6.2.
 * Tested on Arduino Uno board and ATmega328P, 16 MHz.
 */

// -- Includes -------------------------------------------------------
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <uart.h>           // Peter Fleury's UART library
#include <stdlib.h>         // C library. Needed for number conversions


// -- Function definitions -------------------------------------------
/*
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and transmit UART data.
 * Returns:  none
 */
int main(void)
{
    uint16_t value;
    char string[8];  // String for converted numbers by itoa()

    // Initialize USART to asynchronous, 8-N-1, 9600 Bd
    uart_init(UART_BAUD_SELECT(9600, F_CPU));

    tim1_ovf_1sec();
    // tim1_ovf_enable();
    tim0_ovf_16ms();

    // Interrupts must be enabled, bacause of `uart_puts()`
    sei();

    // Put strings to ringbuffer for transmitting via UART
    uart_puts("\r\n");  // New line only
    uart_puts("Click to Serial monitor and press a key on keyboard...\r\n");
    uart_puts("      0: Timer0 value\r\n");
    uart_puts("      1: Timer1 value\r\n");
    uart_puts("  other: ASCII code\r\n");

    // Infinite loop
    while (1)
    {
        // Get received data from UART
        value = uart_getc();
        if ((value & 0xff00) == 0)  // If successfully received data from UART
        {
            if (value == '1')
            {
                value = TCNT1;
                itoa(value, string, 16);
                uart_puts("Timer1: 0x");
                uart_puts(string);
                uart_puts("\r\n");
            }
            else if (value == '0')
            {
                value = TCNT0;
                itoa(value, string, 16);
                uart_puts("Timer0: 0x");
                uart_puts(string);
                uart_puts("\r\n");
            }
            else
            {
                // Transmit the received character back via UART
                uart_putc(value);

                // Transmit the ASCII code also in hex, dec, and bin
                itoa(value, string, 16);
                uart_puts("\x1b[1;32m");
                uart_puts("\t0x");
                uart_puts(string);
                uart_puts("\x1b[0m");

                itoa(value, string, 10);
                uart_puts("\t");
                uart_puts(string);

                itoa(value, string, 2);
                uart_puts("\t0b");
                uart_puts(string);

                // New line
                uart_puts("\r\n");
            }
        }
    }

    // Will never reach this
    return 0;
}


// -- Interrupt service routines -------------------------------------
/*
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Transmit UART data.
 */
ISR(TIMER1_OVF_vect)
{
    // WRITE YOUR CODE HERE
    uart_puts("Paris\t");
}



/*
        // Get the Even parity
        // uint8_t even = 0;
        // Even = b0 xor b1 xor .... xor b7
        // value = b7 b6 b5 b4 b3 b2 b1 b0
        // Cycle 8 times:
        //   -- extract just one bit: LSB
        //   -- apply binary mask and logic AND
        //   -- even = even xor b0
        //   -- shift even to the right
        for (uint8_t i = 0; i < 8; i++) {
            even = even ^ (value & 0x01);
            value = value >> 1;
        }
        uart_puts("\tEven: ");
        itoa(even, string, 10);
        uart_puts(string);
*/
