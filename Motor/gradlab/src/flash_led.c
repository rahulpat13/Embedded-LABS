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
/*******************************************************************************
* File Name     : flash_led.c 
* Version       : 1
* H/W Platform  : YRDKRX63N
* Description   : Defines LED flashing functions used in this tutorial.
*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 24.02.2012     1.00        First release
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "platform.h"

/* Defines switch functions and global variables used in this file */
/* Provides declarations of functions defined in this file. */
#include "flash_led.h"

#include "cmt_periodic_multi.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define TIMER_COUNT_100MS   9375 /* 0.1 second with 48MHz pclk / 512. */ 
#define CHANNEL_0   0

/*******************************************************************************
Local Function Prototypes and global variables.
*******************************************************************************/
static void cmt_callback_flash(void);

/* Flash count variable - the LEDs are toggled as many times as the value this
   variable is initialised to. */
volatile static uint32_t g_flash_count = 200u;

/*******************************************************************************
External global variables used in this file.
*******************************************************************************/
/* The switch press flag indicates when when a switch has been pressed. */
extern volatile bool g_sw1_press;

/*******************************************************************************
* Function name: flash_led
* Description  : Function configures a CMT timer to periodically call the call-
*                back function cmt_callback_flash which toggles the LEDs. The function
*                waits in a while loop until the user LEDs have been toggled
*                200 times and the g_flash_count variable reaches zero. If switch
*                SW1 is pressed, the while loop ends early.
* Argument     : none
* Return value : none
*******************************************************************************/
void flash_led(void)
{
    /* Start with all LEDs off. */
    PORTD.PODR.BYTE = 0xFF;       /* Turn off all LEDs on port D.*/
    PORTE.PODR.BYTE |= 0x0F;      /* Turn off all LEDs on port E.*/
        
    /* Initialize a CMT one-shot timer. */ 
    cmt_init();
    /* Set up the callback function on cmt channel 0 */   
    cmt_callback_set(CHANNEL_0, &cmt_callback_flash);
    /* Start 100mS count on cmt channel 0. */
    cmt_start(CHANNEL_0, TIMER_COUNT_100MS);

    /* Wait in a loop until the variable g_flash_count is zero. LEDs continue to
       flash in the meantime. If SW1 is pressed the count is zeroed early. */
    while (g_flash_count > 0)
    {
        /* Check if switch SW1 has been pressed. */
        if (g_sw1_press)
        {         
            g_flash_count = 0; /* Set the flash count variable to zero. */
        }
    }
    
    g_sw1_press = false; /* Done with the flag so clear it. */

    /* Stop counting on cmt channel 0. */
    cmt_stop(CHANNEL_0);
}
/* End of function flash_led(). */

/*******************************************************************************
* Function name: cmt_callback_flash
* Description  : This callback function is called every 100ms by a CMT interrupt.
*                The function calls the toggle_led function which toggles the 
*                user LEDs. The function decrements the count variable 
*                g_flash_count each time it is executed.
* Argument     : none
* Return value : none
*******************************************************************************/
static void cmt_callback_flash(void)
{
    /* Decrement the flash count variable. */
    g_flash_count--;

    /* Execute the toggle LEDs function. */
    toggle_led();
    
} /* End of function cmt_callback_flash(). */



/*******************************************************************************
* Function name: toggle_led
* Description  : This toggles the output status of pins connected to the user LEDs
* Argument     : none
* Return value : none
*******************************************************************************/
void toggle_led(void)
{
    uint8_t bit_mask = 0;
    
    PORTD.PODR.BYTE = ~PORTD.PODR.BYTE;  /* Toggle all LEDs on port D.*/
    
    /* Toggle all LEDs on port E (only toggle lower 4 bits).*/
    bit_mask = (PORTE.PODR.BYTE & 0xF0);
    bit_mask |= ~(PORTE.PODR.BYTE & 0x0F);
    PORTE.PODR.BYTE = bit_mask;  
    
} /* End of function toggle_led(). */

