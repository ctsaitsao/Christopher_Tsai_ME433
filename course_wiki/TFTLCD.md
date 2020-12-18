
In ME333 we interfaced to a simple 16x2 character display with the NU32, using something like [this LCD from Sparkfun.](https://www.sparkfun.com/products/255) This type of display typically uses parallel communication, requiring lots of pins. The PIC32MX250 is lacking in pins, so we'll use a display that uses SPI (3 pins + 1 control pin) and get a larger area to write to as well.  

An LCD is a two dimensional array of pixels used to display data. Each pixel has three little screens, tinting the light that passes through them red, green, and blue. A white backlight behind all of the pixels passes through the screens, creating the color you see. 

We will use a [240 x 320 pixel TFT LCD](https://www.aliexpress.com/item/240x320-2-8-SPI-TFT-LCD-Touch-Panel-Serial-Port-Module-with-PCB-ILI9341-5V-3/32815224002.html), 2.8 inches diagonal, communicating via a special type of SPI. It also has a resistive touch screen! We'll use that later.

The LCD has a built in controller, a ILI9341. With the highest color depth resolution, it takes 240 x 320 x 3 = 230400 bytes of data to fill the screen! That's a lot of memory. We will not be able to "double buffer" our screen, so you have to tell the screen the color of a pixel as you calculate it.

The files ili9341.c and ili9341.h contain functions that:
* initialize SPI and the associated pins
* initialize the LCD 
* write a color to a single pixel at the location (x,y), where (0,0) is the top-left of the screen.
* set the color of the background (make all the pixels the same color)

The code uses SPI1:
* SDI1 is B8 (not used but still assigned)
* SDO1 is A1
* SCK1 is B14
* CS is B7  

The LCD requires an additional pin to know if a byte is a command or represents data. This code uses:
* DC is B15

Leave the Touch pins unconnected for now.
The 9 LCD pins are connected to:
* VCC - 3.3V
* GND - 0V
* CS - to PIC B7
* RESET - 3.3V
* DC - to PIC B15
* SDI(MOSI) - to PIC A1
* SCK - to PIC B14
* LED - 3.3V
* SDO(MISO) - to PIC B8, although this functionality isn't used

**On the back of the LCD are two unconnected pads labeled J1 - SOLDER THEM TOGETHER SO THE SCREEN WORKS WITH 3.3V.**

ili9341.h contains a two-dimensional constant array called `static const char ASCII[96][5]`. Every row in ASCII contains 5 chars whose bits describe the image of a character (if you literally drew the bits of a byte in a column, with the bytes next to each other). For example, a space is 5 0x00's, or a blank 8x5 pixel image. A "!" is two blank columns, a column that draws a !, and two more blank columns. The variable ASCII describes a fixed-width font! The rows are in the order presented by the [ascii table](https://en.wikipedia.org/wiki/ASCII#Printable_characters), the same one used by sprintf() to convert chars into letters and numbers. ASCII will be used in the homework to draw text to the LCD.

Back to the [Schedule](https://github.com/ndm736/ME433_2019/wiki/Schedule)