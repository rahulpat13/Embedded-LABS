/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer *
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/******************************************************************************
* File Name     : riic_master_main.c
* Version       : 1.0
* Device(s)     : RX
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.0
* OS            : None
* H/W Platform  : YRDKRX6xx
* Description   : Demonstrates usage of RIIC unit in master mode by actively 
*                 reading from the ADXL345 digital accelerometer, and optionally
*                 from the ADT7420 digital thermal sensor if present. 
*
* Operation     : 1. Build and download the sample code to the YRDK.
* 
*                 2. Click 'Reset Go' to start the software. The name of the
*                    sample will by displayed on the LCD.
*                    
*                 3. Pick up the board and tilt the board forward/backward and
*                    left/right.  
*
*                 4. As you slowly roll the board in a circular motion, observe 
*                    the circle LEDs light to indicate the direction of the tilt.
*               
*                 5. Observe the board temperature displayed on the LCD. Blow on
*                    the sensor or touch it with a finger to see the temperature
*                    reading change.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 16.01.2012     1.00        First release
*           07.02.2012                 Added thermal sensor demo.
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <machine.h>
#include "platform.h"
#include "r_riic_rx600.h"
#include "r_riic_rx600_master.h"
//#include "cmt_periodic.h"
#include "riic_master_main.h"
//#include "accelerometer_demo.h"
#include "temp_sensor.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define TIMER_COUNT 9375  /* Should give 100mS with 48MHz pclk/512 */

/*******************************************************************************
Prototypes for local functions
*******************************************************************************/
static riic_ret_t riic_master_init(void);
void static cmt_callback(void);

/* Not all boards have a thermal sensor, so this will be tested. */
static bool g_thermal_sensor_good = false;  

/*******************************************************************************
* Function name: main
* Description  : Main program function. Calls initialization functions for the 
*                peripherals used in the demo. Starts the timer that generates
*                periodic interrupts in which the accelerometer and thermal sensor
*                demos are updated. Sits in an infinite while loop and the demo 
*                operates in the background.
* Arguments    : none
* Return value : none
*******************************************************************************/
void main(void)
{   
    /* Declare and initialize the IIC result code. */
    riic_ret_t iic_ret = RIIC_OK; 
    
    /* Initialize LCD */
    lcd_initialize();
    
    /* Clear LCD */
    lcd_clear();
    
    /* Display message on LCD */
    lcd_display(LCD_LINE1, "  RENESAS   ");
    lcd_display(LCD_LINE2, " YRDKRX63N  ");
    
    /* Display IIC mode on the debug LCD */
    lcd_display(LCD_LINE3, "RIIC Master ");
    lcd_display(LCD_LINE4, "Accel data: ");    

    /* 
        STDOUT is routed through the virtual console window tunneled through the 
        JTAG debugger. Open the console window in HEW to see the output 
    */
    printf("This is the debug console\r\n");   
    
    /* Prepare an RIIC channel for master mode communications. */
    iic_ret |= riic_master_init();
    while (RIIC_OK != iic_ret)
    {
        nop(); /* Failure to initialize here means demo can not proceed. */    
    }

    /* Some boards may not have the thermal sensor present. This sequence 
       demonstrates an example of how to recover from some bus error conditions. */
    iic_ret |= thermal_sensor_init();    /* Initialize IIC thermal sensor */
   
    if (RIIC_OK == iic_ret)
    {
        g_thermal_sensor_good = true;
    }
    /* Thermal sensor not present or malfunctioning. Remove it from the demo. */ 
    else  
    {   /* Got a NACK. */
        g_thermal_sensor_good = false;  
          
        iic_ret = R_RIIC_Reset(RIIC_CHANNEL); /* Do soft reset to clean up bus states. */
        
        if (RIIC_RESET_ERR & iic_ret) /* Check for successful IIC soft-reset. */
        {   /* Soft-reset failed. Need to do complete re-initialization. */
            /* Need to release the channel first berfore it can be re-initialized. */
            R_RIIC_ReleaseChannel(RIIC_CHANNEL);    
            iic_ret = riic_master_init(); 
        }
        
        while (RIIC_OK != iic_ret)
        {
            nop(); /* Failure to initialize here means demo can not proceed. */
        }               
    }

    /* Prepare the accelerometer for the demo. */
    accelerometer_init();
    
    /* Initialize a CMT timer for periodicly triggering accelerometer read. */ 
    cmt_periodic_init();
    /* Set the function that will be called at every CMT timer match interval. */
    cmt_periodic_callback_set(&cmt_callback);    

    cmt_periodic_start(TIMER_COUNT); /* Starts the RIIC demo. */

    while (1) 
    { 
        /* The demo runs continuously in the background. */
    }
}   /* End of function main() */


/*******************************************************************************
* Function name: riic_master_init
* Description  : Prepare an RIIC channel for master mode communications used in the demo
* Arguments    : none
* Return value : riic_ret_t -
*                   RIIC result code
*******************************************************************************/
static riic_ret_t riic_master_init(void) 
{ 
    /* Initialize a data structure used by the R_RIIC_Init() API function. */
    /* See r_riic_rx600.h for definitions of this structure. */
    riic_config_t   riic_master_config = {RIIC_CHANNEL, RIIC_MASTER_CONFIG, 
                                          0, 
                                          0, 
                                          0, 
                                          0, 
                                          MASTER_IIC_ADDRESS_LO, 
                                          MASTER_IIC_ADDRESS_HI}; 

    /* Initialize the RIIC channel for communication with the accelerometer. */
    return R_RIIC_Init(&riic_master_config);
}


/*******************************************************************************
* Function name: riic_master_init
* Description  : CMT interrupt callback function. This callback function is
*                executed after every period of the CMT timer. 
* Argument     : none
* Return value : none
*******************************************************************************/
void static cmt_callback(void)
{
    accelerometer_demo_update();
    if (g_thermal_sensor_good) /* Only run thermal sensor demo if it is present. */
    {
        temperature_display();
    }
}
