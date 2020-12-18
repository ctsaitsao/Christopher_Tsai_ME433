I2C is a bus-based communication protocol that uses only two pins (in addition to common ground). Connect the clock pins together (SCL), and connect the data pins together (SDA). Both connections need pull-up resistors, 10k works well.

The I2C Family Reference Manual pdf is missing from the PIC32MX250F128B website, you can find a copy of it [here](http://ww1.microchip.com/downloads/en/DeviceDoc/61116F.pdf).

I2C communication does not use a chip select pin, so every chip on the bus "hears" everything that is said. To talk to a specific chip, the first byte that is sent out is an address. All the chips on the bus listen to the address, but only chips that match that address continue to listen.

Addresses are 7-bit. The 0th bit in the address byte that is sent tells the chip that the next command is either a read (1) or a write (0) command.

I2C communication follows a strict state-machine, meaning that after sending one command, there are limited options in what to send next. Here is the state machine as described in the MCP23008 datasheet:
![](https://github.com/ndm736/ME433_2019/blob/master/i2c/i2c_state_machine.png)
The first part of a message is a Start bit, followed by an address with an embedded write bit.

If writing, the next byte sent is usually the name of the register you want to change, and then the next byte is the value you want to write into that register. The communication ends with a Stop bit.

Every time the Master writes something, the slave sends back an "Ack", or acknowledgment, so that the Master knows it can move on.

If reading, the next byte sent is usually the name of the register you want to read from. Then a Restart bit is sent, followed by the address with an embedded read bit. Then the Master makes its SDA pin an input, but continues to control the SCL pin. The Master outputs 8 bits on the SCL pin, and the Slave outputs the data to the Master on the SDA pin. Then the Master sends an Ack back to the Slave, and ends the communication with a Stop bit.

I2C code is annoying to write from scratch, so the functions are included in [the repo.](https://github.com/ndm736/ME433_2019/blob/master/i2c/i2c_master_noint.c) 

As an example, let's say you want to write the value 123 to register 5 in the chip with address 12. The code would look like:

i2c_master_start(); // make the start bit

i2c_master_send(12<1|0); // write the address, shifted left by 1, or'ed with a 0 to indicate writing

i2c_master_send(5); // the register to write to

i2c_master_send(123); // the value to put in the register

i2c_master_stop(); // make the stop bit


To read the value from register 7 in a chip with address 12, the code would look like:

i2c_master_start(); // make the start bit

i2c_master_send(12<1|0); // write the address, shifted left by 1, or'ed with a 0 to indicate writing

i2c_master_send(7); // the register to read from

i2c_master_restart(); // make the restart bit

i2c_master_send(12<1|1); // write the address, shifted left by 1, or'ed with a 1 to indicate reading

char r = i2c_master_recv(); // save the value returned

i2c_master_ack(1); // make the ack so the slave knows we got it

i2c_master_stop(); // make the stop bit


Before you can use the functions, you have to turn on the I2C peripheral, using the i2C_master_setup() function. You get to choose the bit rate, but it is typically 100kHz or 400kHz. The calculation depends on the PBCLK and a timing parameter called PGD, which is different for each type of PIC32. Search for PGD in the PIC32 datasheet to find it.

Back to the [Schedule](https://github.com/ndm736/ME433_2019/wiki/Schedule)