# Lab 2: Control of GPIO pins

* [Pre-Lab preparation](#preparation)
* [Part 1: PlatformIO](#part1)
* [Part 2: GPIO control registers](#part2)
* [Part 3: GPIO library files](#part3)
* [Part 4: SimulIDE electronic circuit simulator](#part4)
* [Challenges](#challenges)
* [References](#references)

### Component list

* Arduino Uno board, USB cable
* Breadboard
* 2 LEDs
* 1 two-color LED
* 4 resistors
* 1 push button
* Male to male jumper wires
* Logic analyzer

### Learning objectives

* Configure input/output ports of AVR using control registers
* Use ATmega328P manual and find information
* Understand the difference between header and source files
* Create your own library
* Understand how to call a function with pointer parameters

<a name="preparation"></a>

## Pre-Lab preparation

1. Any function in C contains a declaration (function prototype) and definition (block of code, body of the function), and each declared function can be executed (called). Study [this article](https://www.programiz.com/c-programming/c-user-defined-functions) and complete the missing sections in the following user defined function declaration, definition, and call.

   ```c
   #include <avr/io.h>

   // Function declaration (prototype)
   uint16_t calculate(uint8_t, ***    );

   int main(void)
   {
       uint8_t a = 210;
       uint8_t b = 15;
       uint16_t c;

       // Function call
       c = ***      (a, b);

       // Infinite loop
       while (1) ;

       // Will never reach this
       return 0;
   }

   // Function definition (body)
   ***      calculate(uint8_t x, uint8_t y)
   {
       uint16_t result;    // result = x^2 + 2xy + y^2

       result = x*x;
       ***
       ***
       return result;
   }
   ```

2. Find differences between `variable` and `pointer` in C. What mean notations `*variable` and `&variable`?

<a name="part1"></a>

## Part 1: PlatformIO

1. Run Visual Studio Code, follow [instructions](../README.md) and install the PlatformIO plugin.

2. Create a new project `lab2-gpio`, select `Arduino Uno` board, and change project location to your local folder, such as `Documents`.

3. IMPORTANT: Rename `LAB2-GPIO > src > main.cpp` file to `main.c`, ie change the extension to `.c`.

4. Copy/paste [blink example code](https://raw.githubusercontent.com/tomas-fryza/avr-course/master/examples/blink/main.c) to your `LAB2-GPIO > src > main.c` file.

5. Compile and upload the firmware to ATmega328P microcontroller. Change the delay duration and observe the behavior of on-board LED. See Arduino Uno [pinout](https://docs.arduino.cc/static/6ec5e4c2a6c0e9e46389d4f6dc924073/2f891/Pinout-UNOrev3_latest.png):

   ![arduino uno pinout](images/arduino_uno-pinout.png)

<a name="part2"></a>

## Part 2: GPIO control registers

AVR microcontroller associates pins into so-called ports, which are marked with the letters A, B, C, etc. Each of the pins is controlled separately and can function as an input (entry) or output (exit) point of the microcontroller. Control is possible exclusively by software via control registers.

There are three control registers for each port: DDR, PORT and PIN, supplemented by the letter designation of the port. For port A these are registers DDRA, PORTA and PINA, for port B registers DDRB, PORTB, PINB, etc.

DDR (Data Direction Register) is used to set the input/output direction of port communication, PORT register is the output data port and PIN register works for reading input values from the port.

A detailed description of working with input/output ports can be found in [ATmega328P datasheet](https://www.microchip.com/wwwproducts/en/ATmega328p) in section I/O-Ports.

1. Use the datasheet to find out the meaning of the DDRB and PORTB control register values and their combinations. (Let PUD (Pull-up Disable) bit in MCUCR (MCU Control Register) is 0 by default.)

   | **DDRB** | **PORTB** | **Direction** | **Internal pull-up resistor** | **Description** |
   | :-: | :-: | :-: | :-: | :-- |
   | 0 | 0 | input | no | Tri-state, high-impedance |
   | 0 | 1 | | | |
   | 1 | 0 | | | |
   | 1 | 1 | | | |

2. To control individual bit(s) within a register, the following binary operations are used.
   1. `|` OR
   2. `&` AND
   3. `^` XOR
   4. `~` NOT
   5. `<<` binary shift to left

   | **b** | **a** |**b OR a** | **b AND a** | **b XOR a** | **NOT b** |
   | :-: | :-: | :-: | :-: | :-: | :-: |
   | 0 | 0 | 0 | 0 | 0 | 1 |
   | 0 | 1 | 1 | 0 | 1 | 1 |
   | 1 | 0 | 1 | 0 | 1 | 0 |
   | 1 | 1 | 1 | 1 | 0 | 0 |

   ```c
   // Set bit value to 1
   reg = reg | (1<<bit);

   // Clear bit value to 0
   reg = reg & ~(1<<bit);

   // Toggle bit value
   reg = reg ^ (1<<bit);
   ```

   ![binary operations](images/binary_operations.png)

<a name="part3"></a>

## Part 3: GPIO library files

For clarity and efficiency of the code, the individual parts of the application in C are divided into two types of files: header files and source files. Note that together they form one module.

**Header file** is a file with extension `.h` and generally contains definitions of data types, function prototypes and C preprocessor commands. **Source file** has the extension `.c` and is used to implement the code. It is bad practice to mix usage of the two although it is possible.

C programs are highly dependent on functions. Functions are the basic building blocks of C programs and every C program is combination of one or more functions. There are two types of functions in C: **built-in functions** which are the part of C compiler and **user defined functions** which are written by programmers according to their requirement.

To use a user-defined function, there are three parts to consider:

* Function declaration or Function prototype (`*.h` file)
* Function definition (`*.c` file)
* Function call (`*.c` file)

   ![user library](images/user_library.png)

*[A function prototype](https://www.programiz.com/c-programming/c-user-defined-functions) is simply the declaration of a function that specifies function's name, parameters and return type. It doesn't contain function body. A **function prototype** gives information to the compiler that the function may later be used in the program.*

*A **function definition** contains the block of code to perform a specific task.*

*By **calling the function**, the control of the program is transferred to the function.*

A header file can be shared between several source files by including it with the C preprocessing directive `#include`. If a header file happens to be included twice, the compiler will process its contents twice and it will result in an error. The standard way to prevent this is to enclose the entire real contents of the file in a conditional, like this:

```c
#ifndef HEADER_FILE_NAME        // Preprocessor directive allows for conditional compilation. If not defined.
# define HEADER_FILE_NAME       // Definition of constant within your source code.

// The body of entire header file

#endif                          // The #ifndef directive must be closed by an #endif
```

This construct is commonly known as a wrapper `#ifndef`. When the header is included again, the conditional will be false, because `HEADER_FILE_NAME` is already defined. The preprocessor will skip over the entire contents of the file, and the compiler will not see it twice.

1. In PlatformIO project, create a new folder `LAB2-GPIO > lib > gpio`. Within this folder, create two new files `gpio.c` and `gpio.h`. See the project structure:

   ```c
   LAB2-GPIO           // PlatfomIO project
   ├── include         // No need this
   ├── lib             // Libraries
   │   └── gpio        // Our new GPIO library
   │       ├── gpio.c
   │       └── gpio.h
   ├── src             // Source file(s)
   │   └── main.c
   ├── test            // No need this
   └── platformio.ini  // Project Configuration File
   ```

   1. Copy/paste the [library source file](https://raw.githubusercontent.com/tomas-fryza/avr-course/master/library/gpio/gpio.c) to `gpio.c`
   2. Copy/paste the [header file](https://raw.githubusercontent.com/tomas-fryza/avr-course/master/library/gpio/gpio.h) to `gpio.h`
   3. Include header file to `src > main.c`:

      ```c
      // -- Includes -------------------------------------------------------
      #include <gpio.h>       // GPIO library for AVR-GCC
      ...

      int main(void)
      {
          ...
      }
      ```

2. Go through both files and make sure you understand each line. The GPIO library defines the following functions.

   | **Return** | **Function name** | **Function parameters** | **Description** |
   | :-: | :-- | :-- | :-- |
   | `void` | `gpio_mode_output` | `volatile uint8_t *reg, uint8_t pin` | Configure one output pin |
   | `void` | `gpio_mode_input_pullup` | `volatile uint8_t *reg, uint8_t pin` | Configure one input pin and enable pull-up resistor |
   | `void` | `gpio_write_low` | `volatile uint8_t *reg, uint8_t pin` | Write one pin to low value |
   | `void` | `gpio_write_high` | `volatile uint8_t *reg, uint8_t pin` | Write one pin to high value |
   | `uint8_t` | `gpio_read` | `volatile uint8_t *reg, uint8_t pin` | Read a value from input pin |

   > **Note:** Suggestions for other features you can add are:
   >
   > * `void gpio_mode_input_nopull(volatile uint8_t *reg, uint8_t pin)` Configure one input pin without pull-up resistor
   > * `void gpio_toggle(volatile uint8_t *reg, uint8_t pin)` Toggle one pin value

   The register name parameter must be `volatile` to avoid a compiler warning. Note that the C notation `*variable` representing a pointer to memory location where the variable's **value** is stored. Notation `&variable` is address-of-operator and gives an **address** reference of variable.

   ```c
   int main(void)
   {
       ...
       // Examples of various function calls
       gpio_mode_output(&DDRB, LED_BUILTIN);  // Set output mode in DDRB reg
       ...
       gpio_write_low(&PORTB, LED_BUILTIN);   // Set output low in PORTB reg
       ....
       temp = gpio_read(&PIND, BTN);          // Read input value from PIND reg
       ...
   }
   ```

   > ![pointers](images/pointers.png)
   >
   > **Note:** Understanding C Pointers: A Beginner's Guide is available [here](https://www.codewithc.com/understanding-c-pointers-beginners-guide/). Explanation of how to pass an IO port as a parameter to a function is given [here](https://www.eit.lth.se/fileadmin/eit/courses/eita15/avr-libc-user-manual-2.0.0/FAQ.html#faq_port_pass).

3. In `main.c` comment bitwise operations with control registers (DDRB, PORTB) and rewrite the application with library functions.

4. On a breadboard, connect a LED or a [two-color LED](http://lednique.com/leds-with-more-than-two-pins/) (3-pin LED) and resistor(s) to pin(s) PB0 (and PB1). Develop the code to achieve alternating blinking of two LEDs.

5. On a breadboard, connect an active-low push button to pin PD2. In your code, activate the internal pull-up resistor on this pin. Make the LEDs blink only when the button is pressed.

   ![schema of active-low push button](images/schema_button_active-low.png)

<a name="part4"></a>

## Part 4: SimulIDE electronic circuit simulator

1. Run SimulIDE, use online [tutorials](https://www.simulide.com/p/blog-page.html), and create a circuit with ATmega328 AVR microcontroller.

2. All circuit and control elements are available in the **Components** tab. Use the following components: ATmega328 (**Micro > AVR > atmega > atmega328**), resistor (**Passive > Resistors > Resistor**), LED (**Outputs > Leds > Led**), and GND (**Sources > Ground (0 V)**) and connect them as shown.

   ![SimulIDE](images/screenshot_simulide.png)

3. Right-click on the ATmega package and select **Load firmware**. In your project folder, find the `*.hex` file that was created by the previous compilation process.

4. Right-click on the ATmega package and select **Open Mcu Monitor** to view internal registers and memory contents. Select **Variable** folder. In the **Reg.** column, type `DDRB` on the first line and `PORTB` on the second.

5. Click to **Power Circuit** button at the top of the window to simulate the project and monitor the LED status and register values. The simulation can be paused with the **Pause Simulation** button and stopped by pressing the **Power Circuit** button again.

6. You can connect a probe (**Meters > Probe**), an oscilloscope (**Meters > Oscope**), a voltmeter (**Meters > Voltimeter**), or a frequency meter (**Meters > Frequencimeter**) to output B5 and observe the signal.

7. Properties of individual components can be found/changed by right-clicking on the component and selecting **Properties**.

<a name="challenges"></a>

## Challenges

1. Complete declarations (`*.h`) and definitions (`*.c`) of GPIO suggested functions `gpio_mode_input_nopull` and `gpio_toggle`.

2. Connect at least five LEDs and one push button to the microcontroller and program an application in [Knight Rider style](https://www.youtube.com/watch?v=w-P-2LdS6zk). When you press and release a push button once, the LEDs starts to switched on and off; ensure that only one of LEDs is switched on at a time. Do not implement the blinking speed changing.

3. Simulate the Knight Rider application in [SimulIDE](https://simulide.com/p/).

4. Draw a schematic of Knight Rider application. The image can be drawn on a computer or by hand. Always name all components, their values and pin names!

<a name="references"></a>

## References

1. Parewa Labs Pvt. Ltd. [C User-defined functions](https://www.programiz.com/c-programming/c-user-defined-functions)

2. [Understanding C Pointers: A Beginner's Guide](https://www.codewithc.com/understanding-c-pointers-beginners-guide/)

3. avr-libc. [How do I pass an IO port as a parameter to a function?](https://www.eit.lth.se/fileadmin/eit/courses/eita15/avr-libc-user-manual-2.0.0/FAQ.html#faq_port_pass)

4. LEDnique. [LED pinouts - 2, 3, 4-pin and more](http://lednique.com/leds-with-more-than-two-pins/)
