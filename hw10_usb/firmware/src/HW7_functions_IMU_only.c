#include <xc.h>           // processor SFR definitions
#include <stdio.h>
#include <math.h>
#include "I2C_master_no_int.h"
#include "HW7_functions_IMU_only.h"

void PIC32_Startup(void) {

    __builtin_disable_interrupts();

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // do your TRIS and LAT commands here
    TRISAbits.TRISA4 = 0;
    TRISBbits.TRISB4 = 1;
    LATAbits.LATA4 = 1;
    
    __builtin_enable_interrupts();

}

void initLSM6DS33() {
    I2C_master_start();
    I2C_master_send(0b11010110);  // writing to slave with address 1101011  
    I2C_master_send(0b00010000);  // sending to register of address 00010000 (CTRL1_XL)
    I2C_master_send(0b10000010);  // sample rate 1.66 kHz (first 1000), 2g sensitivity (next 00), 100 Hz anti-aliasing \
                                        filter bandwidth (final 10)
    I2C_master_restart();
    I2C_master_send(0b11010110);  // writing to slave with address 1101011      
    I2C_master_send(0b00010001);  // sending to register of address 00010001 (CTRL2_G)
    I2C_master_send(0b10001000);  // sample rate 1.66 kHz (first 1000), 1000 dps sensitivity (next 10)
    I2C_master_restart();
    I2C_master_send(0b11010110);  // writing to slave with address 1101011      
    I2C_master_send(0b00010010);  // sending to register of address 00010001 (CTRL3_C)
    I2C_master_send(0b00000100);  // IF_INC bit enabled (enable the ability to read multiple registers in a row \
                                        without specifying every register location)
    I2C_master_stop();
}
 
void I2C_read_multiple(unsigned char address, unsigned char register_, unsigned char * data, int length) {  
    
    //"register" = first register where data acquisition starts
    // sequential read is turned on, so if we tell the chip to read multiple times it goes to the next register
    
    I2C_master_start();
    I2C_master_send(address << 1);  // writing to slave with address "address"
    I2C_master_send(register_);  // sending to register of address "register"
    I2C_master_restart();
    I2C_master_send((address << 1) | 0b00000001);  // reading from slave with address 1101011      
    
    int i = 0;
    for (i = 0; i < length; i++) {
        if (i < length - 1) {
            data[i] = I2C_master_recv();   
            I2C_master_ack(0);
        }
        else {
            data[i] = I2C_master_recv();   
            I2C_master_ack(1);           
        }
    }
    
    I2C_master_stop();    
}