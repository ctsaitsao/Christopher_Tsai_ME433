#include <stdio.h>
#include <math.h> 	            // for sine wave plotting
#include <xc.h>                 // for digital I/O
#include <math.h>
#include "ili9341.h"            // LCD built-in controller

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
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // do not enable clock switch
#pragma config WDTPS = PS1048576 // use slowest wdt
#pragma config WINDIS = OFF // wdt no window mode
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
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

void XPT2046_read(unsigned short *x, unsigned short *y, unsigned int *z) {   // XPT2046 = touch screen controller \
                                    *x, *y, and *z are used so that only one function is called to put data into 3 axes 

    CS2 = 0;
    unsigned char garbage3 = spi_io(0b10110001);
    unsigned char z1temp1 = spi_io(0);   // z1-position, high bits
    unsigned char z1temp2 = spi_io(0);   // z1-position, low bits
    CS2 = 1;
    unsigned short z1temp = ((z1temp1 << 8) | z1temp2) >> 3;
    
    CS2 = 0;
    unsigned char garbage4= spi_io(0b11000001);
    unsigned char z2temp1 = spi_io(0);   // z2-position, high bits
    unsigned char z2temp2 = spi_io(0);   // z2-position, low bits
    CS2 = 1;
    unsigned short z2temp = ((z2temp1 << 8) | z2temp2) >> 3;
    
    CS2 = 0;
    unsigned char garbage2 = spi_io(0b10010001);
    unsigned char ytemp1 = spi_io(0);   // y-position, high bits
    unsigned char ytemp2 = spi_io(0);   // y-position, low bits
    CS2 = 1;
    unsigned short ytemp = ((ytemp1 << 8) | ytemp2) >> 3;
    
    CS2 = 0;
    unsigned char garbage1 = spi_io(0b11010001);
    unsigned char xtemp1 = spi_io(0);   // x-position, high bits
    unsigned char xtemp2 = spi_io(0);   // x-position, low bits
    CS2 = 1;
    unsigned short xtemp = ((xtemp1 << 8) | xtemp2) >> 3;
    
    int ztemp = z1temp - z2temp + 4095;
    
    *x = xtemp; 
    *y = ytemp; 
    *z = ztemp;
}

int main() {
    PIC32_Startup();
    SPI1_init();
    LCD_init();
    LCD_clearScreen(ILI9341_BLACK);  // clear to background color
    unsigned short x, y, x_scaled, y_scaled; 
    unsigned int z; 
    char FPS_counter[10];
    
    int i, j;
    for (i = 160; i < 210; i++) {
        for (j = 100; j < 140; j++) {
            LCD_drawPixel(i, j, ILI9341_WHITE);
        }
    }        
    int i2, j2;
    for (i2 = 160; i2 < 210; i2++) {
        for (j2 = 200; j2 < 240; j2++) {
            LCD_drawPixel(i2, j2, ILI9341_WHITE);
        }
    }
    char I_plus[10], I_minus[10];
    sprintf(I_plus, "I++");
    sprintf(I_minus, "I--");
    LCD_drawString(I_plus, 175, 115, ILI9341_BLACK, ILI9341_WHITE);
    LCD_drawString(I_minus, 175, 215, ILI9341_BLACK, ILI9341_WHITE);   
    
    int I = 0;
        
    while(1) {  
        _CP0_SET_COUNT(0);
        
        XPT2046_read(&x, &y, &z);  
        
        char x_s[50], y_s[50], z_s[50];
        sprintf(x_s, "x raw: %d             ", x);
        sprintf(y_s, "y raw: %d             ", y);
        sprintf(z_s, "z raw: %d             ", z);
        LCD_drawString(x_s, 20, 120, ILI9341_WHITE, ILI9341_BLACK);
        LCD_drawString(y_s, 20, 140, ILI9341_WHITE, ILI9341_BLACK);
        LCD_drawString(z_s, 20, 160, ILI9341_WHITE, ILI9341_BLACK);
        
        if (z > 50) {
            x_scaled = x * 0.0586;                 // (240/4095) doesn't work bc it rounds the int to 0
            y_scaled = 320 - (y * 0.0781);         // (320/4095) doesn't work bc it rounds the int to 0, do \
                                                       320 minus to so origin is in top left corner
        }
        else {
            x_scaled = 0;
            y_scaled = 0;
        }
        
        char x_scaled_s[50], y_scaled_s[50];
        sprintf(x_scaled_s, "x scaled: %d             ", x_scaled);
        sprintf(y_scaled_s, "y scaled: %d             ", y_scaled);
        LCD_drawString(x_scaled_s, 20, 180, ILI9341_WHITE, ILI9341_BLACK);
        LCD_drawString(y_scaled_s, 20, 200, ILI9341_WHITE, ILI9341_BLACK);
                
        if (z > 1000 && x_scaled > 160 && x_scaled < 220 && y_scaled > 100 && y_scaled < 140) {
            I++;
        }
        
        if (z > 1000 && x_scaled > 160 && x_scaled < 220 && y_scaled > 200 && y_scaled < 240) {
            I--;
        }
           
        char I_s[15];
        sprintf(I_s, "I = %d ", I);
        LCD_drawString(I_s, 170, 165, ILI9341_WHITE, ILI9341_BLACK);
        
        float FPS = 24000000/_CP0_GET_COUNT();  // (counts/second)/counts, 1 frame = amount of time it requires to run the while loop until this point
        sprintf(FPS_counter, "FPS = %5.2f    ", FPS);
        LCD_drawString(FPS_counter, 20, 40, ILI9341_WHITE, ILI9341_BLACK);
    }
    return 0;
}