#ifndef _HW7_FUNCTIONS_H    /* Guard against multiple inclusion */
#define _HW7_FUNCTIONS_H

//#include <xc.h>           // processor SFR definitions
//#include <stdio.h>
//#include <math.h>
//#include "ili9341.h"
//#include "I2C_master_no_int.h"

void PIC32_Startup(void);

void LCD_drawLetter(char letter, int x, int y, int c1, int c2); 

void LCD_drawString(char *m, int x, int y, int c1, int c2); 

void initLSM6DS33(); 
 
void I2C_read_multiple(unsigned char address, unsigned char register_, unsigned char * data, int length); 

void LCD_write_WHO_AM_I(); 

#endif