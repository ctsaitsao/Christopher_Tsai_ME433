/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "system/common/sys_module.h"   // SYS function prototypes
#include <xc.h>                         // processor SFR definitions
#include <stdio.h>
#include <math.h>
#include "ili9341.h"
#include "I2C_master_no_int.h"
#include "HW7_functions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
    
    PIC32_Startup();
    ANSELBbits.ANSB2 = 0;
    ANSELBbits.ANSB3 = 0;  
    I2C_master_setup();  // chooses baud rate and turns I2C2 on
    initLSM6DS33();  // turns on gyroscope and accelerometer, among other things
    SPI1_init();
    LCD_init();
    LCD_clearScreen(ILI9341_BLUE);  // clear to background color
    LCD_write_WHO_AM_I();
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;
       
        
            if (appInitialized)
            {
            
                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
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
                register_data_appended[i] = ((register_data[i*2+1] << 8) | register_data[(i*2)]);
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
        
            break;
        }

        /* TODO: implement your application state machine.*/
        

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

 

/*******************************************************************************
 End of File
 */
