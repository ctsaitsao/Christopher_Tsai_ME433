#include <xc.h>           // processor SFR definitions
#include <stdio.h>
#include <math.h>
#include "ili9341.h"
#include "I2C_master_no_int.h"

// DEVCFG0
#pragma config DEBUG = OFF // no debugging
#pragma config JTAGEN = OFF // no jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // no write protect
#pragma config BWP = OFF // no boot write protect
#pragma config CP = OFF // no code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // turn off secondary oscillator
#pragma config IESO = OFF // no switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // disable secondary osc (CLKO Output Signal Active on the OSCO Pin)
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock; PBCLK = SYSCLK
#pragma config FCKSM = CSDCMD // do not enable clock switch
#pragma config WDTPS = PS1048576 // use slowest wdt
#pragma config WINDIS = OFF // wdt no window mode
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal; SYSCLK = 48 MHz
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz
#pragma config UPLLIDIV = DIV_2 // divider for the 8MHz input clock, then multiplied by 12 to get 48MHz for USB
#pragma config UPLLEN = ON // USB clock on

// DEVCFG3
#pragma config USERID = 0 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations
#pragma config FUSBIDIO = ON // USB pins controlled by USB module
#pragma config FVBUSONIO = ON // USB BUSON controlled by USB module

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

