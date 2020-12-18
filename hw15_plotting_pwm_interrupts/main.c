#include <stdio.h>
#include <math.h> 	            // for sine wave plotting
#include <xc.h>                 // for digital I/O
#include <math.h>
#include "ili9341.h"            // LCD built-in controller
#include <limits.h>             // for INT_MIN  
#include <sys/attribs.h>        // __ISR macro
#include <stdlib.h> 

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
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock (PBCLK)
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
    
    // user button
    TRISBbits.TRISB4 = 1;
    
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

void plot_three_highest(unsigned char *color_array, int x, int y, int c1, int c2) {    

                                                        // 3 color arrays for the 3 colors of a pixel
    
    int i, highest, second, third; 
    highest = second = third = INT_MIN; 
    
    for (i = 0; i < 240 ; i++) { 
        if (color_array[i] > highest) { 
            third = second; 
            second = highest; 
            highest = color_array[i]; 
        } 
        else if (color_array[i] > second) { 
            third = second; 
            second = color_array[i]; 
        }
        else if (color_array[i] > third) {
            third = color_array[i]; 
        }
    }
    
    char highest_s[30], second_s[30], third_s[30];
    sprintf(highest_s, "Highest bit: %d", highest);
    sprintf(second_s, "Second bit: %d", second);
    sprintf(third_s, "Third bit: %d", third);
    
    LCD_drawString(highest_s, x, y, c1, c2);
    LCD_drawString(second_s, x, y + 10, c1, c2);
    LCD_drawString(third_s, x, y + 20, c1, c2);
}

void __ISR(_TIMER_3_VECTOR, IPL5SOFT) Timer3ISR(void) {    // this interrupt is called every time TMR 3 counts
    
    IFS0bits.T3IF = 0;             // clear flag  
    if (LATAbits.LATA4 == 0) {
        OC3RS = OC3RS + 1;
    }
    else if (LATAbits.LATA4 == 1) {
        OC3RS = OC3RS - 1;
    }
}

int main() {
    
    PIC32_Startup();
    SPI1_init();
    LCD_init();
    LCD_clearScreen(ILI9341_BLUE);  // clear to background color    
    TRISAbits.TRISA4 = 0;   // pin 12 (A4) is the direction pin, which is an output
    RPB9Rbits.RPB9R = 0b0101;   // pin 18 (RB9) is OC3
    
    T2CONbits.TCKPS = 0; // Timer2 prescaler N=1 (1:1)
    PR2 = 2399; // PR = max value of count = (PBCLK / N / desiredF) - 1 = 48,000,000/1/(20000) - 1
    TMR2 = 0; // initial TMR2 count is 0
    OC3CONbits.OCM = 0b110; // PWM mode without fault pin; other OC3CON bits are defaults
    OC3CONbits.OCTSEL = 0;   //0 = use timer 2 for comparison
    OC3RS = 0; // duty cycle
    OC3R = 0; // initialize before turning OC3 on; afterward it is read-only
    T2CONbits.ON = 1; // turn on Timer2
    OC3CONbits.ON = 1; // turn on OC3
    
    __builtin_disable_interrupts();
    IPC3bits.T3IP = 5;   // priority level
    PR3 = 479999;  // PR = (PBCLK / N / desiredF) - 1 = 48,000,000/1/(100) - 1
    TMR3 = 0;
    T3CONbits.ON = 1;     // turn on Timer3
    IFS0bits.T3IF = 0;    // clear flag
    IEC0bits.T3IE = 1;    // enable TMR3 interrupt
    __builtin_enable_interrupts();
    
    int l = 0;
    
    unsigned char color[240];
    
    while(1) {
        
        for (l = 0; l < 240; l++) {
            color[l] = rand();
        }
        
        plot_three_highest(color, 20, 20, ILI9341_WHITE, ILI9341_BLUE);
        
        _CP0_SET_COUNT(0);
        LATAbits.LATA4 = 0;
        while (_CP0_GET_COUNT() < 24000000) {;}   // 1 second
        _CP0_SET_COUNT(0);
        LATAbits.LATA4 = 1;
        while (_CP0_GET_COUNT() < 24000000) {;}   // 1 second
    }
    
    return 0;
}