#include <xc.h>           // processor SFR definitions
#include <stdio.h>
#include <math.h>
#include "ili9341.h"
#include "I2C_master_no_int.h"
#include "HW7_functions.h"

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

void LCD_drawLetter(char letter, int x, int y, int c1, int c2) {  // c1 = font color, c2 = background (like highlight) \
                                                                                                            color
    int i = 0;
    for (i=0; i<5; i++) {
        char col = ASCII[letter - 0x20][i]; // 8-bit number stating what pixels of column are on. The characters in \
                                               ASCII start at 0x20 in the ASCII table, so if you know the character \
                                            you want to display, its location in ASCII is its decimal value minus 0x20.
        int j = 0;
        for (j=7; j>=0; j--) {
            char bit_ = ((col >> j)&1); // or (col << 1) >> 7 
            if (bit_ == 1) {
                LCD_drawPixel(x+i,y+j,c1);
            }
            else {
                LCD_drawPixel(x+i,y+j,c2);
            }
        }
    }
}

void LCD_drawString(char *m, int x, int y, int c1, int c2) {
    int i = 0;
    while (m[i]) {  // when you loop through message, you know to stop when you get to a '/0' ('/0' is a char number 0)
        LCD_drawLetter(m[i], x + (i*5), y, c1, c2);
        i++;
    }
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

void LCD_write_WHO_AM_I() {
    I2C_master_start();
    I2C_master_send(0b11010110);  // writing to slave with address 1101011  
    I2C_master_send(0b00001111);  // sending to register of address 00001111 (WHO_AM_I)
    I2C_master_restart();
    I2C_master_send(0b11010111);  // reading from slave with address 1101011  
    int who_am_i = I2C_master_recv();
    I2C_master_ack(1);
    I2C_master_stop();
    char i_am[100];
    sprintf(i_am, " Who am I? I am %d (correct)  ", who_am_i);
    LCD_drawString(i_am, 20, 20, ILI9341_WHITE, ILI9341_BLACK);
}