#ifndef SEGMENT_H
# define SEGMENT_H

/*
 * Seven-segment display library for AVR-GCC.
 * (c) 2019-2024 Tomas Fryza, MIT license
 *
 * Developed using PlatformIO v3.3.3 and Atmel AVR platform 5.0.0
 * Tested on Arduino Uno board and ATmega328P, 16 MHz.
 */

/**
 * @file 
 * @defgroup fryza_segment Seven-segment Library <segment.h>
 * @code #include <segment.h> @endcode
 *
 * @brief Seven-segment display library for AVR-GCC.
 *
 * The library contains functions for controlling the seven-segment 
 * display (SSD) using two shift registers 74HC595.
 *
 * @copyright (c) 2019-2024 Tomas Fryza, MIT license
 * @{
 */

// -- Includes -------------------------------------------------------
#include <avr/io.h>


// -- Defines --------------------------------------------------------
/**
 * @name  Definition of SSD interface
 * @note  Connection is based on Multi-function shield.
 */
#define SEG_LATCH_PORT PORTD
#define SEG_LATCH_PIN PD4

#define SEG_CLK_PORT PORTD
#define SEG_CLK_PIN PD7

#define SEG_DATA_PORT PORTB
#define SEG_DATA_PIN PB0

/**
 * @name Address of data direction register of port x
 */
#define DDR(x) (*(&x - 1))


// -- Function prototypes --------------------------------------------
/**
 * @brief  Configure SSD signals LATCH, CLK, and DATA as output.
 * @return none
 */
void seg_init(void);


/**
 * @brief  Display segments at one position of the SSD.
 * @param  segments Segments to be displayed (abcdefgDP, active low)
 * @param  position Position of the display where the segments are to 
 *                  be displayed (p3 p2 p1 p0 xxxx, active high)
 * @note   Two shift registers are connected in series, ie 16 bits are 
 *         transmitted.
 * @return none
 */
void seg_show_digit(uint8_t value, uint8_t position);


/**
 * @brief  Shift 8-bit data into a shift register.
 * @param  data Shift register dara
 */
void shift_out(uint8_t data);


/**
 * @brief  Generate one clock pulse at SEG_CLK_PIN with length of 2 microsecs.
 */
void clk_pulse_2us(void);


/**
 * @brief  Turn off all segments.
 */
void seg_clrscr(void);


/** @} */

#endif
