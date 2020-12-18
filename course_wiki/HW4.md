Homework 4 - SPI DAC 

Build the MCP49x2 DAC circuit and write a library to communicate with it. The important function should be

`void setVoltage(char channel, int voltage)` where channel is 0 or 1 (for VoutA and VoutB), and voltage is the 8/10/12-bit output level.

Write code that:
* the DAC outputs a 10Hz sine wave on VoutA and a 5Hz triangle wave on VoutB, updating the values 1000 times a second
![](https://github.com/ndm736/ME433_2019/blob/master/images/HW4_dac.PNG)

Use SPI1 to talk to the DAC.  

For SPI1, SCK1 is fixed to B14, but you get to choose which pins to use for SDI1 (actually not connected to the DAC, but pick it out anyway), SDO1, and SS1. Use the input and output tables from the [pin Functions pdf](https://github.com/ndm736/ME433_2019/blob/master/PIC32MX250F128B_pinFunctions.pdf) to find out what functions the remaining pins can take.

To assign a function to a pin:
* if the function is an output (like SDO, SS, or OC), the pin points to the function. For example, to use PWM, `RPB15Rbits.RPB15R = 0b0101;` assigns OC1 to pin B15.
* if the function is an input (like SDI or INT), the function points to the pin. For example, to use an external interrupt pin, `INT4Rbits.INT4R = 0b0011;` assigns INT4 to pin B15.

For your library, write an initialization function for the communication, like `void initSPI1()` and functions that do generic communication, like `char SPI1_IO(char write)`. 

When you initialize SPI1, use the fastest bit rate possible (SPI1BRG must be an integer >=1). Do not enable automatic control of SS1, just use your pin as a digital pin (initialized with TRIS and set with LAT).  

You can put these functions in main.c, or make a library like spi.c and spi.h.

Save and commit your code to a git folder called HW4. Include a screenshot or photo of an oscilloscope showing the DAC outputs, and a drawing of your circuit schematic.   

Back to the [Schedule](https://github.com/ndm736/ME433_2019/wiki/Schedule)