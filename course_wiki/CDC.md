CDC on the PIC32 using Harmony

The first type of USB to get working on the PIC is CDC, so that we can print ASCII to the computer and debug like we did with the NU32.  

Make a new Harmony project for the PIC and change the device registers to match the pragma configurations we have been using. Note that the pin VBUSON is also SCK1 and B14, so USB will prevent your LCD from working unless you set FVBUSONIO to OFF. Before generating the code, open the Harmony Framework Configuration tree, and open the USB Library. Check the Use USB stack? box. Check the Use SOF Events box. Open the USB Device Instance 0 tree, and Function 1 tree. Change Device Class to CDC. Minimize Function 1, and change Product ID Selection to cdc_com_port_single_demo. Now generate the code.  

The project is set up to include all of the USB libraries, but it does not have the CDC state machine. I have edited app.c, app.h, and system_config.h to be able to send and receive ASCII data. Open those files and replace their content with the files in the repo usb/cdc folder. Compile and load the code, and check if you have a new com port on your computer (on Windows, in cmd.exe, type mode, or open the device manager. On Mac or linux, in the terminal, type ls /dev/tty.*). Open the port in a serial terminal , and a number will increment at 5 Hz, and any character you type will be echoed back (on Windows, open putty, change the mode to serial, type in the name of the COM, and hit open. On Mac / linux, from the terminal, type screen and the name of the port).  

Let's look at how the code works. APP_Tasks() in app.c contains the state machine that is called from the infinite loop in main(). After APP_STATE_INIT and APP_STATE_WAIT_FOR_CONFIGURATION, the state machine transitions through APP_STATE_SCHEDULE_READ, APP_STATE_WAIT_FOR_READ_COMPLETE, APP_STATE_CHECK_TIMER, APP_STATE_SCHEDULE_WRITE, and APP_STATE_WAIT_FOR_WRITE_COMPLETE.  

APP_STATE_SCHEDULE_READ and APP_STATE_WAIT_FOR_READ_COMPLETE check to see if any data was sent from the computer, using the variable appData.isReadComplete which is set by the USB interrupt. The data is saved in the character array (pointer) appData.readBuffer.  

APP_STATE_CHECK_TIMER checks to see if there is data to echo, or if a timer has reached 5 Hz, and lets the code move on to send data back to the computer.  

APP_STATE_SCHEDULE_WRITE and APP_STATE_WAIT_FOR_WRITE_COMPLETE send data back to the computer and reset the machine.  

Note that you have to write something when you hit the APP_STATE_SCHEDULE_WRITE state. If you don't have any data to send, set len = 1; dataOut[0] = 0; to send a blank packet.  

Back to the [Schedule](https://github.com/ndm736/ME433_2019/wiki/Schedule) 