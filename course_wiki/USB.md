The PIC32 contains a peripheral for USB OTG (Universal Serial Bus On The Go). Using this peripheral, the PIC can be a host and receive data from mice, keyboards, and thumb drives, or the PIC can be a device that provides data to a computer.  

When USB was first created, it replaced parallel and serial port communication, and quickly became the standard peripheral port for computers and devices. As data needs for external data drives and webcams increased, the USB protocol was amended from version 1.0, to 1.1, to 2.0, to 3.0 and 3.1, and now USB C. USB 3.0 and above are way too fast for the PIC, so we'll ignore them.  

USB 2.0 is divided into two speeds: High Speed (480 Mbit/s) and Full Speed (12 Mbit/s). Our PIC, running at 48 MHz, cannot operate in High Speed Mode, so we'll stick with Full Speed.  

USB communication requires four wires: 5V, GND, D+ and D-. As a Host, the PIC would provide 5V to the device through a type A port (the kind you have on your laptop). As a device, the PIC would receive 5V from the host through a type B port (the full size is a big square often found on printers, the mini is the type the NU32 and PICkit use, the micro is the type an Android phone, Snap, and nScope use). It is possible to use a type AB (most often in micro form) which is usually a device but can use an adapter (called a USB OTG cable) to convert to a full sized A and be a host.   

The USB data lines (D+ and D-) are digital at 0V and 3.3V. Specific errors can be determined when both lines are high or both are low, but it is often not helpful to look at the signal voltages with an oscilloscope. Most debugging is done with code.  

USB is different from UART, SPI, and I2C, in that the peripheral has the ability to directly read and write to RAM, without going through the CPU (this is called direct memory access, or DMA). This means that USB communication occurs automatically on the PIC after you have initialized the peripheral. The memory addresses that are accessed are called Endpoints. We will use the PIC as a device. In device mode, the computer, at a specific time interval, will send data to the PIC, which will automatically put that data into PIC RAM. In the PIC code, a bit will be set to tell you that there is new data in RAM, and you can read it at your leisure. When you want to send data to the computer, you put the data into the Endpoint RAM, and at a specific time interval the computer will request it (usually right after it has written its data) and a bit will be set to let you know your data has been read.  

The time interval is determined by the type of device you make the PIC, and how many other USB devices are plugged into the computer. The are 16 types of device that you can make the PIC, called classes. When you first plug a USB device into the host, a process called enumeration allows the device to tell the host what class of communication to use. The most common classes are HID, CDC, and MSD.  

HID (human interface device) is a class for small amounts of data sent often (64 bytes in, 64 bytes out, every 1 ms). HID is used for mice, keyboards, and other types of computer IO devices. There are standards for how to package data into the 64 bytes so that every computer knows how to receive mouse data, so HID is known as a driverless class (no custom driver needs to be installed on the host).  

CDC (communication class device) is a class for small-ish amounts of data sent often. CDC is used (among other reasons) to emulate the serial devices that USB replaced. When the computer enumerates a CDC device, it creates a virtual comm port to send and receive the data as ASCII characters. This can use the built in computer driver (usually linux and Mac) or install a custom driver (usually Windows).  

MSD (mass storage device) is a class for large amounts of data sent irregularly. MSD devices are external memory, like a hard drive or thumb drive. If you think of USB communication as PWM, HID and CDC are given fixed duty cycles worth of data, and MSD is given the rest if it needs it.  

For devices that do not fit into one of these classes, custom classes can be created. This can be difficult, because on top of writing all of the USB code for the PIC, you have to write the computer side (the driver). Applications usually try to take advantage of HID or CDC classes for general purpose communication.  

For instance, the nScope is an HID device that does not follow any of the predefined data standards for HID devices. The advantage is that the computer does not need to use a driver, and the communication does not need to be converted to ASCII (as it would for CDC). The disadvantage is that the maximum number of bytes per second is 64000 (64 bytes every ms) and this limits the streaming data capabilities of the oscilloscope.  

The PIC implements USB as an interrupt driven state machine, which is more work than we want to learn about at this stage. Harmony provides code to make HID and CDC devices, so we will use Harmony to generate our USB code.  


Back to the [Schedule](https://github.com/ndm736/ME433_2019/wiki/Schedule)  