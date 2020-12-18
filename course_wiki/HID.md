HID on the PIC32 using Harmony  

The HID, or human interface device, class of USB devices is used for low data bandwidth, low latency devices, like computer mice and keyboards. Every 1 ms, the host sends 64 bytes to the device, and reads 64 bytes from the device. If you enumerate as a specific subclass, like a mouse or keyboard, and format your data within the 64 bytes in a specific order, you can control the mouse cursor position or send ASCII characters to a text input as if typing on a keyboard.  

If you want to send unformatted data with general messages like we did with CDC, you need to write a computer program to open the communication and interpret the bytes. You can use the [HID API](https://github.com/signal11/hidapi) to write a program in c with gcc or g++ to process the data.  

We will use the HID class to turn the PIC32 into a mouse. Make a new Harmony project, and set the device configuration registers like you did for CDC. Open the Harmony Framework Configuration tree, and open the USB Library. Check the Use USB stack? box. Check the Use SOF Events box. Open the USB Device Instance 0 tree, and Function 1 tree. Change Device Class to HID. Minimize Function 1, and change Product ID Selection to hid_mouse_demo. Now generate the code.  

Replace app.c, app.h, and config.h with the code from the usb/hid folder. Also, copy mouse.c and mouse.h into your firmware/src folder, then add them to the Header Files/app and Source Files/app trees in the IDE project.  

A generic HID mouse sends updated mouse cursor data to the host 250 times per second. The data is formatted as a Mouse Report, which is defined in mouse.h. A mouse report is 3 signed chars: a relative change in x position, a relative change in y position, and the state of the buttons encoded in the bits of the final char.  

The mouse report is generated in the case APP_STATE_MOUSE_EMULATE. This case occurs at the HID update rate of 1kHz. The line `if (!appData.isMouseReportSendBusy) {` takes care of assembling the 64 byte packet with the mouse report in the correct set of bytes, and deals with sending only every 4th loop (to meet the 250 Hz requirement) and copying the data to the right buffer only when the report has changed from the previous report.  

This means that our code that controls the mouse cursor occurs right before the `if (!appData.isMouseReportSendBusy) {`. In this sample code, the mouseButton array representing the left and right mouse button are never changed from the MOUSE_BUTTON_STATE_RELEASED state, so no button presses are simulated. The xCoordinate and yCoordinate values are selected from an array `dir_table`, where the yCoordinate is always 2 positions ahead of xCoordinate. How will this make the mouse cursor act? Load the code onto your PIC and find out!  

This is a mean trick to play on your friends. It also makes it tricky to work on your own computer. Remember you can hold your PIC reset button, or unplug the PIC power, to stop the cursor from moving, but the PIC must have power applied while programming.  

Now it is time to get the cursor under more understandable control. Remove the variables `static int8_t vector`, `static uint8_t movement_length`, and `int8_t dir_table[]`, they are unnecessary. In APP_STATE_MOUSE_EMULATE remove the if statement `if (movement_length > 50) {`, remove the line `vector++`, and replace the xCoordinate and yCoordinate assignments with the value 1 (they should look like `appData.xCoordinate = (int8_t) 1;`). Remove the `movement_length++` towards the bottom of the code, and run the code. The cursor should move diagonally towards the bottom right of your screen, at a medium velocity, but will vary computer-to-computer, depending on operating system settings.  

Change the x and yCoordinate assignments to 2 and see the effect. Can you fight to get your cursor back to the middle of your screen with your real mouse?  

So note, for controllability, that even tiny changes of the x and y positions add up fast at 250 Hz. To get the cursor to move slower, implement a variable like `static uint8_t inc = 0;`, and wrap your mouse report assignment in an if statement so that if `inc` is some number, x and yCoordinate are 1, otherwise they are 0. The larger that number is, the slower the mouse will move.  

Now you have the ability to control the mouse cursor speed by setting when inc allows the xCoordinate and yCoordinate to be non-zero, and what that non-zero value should be, according to some sensor reading.  


Back to the [Schedule](https://github.com/ndm736/ME433_2019/wiki/Schedule)  