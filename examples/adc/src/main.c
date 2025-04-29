/***********************************************************************
 * 
 * Example of usage Analog-to-digital conversion to read push buttons
 * on LCD keypad shield.
 * (c) 2018-2024 Tomas Fryza, MIT license
 * 
 * Developed using PlatformIO v3.3.3 and Atmel AVR platform 5.0.0
 * Tested on Arduino Uno board and ATmega328P, 16 MHz.
 */

// -- Includes -------------------------------------------------------
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <uart.h>           // Peter Fleury's UART library
#include <lcd.h>            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for number conversions


// -- Defines --------------------------------------------------------
// ADC measured values
#define NONE_VALUE 1023
#define SELECT_VALUE 640
#define LEFT_VALUE 410
#define DOWN_VALUE 258
#define UP_VALUE 99
#define RIGHT_VALUE 0
#define NONE_THRESHOLD ((NONE_VALUE - SELECT_VALUE)/2 + SELECT_VALUE)
#define SELECT_THRESHOLD ((SELECT_VALUE - LEFT_VALUE)/2 + LEFT_VALUE)
#define LEFT_THRESHOLD ((LEFT_VALUE - DOWN_VALUE)/2 + DOWN_VALUE)
#define DOWN_THRESHOLD ((DOWN_VALUE - UP_VALUE)/2 + UP_VALUE)
#define UP_THRESHOLD ((UP_VALUE - RIGHT_VALUE)/2 + RIGHT_VALUE)


// -- Global variables -----------------------------------------------
volatile uint8_t flag_adc_completed = 0;


// -- Function definitions -------------------------------------------
void adc_init(void)
{
    // Select ADC voltage reference to "AVcc with external capacitor at AREF pin"
    ADMUX = ADMUX | (1<<REFS0);
    // Select input channel ADC0 (voltage divider pin)
    ADMUX = ADMUX & ~(1<<MUX3 | 1<<MUX2 | 1<<MUX1 | 1<<MUX0);
    // Enable ADC module
    ADCSRA = ADCSRA | (1<<ADEN);
    // Enable conversion complete interrupt
    ADCSRA = ADCSRA | (1<<ADIE);
    // Set clock prescaler to 128
    ADCSRA = ADCSRA | (1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0);
}


void timer1_init(void)
{
    // Configure 16-bit Timer/Counter1 to start ADC conversion
    tim1_ovf_33ms();
    tim1_ovf_enable();
}


/*
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and start ADC conversion every 100 ms.
 *           When AD conversion ends, display value to LCD screen.
 * Returns:  none
 */
int main(void)
{
    uint16_t value;
    char string[4];  // String for converted numbers by itoa()

    // Initialize display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(1, 0); lcd_puts("value:");
    lcd_gotoxy(3, 1); lcd_puts("key:");
    lcd_gotoxy(8, 0); lcd_puts("a");  // Put ADC value in decimal
    lcd_gotoxy(13,0); lcd_puts("b");  // Put ADC value in hexadecimal
    lcd_gotoxy(8, 1); lcd_puts("c");  // Put button name here

    adc_init();
    timer1_init();
    uart_init(UART_BAUD_SELECT(115200, F_CPU));

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        if (flag_adc_completed == 1)
        {
            // Note that, register pair ADCH and ADCL can be read as a 16-bit value ADC
            value = ADC;
            // Convert "value" to "string" and display it
            itoa(value, string, 10);
            lcd_gotoxy(8, 0);
            lcd_puts("    ");  // Clear previous value
            lcd_gotoxy(8, 0);
            lcd_puts(string);
            uart_puts(string);
            uart_puts("\r\n");

            // Hexa
            itoa(value, string, 16);
            lcd_gotoxy(13, 0);
            lcd_puts("   ");
            lcd_gotoxy(13, 0);
            lcd_puts(string);

            // Button name
            lcd_gotoxy(8, 1);
            lcd_puts("      ");
            lcd_gotoxy(8, 1);
            if (value > NONE_THRESHOLD)
                lcd_puts("none");
            else if (value > SELECT_THRESHOLD)
                lcd_puts("select");
            else if (value > LEFT_THRESHOLD)
                lcd_puts("left");
            else if (value > DOWN_THRESHOLD)
                lcd_puts("down");
            else if (value > UP_THRESHOLD)
                lcd_puts("up");
            else
                lcd_puts("right");

            flag_adc_completed = 0;
        }
    }

    // Will never reach this
    return 0;
}


// -- Interrupt service routines -------------------------------------
/*
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Use Single Conversion mode and start conversion every 100 ms.
 */
ISR(TIMER1_OVF_vect)
{
    // Start ADC conversion
    ADCSRA = ADCSRA | (1<<ADSC);
}


/*
 * Function: ADC complete interrupt
 * Purpose:  Let display converted value on LCD screen.
 */
ISR(ADC_vect)
{
    flag_adc_completed = 1;
}
