HW7

First, check your circuit by reading from the WHOAMI register, you should get 0b01101001 (decimal 105). If you don't, debug your connections and I2C code.

When you get the correct response from WHOAMI, write an I2CmultipleRead() function to be able to read many registers in a row. Read all of the temperature, gyroscope, and accelerometer data registers, and shift them into signed shorts (temperature, gyroX, gyroY, gyroZ, accelX, accelY, accelZ).

Read the LSM6DS33 at 20 Hz, and blink your LED as a heartbeat. Draw bars starting from the center of the LCD in the direction of the chip +X and +Y directions. Scale the length of the bars to match the readings from the accelerometer. It may help to write the text values of the readings as you debug.   

Post your code in a folder called HW7 in your repo. Upload a video of your LCD as you rotate the accelerometer around.

Hint: the IMU does not have a reset pin, so it is likely that in the middle of a read, the PIC will reset during programming, and the IMU won't respond until it has been power cycled. If you notice your heartbeat has stopped, unplug and replug your USB power cord to power cycle the board.

Back to the [Schedule](https://github.com/ndm736/ME433_2019/wiki/Schedule)