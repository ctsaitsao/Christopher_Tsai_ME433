#include <xc.h>           // processor SFR definitions
#include <stdio.h>
#include "ili9341.h"

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

int main(void) {
    PIC32_Startup();
    SPI1_init();
    LCD_init();
    LCD_clearScreen(ILI9341_PINK);  // clear to background color
    char helloWorld[15] = {' ','H','e','l','l','o',' ','W','o','r','l','d','!',' '};  // 15 b/c of secret '/0' after '!'
    LCD_drawString(helloWorld, 28, 32, ILI9341_WHITE, ILI9341_BLACK);
    char progressBar[30] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};  //29 characters limit before progress bar increases forever
    LCD_drawString(progressBar, 40, 120, ILI9341_WHITE, ILI9341_WHITE);
    int i = 0, counter[10], progressBar2[30], FPS_counter[10];  // m = message
    while(1) {
        _CP0_SET_COUNT(0);
        sprintf(counter, " Progress = %d  ", i);
        LCD_drawString(counter, 40, 80, ILI9341_WHITE, ILI9341_BLACK);
        int j, k , xPos, yPos;
        for (j = 0; j < 100; j++) {
            xPos = 40 + j;
            if(j <= i) {
                for (k = 0; k < 8; k++) {  // 8 = height
                    yPos = 120 + k;
                    LCD_drawPixel(xPos, yPos, ILI9341_GREEN);
                }
            }
        }
        i++;
        if (i>100){
            i = 0;
            LCD_drawString(progressBar, 40, 120, ILI9341_WHITE, ILI9341_WHITE);
        }
        int FPS = 24000000/_CP0_GET_COUNT();  // (counts/second)/counts, 1 frame = amount of time it requires to run the while loop until this point
        sprintf(FPS_counter, " FPS = %d  ", FPS);
        LCD_drawString(FPS_counter, 40, 160, ILI9341_WHITE, ILI9341_BLACK);
        int time;
        time = _CP0_GET_COUNT();
        while (_CP0_GET_COUNT() < time + 2400000) {;} // 100 ms, 10 Hz
    }
    return 0;
}

void LCD_drawLetter(char letter, int x, int y, int c1, int c2) {  // c1 = font color, c2 = background (like highlight) color
    int i = 0;
    for (i=0; i<5; i++) {
        char col = ASCII[letter - 0x20][i]; // 8-bit number stating what pixels of column are on. The characters in ASCII start at 0x20 in the ASCII table, so if you know the character you want to display, its location in ASCII is its decimal value minus 0x20.
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
//
//void LCD_dispBar(unsigned short x, unsigned short y, unsigned short length, unsigned short height, unsigned short c1, unsigned short c2, int count, int maxCount) {
//    int i, j, xPos, yPos;
//    if(length >= 0 && height >= 0) {
//        for(i = 0; i <= length; i++) {
//            xPos = x + i;
//            int gap = count*length/maxCount;
//            if(i <= gap) {
//                
//            }
//        }
//    }
//}