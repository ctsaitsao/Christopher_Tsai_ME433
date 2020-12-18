#include <xc.h>           // processor SFR definitions
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

void initExpander() {
    I2C_master_start();
    I2C_master_send(0b01001110);  // sending to slave with address 01001110 (0100 is set already and 111 is user-defined) 
    I2C_master_send(0x00);  // sending to register 0 (IODIR)
    I2C_master_send(0b11110000);  // pins 0-3 are outputs and 4-7 are inputs
    I2C_master_stop();
}

void setExpander(char pin, char level) {  // writes a value (H or L) to a pin in expander
    I2C_master_start();
    I2C_master_send(0b01001110);  // sending to slave with address 01001110 (0100 is set already and 111 is user-defined) 
    I2C_master_send(0x0A);  // sending to register 10 (OLAT)
    I2C_master_send(level << pin);
    I2C_master_stop();
}

char getExpander() {  // reads from expander
    I2C_master_start();
    I2C_master_send(0b01001110);  // sending to slave with address 01001110 (0100 is set already and 111 is user-defined) 
    I2C_master_send(0x09);  // sending to register 9 (GPIO)
    I2C_master_restart();
    I2C_master_send(0b01001111);  // sending to slave with address 01001111 (0100 is set already and 111 is user-defined) 
    unsigned char r = I2C_master_recv();
    I2C_master_ack(1);
    I2C_master_stop();
    return r;
} 

int main() {
    PIC32_Startup();
    ANSELBbits.ANSB2 = 0;
    ANSELBbits.ANSB3 = 0;      // so that analog functionality is turned off
    I2C_master_setup();  // chooses baud rate and turns I2C2 on
    initExpander(); 
    
    while(1) {
        // Making the PIC blink (heartbeat):
        while(!PORTBbits.RB4) { ; }
        _CP0_SET_COUNT(0);
        LATAbits.LATA4 = 1;    
        while(_CP0_GET_COUNT() < 10000000) { ; } //delay for 10m core ticks
        _CP0_SET_COUNT(0);
        LATAbits.LATA4 = 0;
        while(_CP0_GET_COUNT() < 10000000) { ; } //delay for 10m core ticks
        
        // Actual code:
        if ((getExpander() >> 7) == 0b00000000) {
            setExpander(0,1);
        }
        else {
            setExpander(0,0);
        }
    }
    return 0; 
}