This homework is the first half of HW3, which is due next week. You must create a PCB of your PIC32 circuit, 10cm x 10cm, with header pins at specific locations. Do all of the error checking, generate the Gerber files, and zip them up in a file called YourLastName_YourFirstName_pcb.zip. If you don't get HW3 in on time, your board won't be ordered and you'll have to use one that I design.

This first half, HW2, is to keep you on schedule. Make a library of components for your board, draw the schematic, and start the board layout. Upload your .lbr, .sch, and .brd to git.

Watch the following videos as an example:

[Youtube libraries video](https://youtu.be/LUk2yj2UKlE)


[Youtube schematic video](https://youtu.be/hp5ahpYY9M8)

Create a new folder in your repo called HW2. Open EAGLE and make a new project in HW2. Right-click on the project and make a new library. Follow the instruction on the [EAGLElibraries](https://github.com/ndm736/ME433_2019/wiki/EAGLElibraries) page to copy parts from other libraries to your own, and create the parts that you can't find (you should be able to find Packages for everything, and only have to make a few Symbols and Devices from scratch).

Here are the parts you need (and a Digikey link so you can find the datasheet):
* [PIC32MX270F256D](https://www.digikey.com/product-detail/en/microchip-technology/PIC32MX270F256D-I-PT/PIC32MX270F256D-I-PT-ND/4876287)
* [L4931](https://www.digikey.com/product-detail/en/stmicroelectronics/L4931CZ33-AP/497-7246-1-ND/1884870)
* [USB micro B](https://www.digikey.com/product-detail/en/amphenol-icc-fci/10118194-0001LF/609-4618-1-ND/2785382)
* [Push button](http://www.digikey.com/product-search/en?keywords=P8072SCT-ND)
* [8MHz resonator](http://www.digikey.com/product-search/en?keywords=490-1212-ND)
* [red LED](http://www.digikey.com/product-search/en?keywords=754-1211-ND)
* [green LED](http://www.digikey.com/product-search/en?keywords=754-1210-ND)
* [10uF polarized capacitor](http://www.digikey.com/product-search/en?keywords=478-9320-ND)
* [0.1uF capacitor](http://www.digikey.com/product-search/en?keywords=BC1148CT-ND)
* [330 ohm resistor](http://www.digikey.com/product-search/en?keywords=CF14JT330RTR-ND)
* [10k resistor](http://www.digikey.com/product-search/en?keywords=CF14JT10K0TR-ND)

In addition to the circuit you designed, add a slide switch so that you can turn off power without having to unplug the USB cable. The part is
* [slide switch](http://www.digikey.com/product-search/en?keywords=A107673-ND)

You will have to make a new Symbol, Package, and Device for it. In your .sch, insert it between the USB VBUS pin and your 5V connections.

Your PIC32 will have x pins unconnected when you are done. You will want to be able to connect to those, as well as the LED and user button pins, and 5V, 3.3V, and GND, to another PCB or breadboard, so you will need at least x header pin connections as well.

Add all of the parts to your .sch, including x CONN_0x parts in the 1x0x Package.

Draw the connections (using the net tool!) and connect the unused PIC32 pins to the CONN_0x parts. The CONN_0xs should be connected like this:
First CONN_0x:
* A0
* A1
* B2
* B3
* B4
* A4
* 3.3V
* GND

Second CONN_0x:
* B15
* B14
* B13
* B9
* B8
* B7
* 5V
* GND

Back to the [Schedule](https://github.com/ndm736/ME433_2019/wiki/Schedule)