An IMU, or inertial measurement unit, is a sensor that is used to calculate position, velocity, or acceleration. Modern IMUs are [MEMS devices](https://en.wikipedia.org/wiki/Microelectromechanical_systems), with built in analog to digital converters, and their own dedicated microcontroller to control and process the data ([example from Digikey](https://www.digikey.com/en/articles/techzone/2018/jan/apply-sensor-fusion-to-accelerometers-and-gyroscopes?utm_medium=email&utm_source=evn&utm_campaign=30701_EVN1806A&utm_content=LearnMore2_US&utm_cid=5340605&WT.v_sub=1002993&WT.mc_id=em_EVN1806A.US.Send&WT.z_email=30701_EVN1806A0B0US_LearnMore2&mkt_tok=eyJpIjoiTUROaFlqSTFZVGhsWXpjeSIsInQiOiJsMlg3UFZJTjlNd1BiYnZCNkozQjhLckxuZFBBRFZocGI5cUtLUHQ2a0x3clwvTmRLa2xlRmxTV2tKRzRjbVRZSVQ1VmhnTCtMOHJROG9DdnEyODlxUHhvQzVzYWNneXg4Tk5OUTNwNUxBV0wyMWVkd0Mybzl3SFlsRzFkcnFyTDEifQ%3D%3D)).

Common IMUs are accelerometers, gyroscopes, magnetometers, and altimeters. Often they come packaged together in a single chip (although altimeters are still new and rare). The sensors communicate over I2C or SPI.

We will use the [LSM6DS33](https://www.pololu.com/file/0J1087/LSM6DS33.pdf) accelerometer and gyroscope chip made by ST, soldered onto a breakout board by [Pololu](https://www.pololu.com/product/2736).

This chip measures accelerometer and gyroscope data about the X, Y, and Z axes with 16 bit resolution, with a variety of sensitivities. It contains a temperature sensor so that it can self correct for temperature effects, and we can also access the temperature data as a 16 bit number.

The LSM6DS33 can communicate over I2C or SPI. Our SPI bus has the LCD on it, so we will use I2C to talk to the IMU. (You can disassemble your IO pin expander circuit and replace it with the IMU).

Pololu has made it easy to hook up the chip. Connect 3.3V to VIN, GND to GND, SDA2 to SDA, and SCL2 to SCL. The 10k pull up resistors are built onto the breakout board, so you don't have to add them. The Pololu PCB has a pull up resistor on the SA0 pin, so the 7 bit address of the chip is 0b1101011. The maximum baud is 400kHz.

Initialize I2C2 as you did for the IO pin expander. Then, write to several registers to initialize the chip. Once initialized, you can read from the data registers to figure out the acceleration, angular velocity, and temperature of the chip.

The first challenge is to figure out which registers to write to to initialize the chip. This chip has many cool features, but we just want the bare bones ability to read the three acceleration, three angular velocity, and temperature data at any time. Out of the dozen registers, we only need to change three.

To turn on the accelerometer, write to the CTRL1_XL register. Set the sample rate to 1.66 kHz, with 2g sensitivity, and 100 Hz filter.

To turn on the gyroscope, write to the CTRL2_G register. Set the sample rate to 1.66 kHz, with 1000 dps sensitivity.

The last register to change is CTRL3_C, which contains the IF_INC bit. Making the IF_INC bit 1 will enable the ability to read multiple registers in a row without specifying every register location, saving us time when reading out all of the data. 

With IF_INC set, we can read every register from OUT_TEMP_L to OUTZ_H_XL in a single read. Write a specific I2C_read_multiple() function. The prototype should look like:

`void I2C_read_multiple(unsigned char address, unsigned char register, unsigned char * data, int length)`

Call the function with the 7 bit address of the chip and the OUT_TEMP_L register. Send the pointer to an array of unsigned chars as data, and the size of the array as length. There are 14 8-bit bytes to read, to recombine into 7 16-bit shorts containing the temperature, three axes of gyroscope data, and three axes of accelerometer data, so make the data array have 14 values, and send 14 as the length.

Also note, if you wanted to read only the accelerometer data, you could use the same function, sending the address, the OUTX_L_XL register, and an array with a length of 6. 

Note that you can read data from any chip like this, as long as the registers you need are sequential.

Once you have your unsigned char array, reconstruct the signed short by shifting the high byte and ORing it with the low byte.

The I2C_read_multiple() function works just like reading from the IO pin expander, except the i2c_master_recv() function should be in a for loop, length times. After every recv, do a i2c_master_ack(0), to let the chip know you want to continue to read. The very last read should use a i2c_master_ack(1), to let the chip know you are done communicating.

Back to the [Schedule](https://github.com/ndm736/ME433_2019/wiki/Schedule)