int main() {
    PIC32_Startup();
    ANSELBbits.ANSB2 = 0;
    ANSELBbits.ANSB3 = 0;  
    I2C_master_setup();  // chooses baud rate and turns I2C2 on
    initLSM6DS33();  // turns on gyroscope and accelerometer, among other things
    SPI1_init();
    LCD_init();
    LCD_clearScreen(ILI9341_BLUE);  // clear to background color
    LCD_write_WHO_AM_I();
        
    while(1) {  
        
        // Horizontal Bar:   
        int i2, j2;
        for (i2 = 0; i2 < 240; i2++) {
            for (j2 = 80 + 116; j2 < 80 + 124; j2++) {  // 8 = height
                LCD_drawPixel(i2, j2, ILI9341_WHITE);
            }
        }

        // Vertical Bar:
        int i3, j3;
        for (i3 = 116; i3 < 124; i3++) {
            for (j3 = 80; j3 < 320; j3++) {  // 8 = height
                LCD_drawPixel(i3, j3, ILI9341_WHITE);
            }
        }
        
        
        // reading:
        unsigned char register_data[14];
        I2C_read_multiple(0b1101011, 0b00100000, register_data, 14); // register address 00100000 = OUT_TEMP_L
        
//        char hello[10];
//        sprintf(hello, "hello");
//        LCD_drawString(hello, 20, 60, ILI9341_WHITE, ILI9341_BLACK);
        
//        char unappended[25];
//        sprintf(unappended, " Temp: %d  ", register_data[0]);   // showed a value of 140
//        LCD_drawString(unappended, 20, 40, ILI9341_WHITE, ILI9341_BLACK);
               
        //appending:
        signed short register_data_appended[7];
        int i = 0;
        for (i = 0; i < 7; i++) {
            register_data_appended[i] = ((register_data[i*2+1] << 8) | register_data[(i*2)]);   // low bits come fist
        }
        
//        signed short temp = register_data_appended[0];
//        signed short gyroX = register_data_appended[1];
//        signed short gyroY = register_data_appended[2];
//        signed short gyroZ = register_data_appended[3];
        signed short accelX = register_data_appended[4];
        signed short accelY = register_data_appended[5];
//        signed short accelZ = register_data_appended[6];
        
        
        // printing non-scaled values:
        char temp_s[15], gyroX_s[15], gyroY_s[15], gyroZ_s[15], accelX_s[15], accelY_s[15], accelZ_s[15];
        
//        sprintf(temp_s, " Temp: %d  ", temp);
//        sprintf(gyroX_s, " gyroX: %d  ", gyroX);
//        sprintf(gyroY_s, " gyroY: %d  ", gyroY);
//        sprintf(gyroZ_s, " gyroZ: %d  ", gyroZ);
        sprintf(accelX_s, " accelX: %d  ", accelX);
        sprintf(accelY_s, " accelY: %d  ", accelY);
//        sprintf(accelZ_s, " accelZ: %d  ", accelZ);

//        LCD_drawString(temp_s, 100, 40, ILI9341_WHITE, ILI9341_BLACK);
//        LCD_drawString(gyroX_s, 100, 60, ILI9341_WHITE, ILI9341_BLACK);
//        LCD_drawString(gyroY_s, 100, 80, ILI9341_WHITE, ILI9341_BLACK);
//        LCD_drawString(gyroZ_s, 100, 100, ILI9341_WHITE, ILI9341_BLACK);
        LCD_drawString(accelX_s, 20, 40, ILI9341_WHITE, ILI9341_BLACK);
        LCD_drawString(accelY_s, 20, 60, ILI9341_WHITE, ILI9341_BLACK);
//        LCD_drawString(accelZ_s, 20, 80, ILI9341_WHITE, ILI9341_BLACK);
        
        // accelX Bar:
        int i4, j4;
        float accelX_scaled = accelX * 0.0000305 * 120;   // 2^16/2 = 32768, 0.0000305 = 1/32768
        if (accelX_scaled > 0) {
            for (i4 = 120; i4 < 120 + ((signed int) accelX_scaled); i4++) {  
                for (j4 = 80 + 116; j4 < 80 + 124; j4++) {  // 8 = height
                    LCD_drawPixel(i4, j4, ILI9341_GREEN);
                }
            }
        }
        if (accelX_scaled < 0) {
            for (i4 = 120; i4 > (120 + ((signed int) accelX_scaled)); i4--) {  
                for (j4 = 80 + 116; j4 < 80 + 124; j4++) {  // 8 = height
                    LCD_drawPixel(i4, j4, ILI9341_GREEN);
                }
            }            
        }
        
        // accelY Bar:
        int i5, j5;
        float accelY_scaled = accelY * 0.0000305 * 120;   // 2^16/2 = 32768, 0.0000305 = 1/32768
        if (accelY_scaled > 0) {
            for (i5 = 116; i5 < 124; i5++) {  
                for (j5 = 80 + 120; j5 > 80 + 120 - ((signed int) accelY_scaled); j5--) {  // 8 = height
                    LCD_drawPixel(i5, j5, ILI9341_GREEN);
                }
            }
        }
        if (accelY_scaled < 0) {
            for (i5 = 116; i5 < 124; i5++) {  
                for (j5 = 80 + 120; j5 < 80 + 120 - ((signed int) accelY_scaled); j5++) {  // 8 = height
                    LCD_drawPixel(i5, j5, ILI9341_GREEN);
                }
            }            
        }
        
        // printing scaled values:
        char accelX_scaled_s[30], accelY_scaled_s[30];
        sprintf(accelX_scaled_s, " accelX scaled: %6.2f  ", accelX_scaled);
        sprintf(accelY_scaled_s, " accelY scaled: %6.2f  ", accelY_scaled);
        LCD_drawString(accelX_scaled_s, 120, 40, ILI9341_WHITE, ILI9341_BLACK);
        LCD_drawString(accelY_scaled_s, 120, 60, ILI9341_WHITE, ILI9341_BLACK);
        
                
        // Making the PIC blink (heartbeat):
        _CP0_SET_COUNT(0);
        LATAbits.LATA4 = 1;    
        while(_CP0_GET_COUNT() < 12000000) { ; } // 500 ms  for 20Hz
        _CP0_SET_COUNT(0);
        LATAbits.LATA4 = 0;
        while(_CP0_GET_COUNT() < 12000000) { ; } // 500 ms
    }
    return 0;
}