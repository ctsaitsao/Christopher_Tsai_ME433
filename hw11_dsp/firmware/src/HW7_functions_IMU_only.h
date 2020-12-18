#ifndef _HW7_FUNCTIONS_H    /* Guard against multiple inclusion */
#define _HW7_FUNCTIONS_H

void PIC32_Startup(void);

void initLSM6DS33(); 
 
void I2C_read_multiple(unsigned char address, unsigned char register_, unsigned char * data, int length); 

#endif