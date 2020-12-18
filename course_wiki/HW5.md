Homework 5 - I2C pin expander

Build the MCP23008 I/O expander circuit and write a library to communicate with it. Initialize pins GP0-3 as outputs, initially on, and GP4-7 as inputs. Build an LED circuit on pin GP0 and a pushbutton circuit on pin GP7. 

Write code that:
* uses the I/O expander to read pin GP7
* if GP7 is low, output high on GP0, otherwise output low

Use I2C2 to talk to the I/O expander. 

For I2C2, SDA2 is fixed to B2 and SCL2 is B3. 

Note that the I2C2 pins are analog input by default. Turn off analog using ANSELB, for example for B2, `ANSELBbits.ANSB2 = 0;`

The I2C functions already exist in the repo, but be sure to change them to use I2C2 instead of I2C1.

Write an initialization function for the I/O expander called `void initExpander()`, and functions that accomplish the tasks, like `void setExpander(char pin, char level)`, and `char getExpander()`. These functions should use the I2C functions. 

Use 400kHz as the baud for I2C2.

Save and commit your code to a git folder called HW5. Include a circuit diagram, and upload a video onto CANVAS demonstrating the LED turning on and off as you push the button.   

Hints:
* Start with HW1 as a template. Blink the LED controlled directly by the PIC at a few Hz so that as you start adding I2C code, you can tell if your PIC is frozen if the LED stops blinking.
* Connect the RESET pin to MCLR so that the Expander resets when the PIC does. If the PIC resets while in the middle of communicating over I2C but the chip you are talking to does not also get reset, then it is likely the chip will never respond again because the previous communication did not end properly. 
* First use I2C to turn on the LED on GP0, and test thoroughly. Then move on to reading GP7.

Back to the [Schedule](https://github.com/ndm736/ME433_2019/wiki/Schedule)