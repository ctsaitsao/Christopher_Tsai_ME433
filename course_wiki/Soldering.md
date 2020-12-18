Soldering

Take a look at [these soldering gifs](http://hades.mech.northwestern.edu/index.php/EDI_Bootcamp).

Example code for testing each part of the board is posted in the tech_cup_code folder.

Make sure that the iron you are trying to use is nice and shiny, and that it is easy to add a small amount of solder to the tip. If not, use a different iron, and let me know which iron is not working well.

Solder your board in stages, testing after each stage. First, solder the USB connector, slide switch, the voltage regulator, all of the capacitors, and the power LED and resistor. Plug the board into USB and verify that the LED turns on and the 3.3V line stable.

Second, solder the PIC, oscillator, reset and user buttons and resistors, user LED and resistor, and Snap header pins. Try to program the PIC, and verify that you can blink the LED at a known rate to test the clock.

Next, solder female header pins and test the LCD.

Next, solder female header pins to the PIC board and male header pins to the camera board and test the camera.

Next, solder the 28 gauge stranded wire to the motor leads.

Finally, solder the male header pins to the h-bridge (THE CHIP SIDE GOES DOWN), female headers in the h-bridge adapter, power switch, male header pins, and power male header pins in the h-bridge adapter, and female header pins in the PIC board, and test powering the PIC with the battery, and test driving the motors.

Back to the [Schedule](https://github.com/ndm736/ME433_2019/wiki/Schedule)  