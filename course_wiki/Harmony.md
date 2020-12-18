MPLAB Harmony is a software framework for PIC32 projects. At the lowest level, you can think of it as a library of functions for PIC peripherals. At the highest level, Harmony is a wizard that will create a code template for you, initializing peripherals using a GUI.  

We chose not to teach Harmony up until now because the library functions hide the way the SFRs work. Take a look at the function Harmony provides for setting the TRIS bits, in your Harmony directory (something like  ... \harmony\v1_10\framework\peripheral\ports\templates\ports_PortsDirection_MCU32_PPS.h). To create an output pin, you would use the the function `void PORTS_PinDirectionOutputSet_MCU32_PPS()`. It is clear what the function does from the name, but it is a lot of typing for what could really just be `TRISxbits.TRISx# =  ...`. Also, what occurs when you use that function? ` _SFR_BIT_CLEAR( _PORTS_DIRECTION_B_VREG(index) + ((channel-1) * 0x40), bitPos );`, which means other functions are called, and you have to dig really far down in the library to eventually find the 'TRIS' command.  

So for something simple like using pins, timers, and PWM, it makes sense to stick with the SFRs. But for more complicated tasks, like CAN or USB communication, the library is your best option.  

Another advantage of Harmony is that the chip specific commands are hidden, so it is relatively easy to take code written for one PIC (like the PIC32MX250F128B) and put it on the NU32 (the PIC32XM795F512H). That task can be hard when you've specifically written code using SFRs.  

If you haven't yet, download and install [Harmony](https://www.microchip.com/mplab/mplab-harmony) (v2.06). After it installs, open the IDE, and go to Tools -> Plugins. Change the tab to Available Plugins, and check the box for the MPLAB Harmony Configurator. Click Install and follow the prompts. Now you have access to the Harmony libraries and the wizard.  

The Harmony folder contains help files in a few formats. The html version is located in (harmony/v2_06/doc/html/frames.html?frmname=topic&frmfile=index.html). Volume I will give you more information about how Harmony is intended to be used if you are interested in reading more.  

Our first goal is to get our Blink code from HW1 working again, but this time using the MPLAB Harmony Configurator to setup the files.  

Make a New Project, and note that the default selections are now Microchip and 32-bit MPLAB Harmony Project. Leave those defaults and click Next>. Select your Harmony path, and set the project location to your git folder. Give the project a name (like HW8) and select the PIC32MX250F128B as the Target Device. Click Finish.  

MPLAB will open up the MPLAB Harmony Configurator tab where your code usually appears. With this wizard, you will select your pragma values, and MPLAB will generate the main.c file (and many others).  

Open the Device & Project Configuration tree, and open the PIC32MX250F128B Device Configuration tree. Here you can edit the DEVCFG registers set by the pragma values. Open each DEVCFG tree and set the values to match your earlier homeworks. (You can also try the Clock Diagram tab and use the GUI to set the values, but it isn't easy to do right).  

Click the Generate Code button (looks like some gears, a green arrow, and text that says Code). Click Generate at the prompt, and close the Configurator tab.  

Your project has now been filled with many folders, sub folders, and files. In the project tree, open Source Files and app. Open main.c and examine the structure. Harmony has placed two functions in main: SYS_initialize() and SYS_tasks(). In a Harmony project, you do not edit main.c, the code in there is complete already. Instead, you edit those two functions, which are located in app.c. Close main.c and open app.c.  

Inside app.c is a global variable `APP_DATA appData`, which is a type defined in app.h Open app.h in the Header Files and app tree. The type `APP_DATA` is made of another variable of type `APP_STATES`, of which there are two states: `APP_STATE_INIT` and `APP_STATE_SERVICE_TASKS`.  

Yikes. So main calls SYS_initialize, which uses Harmony library functions to do what we always do in our code (set the PIC to run as fast as possible and free up pins). The last thing it does is call APP_Initialize, which makes appData equal to APP_STATE_INIT. Then the infinite while loop begins in main, and APP_Tasks gets called over and over. The first time through, the appData case is APP_STATE_INIT, which changes appData to APP_STATE_SERVICE_TASKS, which doesn't do anything.  

This is the format of a Harmony project template. If you compiled and loaded this code onto your PIC, the PIC would run at 48MHz but not do anything.  

Let's redo what we did in HW1 (open up your HW1 main file in a text editor so you can copy the important bits of the code out of it). We only need to change app.c. In the APP_Initialize function, place your pin initializations. In APP_Tasks, in the APP_STATE_SERVICE_TASKS case, add your blink / delay / read button code. Compile the code and load it onto the PIC. Verify that the LED blinks at the frequency you programmed to check that the sysclk has been set to 48MHz.  

Technically you should use the Harmony functions to initialize and read your pins, but we will mix and match the code that we know (SFR manipulations) with Harmony code, and stick to app.c instead of editing SYS_PORTS_Initialize, etc.  

Try adding your LCD and IMU code and make sure it functions as it did in HW7. You will have to copy your LCD and I2C libraries into the project folder, then add the .c files to the Source Files > app tree and .h files to the Header Files > app tree.  

Back to the [Schedule](https://github.com/ndm736/ME433_2019/wiki/Schedule) 