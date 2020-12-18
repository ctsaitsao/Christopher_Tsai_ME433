SPI is a fast method of digital communication (as fast as 20MHz (bits/s)), used to talk between microcontrollers, sensors, and memory (SD cards use a form of SPI). Devices share three wires, plus one more for every device on the bus. Communication typically stops working when the wires are more than a foot long.

The four SPI wires, in addition to common ground, are:
* SCK - a clock, controlled by the Master device
* SDI - data input to the Master (also called MISO)
* SDO - data output from the Master (also called MOSI)
* SS - chip select output from the Master (also called CS)

Sometimes the SS can be left off, and sometimes SDI is not used. Check the datasheet of the chip you are talking to.

Connect the Master and Slave SCK and SS. Connect the SDOs to the SDIs. No pull-up resistors are needed.

SPI initialization is pretty straight forward - most devices use the same polarity and timing, so the default values for the SPI peripheral can be used. You can enable the automatic SS pin if you are only talking to one slave, but usually you manually control any pin as your SS if you are talking to more than one chip. Another typical change is how many bits are transferred, typically 8-bits are sent at a time, but some chips use 16- or 32-bit communication. 

The main option to change is the bit rate, based on your PBCLK. Typically you want to go as fast as the chip you are talking to will allow, but you can also slow it down, making it easier to see with an oscilloscope and more visible for debugging. The register, SPIxBRG, must be an integer, so not every bit rate is possible.

The strangest part of SPI on the PIC32 is the use of a single register, SPIxBUF, that contains both the output and input data. To write data out of SPI, you put the data into the SPIxBUF register, and wait for the peripheral to send out the data (wait until the SPIRBF bit to goes high). At that moment, the data the PIC32 read while outputting your data is loaded into SPIxBUF, so you can then copy whatever was in SPIxBUF into your read variable. 

Back to the [Schedule](https://github.com/ndm736/ME433_2019/wiki/Schedule)