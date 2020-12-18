Follow along with [Making a PIC32 circuit from scratch](https://github.com/ndm736/ME433_2019/wiki/PIC32MX250) to build and test your breadboarded PIC32 circuit. Make a folder in your repo, called HW1, and add all of the **bold deliverables** to it.

1. On paper, draw your circuit diagram. Label the names and the numbers of the pins. When you are done, **add it to your repo**. The circuit contains:
    * The [Pololu USB micro-B](https://www.pololu.com/product/2586) breakout board for power and USB connections.
    * The [L4931](https://www.st.com/resource/en/datasheet/l4931.pdf) 3.3V regulator in a TO-92 package. It needs 0.1uF capacitors between the input and ground and output and ground, and also throw in a 10uF tantalum capacitor from output to ground. Put a red LED with a 330 ohm series resistor from 3.3V to GND to indicate when power is on.
    * The [MPLAB Snap](http://ww1.microchip.com/downloads/en/DeviceDoc/MPLAB%20Snap%20In-Circuit%20Debugger%20IS%20DS50002787A.pdf) programmer (pinout shown on page 5). By default the Snap does not provide 3.3V, it reads the 3.3V to verify that the PIC32 is powered and ready to program. You will use extra long header pins to plug the Snap into your breadboard. Note we do not use pins 6-8.
    * The [PIC32MX250F128B](https://github.com/ndm736/ME433_2019/blob/master/PIC32MX250F128B_pinout.pdf) in a 28 pin SDIP package (note the divot at the top of the chip, indicating pin 1). The PIC32 needs many connections to 3.3V and GND, 0.1uF capacitors from those 3.3Vs to GND, a 10uF tantalum capacitor from VCAP to GND, an LED with a series 330 ohm resistor on A4 for debugging, a pushbutton with a series 10k ohm resistor on B4 for debugging, a pushbutton with a 10k ohm pullup resistor on MCLR, connections to the Snap, connections to the USB micro-B breakout, and connections to the crystal resonator.
    * The [8MHz crystal resonator](http://www.digikey.com/product-search/en?keywords=490-1212-ND) connected to the OSC pins on the PIC32.

2. Build the voltage regulator circuit. Be careful here, the 5V connection is coming directly from your computer. ***Shorting it would short your computer! This is not catastrophic, but there is no reason tempt fate.***
    * First, leave all of the capacitors out of the circuit (apply 5V to Vin, GND to ground, and power the red LED from Vout). Put GND and 3.3V on both rails of your breadboard. Measure 3.3V with your oscilloscope, you should see some oscillations. **Take a screenshot or photo of the oscillations and add it to your repo**.
    * Next, add the input 0.1uF capacitor, then the output 0.1uF capacitor, then the 10uF tantalum output capacitor. At what point do the oscillations go away? **Take a screenshot of the clean 3.3V voltage.**
    * The 3.3V rail should be flat at all frequencies, and the regulator should not be hot.

3. With the power off, add the PIC32 to the breadboard, with all of the connections to 3.3V and GND, and all of the bypass 0.1uF capacitors and the VCAP 10uF tantalum capacitor. Make it nice and neat, we will be using this breadboard for the next 5 weeks. Connect the USB power source, and scope the 3.3V rail. It should be that same as before, and neither the PIC32 or voltage regulator should get hot.

4. Add the connections to the crystal resonator, keeping the wires as short as possible.

5. Add the connections to the Snap, the reset pushbutton, the green user LED, the user pushbutton, and the USB communication and power pins. Verify that the 3.3V rail is still clean.

6. **Take a photo of your breadboard and add it to your repo**. It should look like:
![](https://github.com/ndm736/ME433_2019/tree/master/images/pic32mx250f128b_hello.jpg)

6. Time to program! Make a new project, called HW1, in your repo. The goal of this code is to get your PIC32 to "Hello World!" We don't yet have the ability to communicate with your computer, so your "Hello World!" will be a blinking LED, at 1kHz, that stops when you push and hold the user pushbutton, and continues when you release the pushbutton.

    * First, you must fill out all of the `#PRAGMA` values. Use the template, which has hints [here](https://github.com/ndm736/ME433_2019/blob/master/code_template.txt).
    * Along with all of the pragmas, you should have some includes, an `int main()` with speed initializations, and an infinite loop. Compile and upload the code with the programmer. The PIC32 won't do anything, but if there are no compile errors and no programmer errors, you are on the right track.
    * Add initializations in main() to make the pushbutton pin an input pin, and the LED pin as an output that is initially high. Upload the code and verify that the LED turns on. (This is a great time to **commit your code on Git!**)
    * In the infinite loop, set the Core Timer to 0, turn on the LED, then wait for 0.5ms to pass, then turn off the LED, then wait another 0.5ms. Remember that the Core Timer always counts at half of the system clock. Upload the code and verify that the LED is half as bright as it was before, and scope the pin to check the timing. If the timing is off, double check your `#PRAGMA` values.
    * Add code that reads the pushbutton, and if it is pushed, wait for it to be unpushed before continuing. **Commit your final code.**

7. **Make a video of your 1kHz square wave and demonstrate the pushbutton. Upload the video to CANVAS**.

Back to the [Schedule](https://github.com/ndm736/ME433_2019/wiki/Schedule)