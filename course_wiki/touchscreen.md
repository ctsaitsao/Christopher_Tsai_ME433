Our [TFT LCD screen](https://www.aliexpress.com/item/240x320-2-8-SPI-TFT-LCD-Touch-Panel-Serial-Port-Module-with-PCB-ILI9341-5V-3/32815224002.html) comes with a resistive touchscreen. This is basically a two-dimensional transparent potentiometer with four wires. Sometimes these wires are broken out directly and you can read the signals with some clever pin IO and ADC code. In this case, a touch screen controller chip has been soldered to the LCD PCB, the [XPT2046](https://github.com/ndm736/ME433_2019/blob/master/XPT2046_touchscreen.pdf).

Review Figure 12, Table 6, and Table 5 in the datasheet. 

Figure 12 shows that the communication is SPI, and 3 8-bit transactions send a request to read from the chip and return a value. The first transaction sends the command and returns garbage. The second and third transactions send 0x00 and read the 12-bit value, led by one zero and trailed by 3 zeros. 

Table 6 shows the bits of the command byte. The left most bit is always 1. The next 3 bits are an address to read from, and the last 4 bits are related to setup and power. We will use 0b0001 as the last 4 bits.

Table 5 shows the address bits. There are 4 interesting values to read: the Y position, the X position, and Z1 and Z2, which can be used to estimate the pressure being applied to the screen, using the equation Z = Z1 - Z2 + 4095, where Z is the pressure (honestly I don't know where this comes from, it does not match the equation in the datasheet but it does not use division so it is fast and it still represents the amount of pressure).

Connect the T_(pins) to your SPI bus and initialize a new CS pin for the touchscreen. **My screen did not work well with the fastest SPI baud rate. Try a slow baud to get the screen to work, then slowly increase it until it stops working, then back down by one (mine used SPI1BRG = 3, which unfortunately will slow down your FPS).**

Write a function that reads the X, Y, and Z values, and returns them using pointers. The function prototype will look like:

> void XPT2046_read(unsigned short *x, unsigned short *y, unsigned int *z);

Call the function with something like:

> unsigned short x, y; int z;
> XPT2046_read(&x, &y, &z);

The function itself will use spi_io() to send the command byte or 0x00 and read bytes from the chip, then OR and shift the data into temporary values. Then set the values in main() using the pointers:

> ...
> *x = xtemp;
> *y = ytemp;
> *z = ztemp;
> ...

The values returned should be 0-4095, although practically they won't go all the way down to 0 or up to 4095. Be sure to manipulate them to math the coordinate system of the LCD pixels (top left is (0,0), increasing down and to the right).


Back to the [Schedule](https://github.com/ndm736/ME433_2019/wiki/Schedule)