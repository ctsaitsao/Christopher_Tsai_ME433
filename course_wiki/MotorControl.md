For the final project, it will be necessary to display data from a sensor, use the data to calculate a control effort for two motors, and set the speed and direction of the motors.

The data will be a row of pixels from a camera. Each pixel contains three unsigned chars that represent the brightness of the red, green, and blue that the camera picks up. There are a variety of data processing techniques to use on camera data, and visualizing them is important to be able to determine if they are working correctly.

Write a function that uses the high three bits of an array of 240 unsigned chars and plots them to your LCD. Call it three times, one for each array of pixel colors.

We will use an h-bridge breakout board from Pololu for the [DRV8835](https://www.pololu.com/product/2135) to drive two brushed DC motors. When the MODE pin is high, each motor is controlled by a direction pin (called PHASE) and a speed pin (called ENABLE). The speed pin is controlled with PWM from an output compare peripheral.

Setup a digital output pin and an OC pin. Connect both to LEDs, and make the frequency of the PWM 20kHz.

Every second, toggle the direction pin, and bring the PWM from 0% to 100%. Change the PWM 100 times per second. So far in class we have been using the core timer in an infinite loop to do delays, but let's review how to do it with an interrupt. 

An interrupt is a function that is called automatically by a peripheral. In code, we initialize the peripheral and assign it a reason to generate an interrupt. Then we enable the interrupt with a specific priority level, so that simultaneous interrupts have an order under which to operate. Then we write a function using a special macro (__ISR(...)), making sure to clear the interrupt flag inside of the function so that the interrupt can happen again.

Use Timer2 for PWM, and Timer3 for the 100Hz interrupt.

Output compare is initialized by setting up a timer with the base frequency, then initializing the duty cycle as a percentage of the PR value used by the timer:

`T2CONbits.TCKPS = 0;     // Timer2 prescaler N=1 (1:1)`

`PR2 = 2399;              // PR = PBCLK / N / desiredF - 1`

`TMR2 = 0;                // initial TMR2 count is 0`

`OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults`

`OC1RS = 0;             // duty cycle`

`OC1R = 0;              // initialize before turning OC1 on; afterward it is read-only`

`T2CONbits.ON = 1;        // turn on Timer2`

`OC1CONbits.ON = 1;       // turn on OC1`

Set the value of OC1RS any time you want to update the duty cycle.

The interrupt bits are listed in Table 7-1 of the [PIC32 datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/PIC32MX1XX2XX-28-36-44-PIN-DS60001168K.pdf). Initialize Timer3 for 100Hz, and use IEC0, IPC3, and IFS0 to initialize the interrupt. Write the ISR like:

`void __ISR(_TIMER_3_VECTOR, IPL5SOFT) Timer3ISR(void) { ` 

`  IFS0bits.T3IF = 0; `

  `// how many times has the interrupt occurred?`

  `// set the duty cycle and direction pin`

`}`

Back to the [Schedule](https://github.com/ndm736/ME433_2019/wiki/Schedule)  