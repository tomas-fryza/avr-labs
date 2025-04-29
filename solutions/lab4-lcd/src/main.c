/* 
 * Stopwatch on Liquid Crystal Display (LCD).
 * (c) 2017-2024 Tomas Fryza, MIT license
 *
 * Developed using PlatformIO and AVR 8-bit Toolchain 3.6.2.
 * Tested on Arduino Uno board and ATmega328P, 16 MHz.
 */

// -- Includes -------------------------------------------------------
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <lcd.h>            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for number conversions


// -- Global variables -----------------------------------------------
volatile uint8_t flag_update_lcd = 0;

// Stopwatch values
// Declaration of "stopwatch" variable with structure "Stopwatch_structure"
struct Stopwatch_structure
{
    uint8_t tenths;  // Tenths of a second
    uint8_t secs;    // Seconds
    uint8_t mins;    // Minutes
} stopwatch;


// -- Function definitions -------------------------------------------
void lcd_setup(void)
{
    // Initialize display
    lcd_init(LCD_DISP_ON);

    // Put string(s) on LCD screen
    lcd_gotoxy(6, 0);  // Move to 7th column and 1st line
    lcd_puts("LCD Test");
    lcd_putc('!');

    lcd_gotoxy(1, 0);
    lcd_puts("00:00.0");

    // Set backlight at PB2
    // gpio_mode_output(&DDRB, PB2);
    // gpio_write_low(&PORTB, PB2);

    // Custom character(s)
    // https://www.quinapalus.com/hd44780udg.html
    uint8_t new_char1[8] = {0x2,0x3,0x2,0x2,0xe,0x1e,0xc,0x0};
    uint8_t new_char2[8] = {0x4,0xe,0xe,0xe,0x1f,0x0,0x4,0x0};

    lcd_custom_char(7, new_char1);  // Location 0..7
    lcd_custom_char(3, new_char2);
    lcd_gotoxy(13, 1);
    lcd_putc(7);
    lcd_putc(3);
}

void timer2_init(void)
{
    // Configuration of 8-bit Timer/Counter2 for Stopwatch update
    tim2_ovf_16ms();
    tim2_ovf_enable();
}


/*
 * Function: Main function where the program execution begins
 * Purpose:  Update stopwatch value on LCD screen when 8-bit Timer/Counter2
 *           overflows.
 * Returns:  none
 */
int main(void)
{
    char string[2];  // String for converted numbers by itoa()

    lcd_setup();
    timer2_init();

    // Enable global interrupts
    sei();

    // Infinite loop
    while (1)
    {
        if (flag_update_lcd == 1)
        {
            // Display "00:00.tenths"
            itoa(stopwatch.tenths, string, 10);  // Convert decimal value to string
            lcd_gotoxy(7, 0);
            lcd_puts(string);

            itoa(stopwatch.secs, string, 10);
            lcd_gotoxy(4, 0);
            if (stopwatch.secs < 10)
                lcd_puts("0");
            lcd_puts(string);

            flag_update_lcd = 0;
        }
    }

    // Will never reach this
    return 0;
}


// -- Interrupt service routines -------------------------------------
/*
 * Function: Timer/Counter2 overflow interrupt
 * Purpose:  Update the stopwatch on LCD screen every sixth overflow,
 *           ie approximately every 100 ms (6 x 16 ms = 100 ms).
 */
ISR(TIMER2_OVF_vect)
{
    static uint8_t n_ovfs = 0;

    n_ovfs++;

    // Do this every 6 x 16 ms = 100 ms
    if (n_ovfs >= 6)
    {
        n_ovfs = 0;
        stopwatch.tenths++;
        // Count tenth of seconds 0, 1, ..., 9, 0, 1, ...
        if (stopwatch.tenths > 9)
        {
            stopwatch.tenths = 0;
            // Seconds
            stopwatch.secs++;
            if (stopwatch.secs > 59)
            {
                stopwatch.secs = 0;
            }
        }
        flag_update_lcd = 1;
    }
    // Else do nothing and exit the ISR
}
