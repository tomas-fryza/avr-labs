/* 
 * Read values from I2C (TWI) temperature/humidity sensor and send
 * them to OLED screen.
 * (c) 2023-2024 Tomas Fryza, MIT license
 *
 * Developed using PlatformIO and AVR 8-bit Toolchain 3.6.2.
 * Tested on Arduino Uno board and ATmega328P, 16 MHz.
 */

// -- Includes -------------------------------------------------------
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <twi.h>            // I2C/TWI library for AVR-GCC
#include <oled.h>
#include <stdio.h>          // C library for `sprintf`


// -- Defines --------------------------------------------------------
#define DHT_ADR 0x5c
#define DHT_HUM_MEM 0
#define DHT_TEMP_MEM 2


// -- Global variables -----------------------------------------------
volatile uint8_t flag_update_oled = 0;
volatile uint8_t dht12_values[5];


// -- Function definitions -------------------------------------------
void oled_setup(void)
{
    oled_init(OLED_DISP_ON);
    oled_clrscr();

    oled_charMode(DOUBLESIZE);
    oled_puts("OLED disp.");

    oled_charMode(NORMALSIZE);

    oled_gotoxy(0, 2);
    oled_puts("128x64, SH1106");

    // oled_drawLine(x1, y1, x2, y2, color)
    oled_drawLine(0, 25, 120, 25, WHITE);

    oled_gotoxy(0, 4);
    oled_puts("AVR course, Brno");

    oled_gotoxy(0, 6);
    oled_puts("Temperature [C]:");
    oled_gotoxy(0, 7);
    oled_puts("Humidity    [\%]:");

    // Copy buffer to display RAM
    oled_display();
}


void timer1_init(void)
{
    tim1_ovf_1sec();
    tim1_ovf_enable();
}


/*
 * Function: Main function where the program execution begins
 * Purpose:  Wait for new data from the sensor and sent them to OLED.
 * Returns:  none
 */
int main(void)
{
    char oled_msg[5];

    twi_init();
    oled_setup();
    timer1_init();

    sei();

    // Infinite loop
    while (1)
    {
        if (flag_update_oled == 1)
        {
            // Clear previous temperature value on OLED
            oled_gotoxy(17, 6);
            oled_puts("    ");

            // Display new temperature data
            oled_gotoxy(17, 6);
            sprintf(oled_msg, "%u.%u", dht12_values[2], dht12_values[3]);
            oled_puts(oled_msg);

            // Clear previous humidity values on OLED
            oled_gotoxy(17, 7);
            oled_puts("    ");

            // Display new humidity data
            oled_gotoxy(17, 7);
            sprintf(oled_msg, "%u.%u", dht12_values[0], dht12_values[1]);
            oled_puts(oled_msg);

            oled_display();

            // Do not print it again and wait for the new data
            flag_update_oled = 0;
        }
    }

    // Will never reach this
    return 0;
}


// -- Interrupt service routines -------------------------------------
/*
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Read temperature and humidity from DHT12, SLA = 0x5c.
 */
ISR(TIMER1_OVF_vect)
{
    static uint8_t n_ovfs = 0;

    n_ovfs++;
    // Read the data every 5 secs
    if (n_ovfs >= 5)
    {
        n_ovfs = 0;
        twi_readfrom_mem_into(DHT_ADR, DHT_HUM_MEM, dht12_values, 5);
        flag_update_oled = 1;
    }
}
