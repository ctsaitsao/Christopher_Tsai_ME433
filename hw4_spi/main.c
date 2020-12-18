#include <math.h> 	            // for sine wave plotting
#include <xc.h>                 // for digital I/O
#include <sys/attribs.h>        // __ISR macro
#define CS LATAbits.LATA0       // chip select pin

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

void initSPI1(void) {
    ANSELA = 0;
    TRISAbits.TRISA0 = 0;     // set A0 as an output (manual CS)
//    SDI1Rbits.SDI1R = 0b0000; // ignore since pic32's SDI is not used
    RPA1Rbits.RPA1R = 0b0011; // A1 pin is SDO
    CS = 1;
    SPI1CON = 0;              // turn off the spi module and reset it
    SPI1BUF;                  // clear the rx buffer by reading from it
    SPI1BRG = 1;              // baud rate to ~10 MHz [SPI1BRG = (48000000/(2*desired))-1]
    SPI1CONbits.MSSEN = 0;
    SPI1CONbits.MODE32 = 0;
    SPI1CONbits.MODE16 = 0;   // 8 bit mode
    SPI1CONbits.SMP = 1;
    SPI1CONbits.SSEN = 0;
    SPI1STATbits.SPIROV = 0;  // clear the overflow bit
    SPI1CONbits.CKP = 0;      // clock is idle when lo, active when hi (depends on slave device)
    SPI1CONbits.CKE = 1;      // data changes when clock goes from hi to lo (depends on slave device)
    SPI1CONbits.MSTEN = 1;    // pic32 is the master
    SPI1CONbits.ON = 1;       // turn on spi 1
}

char SPI1_IO(char write) {
    SPI1BUF = write;
    while(!SPI1STATbits.SPIRBF) { // wait to receive the byte (when SPI peripheral is not busy transferring data)
        ;
    }
    return SPI1BUF;
}

void setVoltage(char channel, int voltage) {
    unsigned short t = 0;
	t = channel << 15; //channel is at the very end of the data transfer
	t = t | 0b0111000000000000;
	t = t | ((voltage & 0b1111111111) << 2); //only last 10 bits matter
	CS = 0;
	SPI1_IO(t >> 8); //because SPI can only transfer 8 bits at a time
    SPI1_IO(t);
    CS = 1;
}

int main(void) {
    PIC32_Startup();
    initSPI1();
    int i = 0;
    while(1) {
        _CP0_SET_COUNT(0);
        int i;
        float f = 512 +512*sin(i*2*3.1415/1000*10);  //should make a 10Hz sin wave, use 512 b/c don't want negatives
        setVoltage(0,f);
        i++;
        float h = 10*3.1415*cos(10*3.1415*i);  // derivative of a 5 Hz sine wave
        float g;
        //        int j;   
        if (h>0) {
            g = 5*1023*i/1000;
        }
        else if (h<0) {
            g = -5*1023*i/1000;
        }
        
//        if (g > 1023) {
//            j=-1;
//        }
//        else if (g < 1023) {
//            j = 1; 
//        }
//        else if (g < 0) {
//            j = j - 1;
//        }
        setVoltage(1,g);
        while(_CP0_GET_COUNT() < 24000) {;}  // core timer = sys/2 = 24 MHz --> 1 ms = 24000 clock cycles 
    }
    return 0;
}