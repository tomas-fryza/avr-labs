#ifndef TIMER_H
# define TIMER_H

/* 
 * Timer library for AVR-GCC.
 * (c) 2019-2024 Tomas Fryza, MIT license
 *
 * Developed using PlatformIO and AVR 8-bit Toolchain 3.6.2.
 * Tested on Arduino Uno board and ATmega328P, 16 MHz.
 */

/**
 * @file 
 * @defgroup fryza_timer Timer Library <timer.h>
 * @code #include <timer.h> @endcode
 *
 * @brief Timer library for AVR-GCC.
 *
 * The library contains macros for controlling the timer modules.
 *
 * @note Based on Microchip Atmel ATmega328P manual and no source file
 *       is needed for the library.
 * @copyright (c) 2019-2024 Tomas Fryza, MIT license
 * @{
 */

// -- Includes -------------------------------------------------------
#include <avr/io.h>


// -- Defines --------------------------------------------------------
/**
 * @name  Definitions for 16-bit Timer/Counter1
 * @note  t_OVF = 1/F_CPU * prescaler * 2^n where n = 16, F_CPU = 16 MHz
 */
/** @brief Stop timer, prescaler 000 --> STOP */
#define tim1_stop() TCCR1B &= ~((1<<CS12) | (1<<CS11) | (1<<CS10));

/** @brief Set overflow 4ms, prescaler 001 --> 1 */
#define tim1_ovf_4ms() TCCR1B &= ~((1<<CS12) | (1<<CS11)); TCCR1B |= (1<<CS10);

/** @brief Set overflow 33ms, prescaler 010 --> 8 */
#define tim1_ovf_33ms() TCCR1B &= ~((1<<CS12) | (1<<CS10)); TCCR1B |= (1<<CS11);

/** @brief Set overflow 262ms, prescaler 011 --> 64 */
#define tim1_ovf_262ms() TCCR1B &= ~(1<<CS12); TCCR1B |= (1<<CS11) | (1<<CS10);

/** @brief Set overflow 1s, prescaler 100 --> 256 */
#define tim1_ovf_1sec() TCCR1B &= ~((1<<CS11) | (1<<CS10)); TCCR1B |= (1<<CS12);

/** @brief Set overflow 4s, prescaler // 101 --> 1024 */
#define tim1_ovf_4sec() TCCR1B &= ~(1<<CS11); TCCR1B |= (1<<CS12) | (1<<CS10);

/** @brief Enable overflow interrupt, 1 --> enable */
#define tim1_ovf_enable() TIMSK1 |= (1<<TOIE1);

/** @brief Disable overflow interrupt, 0 --> disable */
#define tim1_ovf_disable() TIMSK1 &= ~(1<<TOIE1);


/**
 * @name  Definitions for 8-bit Timer/Counter0
 * @note  t_OVF = 1/F_CPU * prescaler * 2^n where n = 8, F_CPU = 16 MHz
 */
/** @brief Stop timer, prescaler 000 --> STOP */
#define tim0_stop() TCCR0B &= ~((1<<CS02) | (1<<CS01) | (1<<CS00));

/** @brief Set overflow 16us, prescaler 001 --> 1 */
#define tim0_ovf_16us() TCCR0B &= ~((1<<CS02) | (1<<CS01)); TCCR0B |= (1<<CS00);

/** @brief Set overflow 128us, prescaler 010 --> 8 */
#define tim0_ovf_128us() TCCR0B &= ~((1<<CS02) | (1<<CS00)); TCCR0B |= (1<<CS01);

/** @brief Set overflow 1ms, prescaler 011 --> 64 */
#define tim0_ovf_1ms() TCCR0B &= ~(1<<CS02); TCCR0B |= (1<<CS01) | (1<<CS00);

/** @brief Set overflow 4ms, prescaler 100 --> 256 */
#define tim0_ovf_4ms() TCCR0B &= ~((1<<CS01) | (1<<CS00)); TCCR0B |= (1<<CS02);

/** @brief Set overflow 16ms, prescaler // 101 --> 1024 */
#define tim0_ovf_16ms() TCCR0B &= ~(1<<CS01); TCCR0B |= (1<<CS02) | (1<<CS00);

/** @brief Enable overflow interrupt, 1 --> enable */
#define tim0_ovf_enable() TIMSK0 |= (1<<TOIE0);

/** @brief Disable overflow interrupt, 0 --> disable */
#define tim0_ovf_disable() TIMSK0 &= ~(1<<TOIE0);


/**
 * @name  Definitions for 8-bit Timer/Counter2
 * @note  t_OVF = 1/F_CPU * prescaler * 2^n where n = 8, F_CPU = 16 MHz
 */
/** @brief Stop timer, prescaler 000 --> STOP */
#define tim2_stop() TCCR2B &= ~((1<<CS22) | (1<<CS21) | (1<<CS20));

/** @brief Set overflow 16ms, prescaler // 111 --> 1024 */
#define tim2_ovf_16ms() TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);

/** @brief Enable overflow interrupt, 1 --> enable */
#define tim2_ovf_enable() TIMSK2 |= (1<<TOIE2);

/** @brief Disable overflow interrupt, 0 --> disable */
#define tim2_ovf_disable() TIMSK2 &= ~(1<<TOIE2);


/** @} */

#endif
