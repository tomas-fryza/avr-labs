#ifndef GPIO_H
# define GPIO_H

/* 
 * GPIO library for AVR-GCC.
 * (c) 2019-2024 Tomas Fryza, MIT license
 *
 * Developed using PlatformIO and AVR 8-bit Toolchain 3.6.2.
 * Tested on Arduino Uno board and ATmega328P, 16 MHz.
 */

/**
 * @file 
 * @defgroup fryza_gpio GPIO Library <gpio.h>
 * @code #include <gpio.h> @endcode
 *
 * @brief GPIO library for AVR-GCC.
 *
 * The library contains functions for controlling AVRs' gpio pin(s).
 *
 * @note Based on AVR Libc Reference Manual.
 * @copyright (c) 2019-2024 Tomas Fryza, MIT license
 * @{
 */

// -- Includes -------------------------------------------------------
#include <avr/io.h>


// -- Function prototypes --------------------------------------------
/**
 * @brief  Configure one output pin.
 * @param  reg Address of Data Direction Register, such as &DDRB
 * @param  pin Pin designation in the interval 0 to 7
 * @return none
 */
void gpio_mode_output(volatile uint8_t *reg, uint8_t pin);


/**
 * @brief  Configure one input pin and enable pull-up.
 * @param  reg Address of Data Direction Register, such as &DDRB
 * @param  pin Pin designation in the interval 0 to 7
 * @return none
 */
void gpio_mode_input_pullup(volatile uint8_t *reg, uint8_t pin);


/**
 * @brief  Write one pin to low value.
 * @param  reg Address of Port Register, such as &PORTB
 * @param  pin Pin designation in the interval 0 to 7
 * @return none
 */
void gpio_write_low(volatile uint8_t *reg, uint8_t pin);


/**
 * @brief  Write one pin to high value.
 * @param  reg Address of Port Register, such as &PORTB
 * @param  pin Pin designation in the interval 0 to 7
 * @return none
 */
void gpio_write_high(volatile uint8_t *reg, uint8_t pin);


/**
 * @brief  Read a value from input pin.
 * @param  reg Address of Pin Register, such as &PIND
 * @param  pin Pin designation in the interval 0 to 7
 * @return Pin value
 */
uint8_t gpio_read(volatile uint8_t *reg, uint8_t pin);


void gpio_mode_input_nopull(volatile uint8_t *reg, uint8_t pin);


void gpio_toggle(volatile uint8_t *reg, uint8_t pin);


/** @} */

#endif
