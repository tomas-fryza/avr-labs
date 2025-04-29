#ifndef SEGMENT_H
# define SEGMENT_H

/***********************************************************************
 * 
 * Seven-segment display library for AVR-GCC.
 * 
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2019 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 *
 **********************************************************************/

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
 * @author Tomas Fryza, Dept. of Radio Electronics, Brno University 
 *         of Technology, Czechia
 * @copyright (c) 2019 Tomas Fryza, This work is licensed under 
 *                the terms of the MIT license
 * @{
 */


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>


/* Defines -----------------------------------------------------------*/
/**
 * @name  Definition of SSD interface
 * @note  Connection is based on Multi-function shield.
 */
#define SEG_LATCH   PD4
#define SEG_CLK     PD7
#define SEG_DATA    PB0


/* Function prototypes -----------------------------------------------*/
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
void seg_update_shift_regs(uint8_t segments, uint8_t position);


/* seg_clear */


/* seg_clk_2us */


/** @} */

#endif
