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


/* Includes ----------------------------------------------------------*/
#ifndef F_CPU
# define F_CPU 16000000
#endif
#include <util/delay.h>
#include "gpio.h"
#include "segment.h"


/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: seg_init()
 * Purpose:  Configure SSD signals LATCH, CLK, and DATA as output.
 * Returns:  none
 **********************************************************************/
void seg_init(void)
{
    /* Configuration of SSD signals */
    gpio_config_output(&DDRD, SEG_LATCH);
    gpio_config_output(&DDRD, SEG_CLK);
    gpio_config_output(&DDRB, SEG_DATA);
}


/**********************************************************************
 * Function: seg_update_shift_regs()
 * Purpose:  Display segments at one position of the SSD.
 * Input:    segments - Segments to be displayed (abcdefgDP, active low)
 *           position - Position of the display where the segments are to 
 *                      be displayed (p3 p2 p1 p0 xxxx, active high)
 * Returns:  none
 **********************************************************************/
void seg_update_shift_regs(uint8_t segments, uint8_t position)
{
    uint8_t bit_number;

    // Pull LATCH, CLK, and DATA low

    // Wait 1 us

    // Loop through the 1st byte (segments)
    // a b c d e f g DP (active low values)
    for (bit_number = 0; bit_number < 8; bit_number++)
    {
        // Test LSB of "segments" by & (faster) or % (slower) and... 
        // ...output DATA value

        // Wait 1 us

        // Pull CLK high

        // Wait 1 us

        // Pull CLK low

        // Shift "segments"
        segments = segments >> 1;
    }

    // Loop through the 2nd byte (position)
    // p3 p2 p1 p0 . . . . (active high values)
    for (bit_number = 0; bit_number < 8; bit_number++)
    {
        // Test LSB of "position" by & (faster) or % (slower) and... 
        // ...output DATA value

        // Wait 1 us

        // Pull CLK high

        // Wait 1 us

        // Pull CLK low

        // Shift "position"
        position = position >> 1;
    }

    // Pull LATCH high

    // Wait 1 us

}


/**********************************************************************
 * Function: seg_clear()
 **********************************************************************/


/**********************************************************************
 * Function: seg_clk_2us()
 **********************************************************************/
