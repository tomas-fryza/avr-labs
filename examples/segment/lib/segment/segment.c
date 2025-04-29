/*
 * Seven-segment display library for AVR-GCC.
 * (c) 2018-2024 Tomas Fryza, MIT license
 *
 * Developed using PlatformIO v3.3.3 and Atmel AVR platform 5.0.0
 * Tested on Arduino Uno board and ATmega328P, 16 MHz.
 */

// -- Defines --------------------------------------------------------
#ifndef F_CPU
# define F_CPU 16000000
#endif


// -- Includes -------------------------------------------------------
#include <avr/io.h>         // AVR device-specific IO definitions
#include <util/delay.h>
#include <stdlib.h>         // C library. Needed for number conversions
#include <segment.h>


// -- Global variables -----------------------------------------------
// Segment values (abcdefgDP, active low) for 10 digits
volatile uint8_t map_digit[10] = {
// 0     1     2     3     4     5     6     7     8     9
0x03, 0x9f, 0x25, 0x0d, 0x99, 0x49, 0x41, 0x1f, 0x01, 0x09};

// Digit position 3:0 (p3 p2 p1 p0 xxxx, active high)
volatile uint8_t map_position[4] = {0x10, 0x20, 0x40, 0x80};


// -- Function definitions -------------------------------------------
/*
 * Function: seg_init()
 * Purpose:  Configure SSD signals LATCH, CLK, and DATA as output.
 * Returns:  none
 */
void seg_init(void)
{
    // Configure data pins as output
    DDR(SEG_LATCH_PORT) |= _BV(SEG_LATCH_PIN);
    DDR(SEG_CLK_PORT) |= _BV(SEG_CLK_PIN);
    DDR(SEG_DATA_PORT) |= _BV(SEG_DATA_PIN);

    seg_clrscr();
}


/*
 * Function: seg_update_shift_regs()
 * Purpose:  Display segments at one position of the SSD.
 * Input:    segments - Segments to be displayed (abcdefgDP, active low)
 *           position - Position of the display where the segments are to 
 *                      be displayed (p3 p2 p1 p0 xxxx, active high)
 * Returns:  none
 */
void seg_show_digit(uint8_t value, uint8_t position)
{
    // Map values 0..9
    value = map_digit[value];
    // Map positions 0..3
    position = map_position[position];

    // Pull LATCH, CLK, and DATA low
    SEG_LATCH_PORT &= ~_BV(SEG_LATCH_PIN);
    SEG_CLK_PORT &= ~_BV(SEG_CLK_PIN);
    SEG_DATA_PORT &= ~_BV(SEG_DATA_PIN);

    // Wait 1 us
    _delay_us(1);

    shift_out(value);
    shift_out(position);

    // Pull LATCH high
    SEG_LATCH_PORT |= _BV(SEG_LATCH_PIN);
}


/*
 * Function: shift_out()
 * Purpose:  Shift 8-bit data into a shift register.
 */
void shift_out(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        // Test the LSB
        if ((data & 0x01) == 1)
            SEG_DATA_PORT |= _BV(SEG_DATA_PIN);
        else
            SEG_DATA_PORT &= ~_BV(SEG_DATA_PIN);

        clk_pulse_2us();

        data = data >> 1;
    }
}


/*
 * Function: seg_pulse_2us()
 * Purpose:  Generate one clock pulse at SEG_CLK_PIN with length of 2 microsecs.
 */
void clk_pulse_2us()
{
    _delay_us(1);
    SEG_CLK_PORT |= _BV(SEG_CLK_PIN);
    _delay_us(1);
    SEG_CLK_PORT &= ~_BV(SEG_CLK_PIN);
}


/*
 * Function: seg_clrscr()
 * Purpose:  Turn off all segments.
 */
void seg_clrscr(void)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        // Blind value (abcdefgDP, active low)
        shift_out(0xff);
        shift_out(map_position[i]);
    }
}
