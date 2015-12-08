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
* File Name     : tutorial_main.c
* Version       : 1.0
* Device(s)     : RX63N
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.0
* H/W Platform  : YRDKRX63N
* Description   : This tutorial sample will demonstrate basic use of the YRDK
*                 hardware and the J-Link debugger. 
* Operation     : 1.  Build and download the tutorial project to the YRDK.
*
*                 2.  Click 'Reset Go' to start the software.
*         
*                 3.  "Renesas RX63N" will be displayed on the debug LCD,
*                     and the user LEDs will flash.
*          
*                 4.  The user LEDs will flash at a fixed rate until either a
*                     switch is pressed, or the LEDs have flashed 200 times.
*          
*                 5.  The software will then vary the rate in which the LEDs flash
*                     by the position of the potentiometer, VR1. Turn the pot in
*                     both directions, and observe the change in flash rate.
*          
*                 6.  While the LEDs flash at a varying rate, the second line of
*                     the debug LCD will show " STATIC ". The second LCD line will
*                     slowly be replaced, letter by letter, with the string 
*                     "TEST TEST".
*          
*                 7.  Once the second line of the debug LCD shows "TEST TEST"
*                     fully, it will return back to showing "RX63N". The LEDs will
*                     continue to flash at a varying rate until the tutorial is
*                     stopped.
*          
*                 8.  In order to repeat the tutorial, click 'Reset Go'.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 24.02.2012     1.00        First release              
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "platform.h" 
#include "r_switches.h"
#include "cmt_periodic_multi.h"
#include "timer_adc.h"
#include "flash_led.h"
#include "customled.h"
#include "int.h"
/*******************************************************************************
Macro Definitions
*******************************************************************************/
#define TIMER_COUNT_600MS 56250  /* 600mS with 48MHz pclk/512 */
#define MCU_NAME "   RX63N    "
#define CHANNEL_0   0


/*******************************************************************************
External functions and public global variables. 
*******************************************************************************/
/* Statics test replacement variable */
uint8_t g_working_string[13] = "   STATIC   "; /* 12 characters plus NULL terminator. */

/* Statics test const variable */
const uint8_t g_replacement_str[] = "TEST TEST"; /* Must be 12 chars or less. */

volatile bool g_sw1_press = false;
volatile bool g_sw2_press = false;
volatile bool g_sw3_press = false;
volatile unsigned int count = 1;
volatile unsigned int waveDriveFlag = 1;
volatile unsigned int fullStepFlag = 0;
volatile unsigned int halfStepFlag = 0;
/*******************************************************************************
Private function prototypes and global variables
*******************************************************************************/
static void statics_test(void);
static void timer_callback(void);

volatile static bool g_delay_complete = false;


/******************************************************************************
* Function name: main
* Description  : Main program function. The function first initialises the debug
*                LCD, and displays the splash screen on the LCD. The function
*                then calls the flash_led, timer_adc and statics_test functions. 
*                These start by flashing the LEDs at a fixed rate until a switch is
*                pressed, then the flash rate varies with the position of the   
*                pot, RV1. Finally, statics_test displays " STATICS " on the    
*                debug LCD, which is replaced with "TEST TEST" a letter at a 
*                time.
* Arguments    : none
* Return value : none
******************************************************************************/
void main(void)
{	
    /* Initialize the debug LCD on the RSPI bus. */
    lcd_initialize();
    
    /* Clear LCD */
    lcd_clear();
    
    /* Display message on LCD */
    lcd_display(LCD_LINE1, "  RENESAS   ");
    lcd_display(LCD_LINE2, MCU_NAME);
	
    cmt_init(); /* Initialize the CMT unit for application timing tasks. */ 
	LED_Init();
    //R_SWITCHES_Init(); /* Prepare the board switches for use. */
    interrupt_init();
	
    /* Executes the initial LED flashing sequence */
    //flash_led();  

    /* Initiates the ADC-varying LED flash sequence */ 
    timer_adc();
	LED_Off(0);
    /* Executes the static variable test */
    //statics_test();

    while (1)
    {
		if(waveDriveFlag == 1){
	        switch(count){
	            case 1: 
					LED_Off(0);
	                phase1_ON();
	                break;
	            case 2:
					LED_Off(0);
	                phase2_ON();
	                phase1_OFF();
	                break;
	            case 3:
					LED_Off(0);
	                phase3_ON();
	                phase2_OFF();
	                break;
	            case 4:
					LED_Off(0);
	                phase4_ON();
	                phase3_OFF();
	                break;
	            case 5:
					LED_Off(0);
	                phase5_ON();
	                phase4_OFF();
	                break;
	            case 6:
					LED_Off(0);
	                phase6_ON();
	                phase5_OFF();
	                break;
				default:
	            	break;

	        }
		}
		
		if(fullStepFlag == 1){
	        switch(count){
	            case 1: 
					LED_Off(0);
	                phase1_ON();
					phase2_ON();
	                break;
	            case 2:
					LED_Off(0);
	                phase2_ON();
					phase3_ON();
	                phase1_OFF();
	                break;
	            case 3:
					LED_Off(0);
	                phase3_ON();
					phase4_ON();
	                phase2_OFF();
	                break;
	            case 4:
					LED_Off(0);
	                phase4_ON();
					phase5_ON();
	                phase3_OFF();
	                break;
	            case 5:
					LED_Off(0);
	                phase5_ON();
					phase6_ON();
	                phase4_OFF();
	                break;
	            case 6:
					LED_Off(0);
	                phase6_ON();
					phase1_ON();
	                phase5_OFF();
	                break;
				default:
	            	break;

	        }
		}

        if(halfStepFlag == 1){
            switch(count){
                case 1: 
                    LED_Off(0);
                    phase1_ON();
                    break;
                case 2:
                    LED_Off(0);
					phase1_ON();
                    phase2_ON();
                    break;
                case 3:
                    LED_Off(0);
                    phase2_ON();
                    break;
                case 4:
                    LED_Off(0);
                    phase2_ON();
                    phase3_ON();
                    break;
                case 5:
                    LED_Off(0);
                    phase3_ON();                    
                    break;
                case 6:
                    LED_Off(0);
                    phase3_ON();
					phase4_ON();
                    break;
				case 7:
					LED_Off(0);
					phase4_ON();
					break;
				case 8:
					LED_Off(0);
					phase4_ON();
					phase5_ON();
					break;
				case 9:
					LED_Off(0);
					phase5_ON();
					break;
				case 10:
					LED_Off(0);
					phase5_ON();
					phase6_ON();
					break;
				case 11:
					LED_Off(0);
					phase6_ON();
					break;
				case 12:
					LED_Off(0);
					phase6_ON();
					phase1_ON();
					break;
                default:
                    break;
            }
        }
    }
} /* End of function main(). */


/*******************************************************************************
* Function name: statics_test
* Description  : Static variable test routine. The function replaces the
*                contents of the string g_working_string with that of g_replacement_str, one
*                element at a time. Right-click the variable g_working_string, and select
*                instant watch - click add in the subsequent dialog. If you step
*                through the function, you can watch the string elements being
*                overwritten with the new data.
* Arguments    : none
* Return value : none
*******************************************************************************/
void statics_test(void)
{
    /* Declare loop count variable */
    uint8_t loop_counter;

    /* Write the initial contents of g_working_string to the debug LCD */
    lcd_display(LCD_LINE2, g_working_string);

    /* Set up the callback function on cmt channel 0 */   
    cmt_callback_set(CHANNEL_0, &timer_callback);
    
    /* Replace each element of g_working_string in each loop iteration with the contents 
       of g_replacement_str */
    for (loop_counter = 0; loop_counter < strlen((char*)g_replacement_str); loop_counter++)
    {
        /* Replace character from g_working_string with characer from g_replacement_str */
        g_working_string[loop_counter] = g_replacement_str[loop_counter];

        /* Write current contents of g_working_string to the debug LCD */
        lcd_display(LCD_LINE2, g_working_string);
   
        g_delay_complete = false; /* Clear this flag variable. */
        
        /* Use a timer to create the delay between each loop iteration */         
        cmt_start(CHANNEL_0, TIMER_COUNT_600MS); /* 600mS period. */
        
        while(!g_delay_complete)
        {
            
        }               
    }

    /* Revert the debug LCD back to displaying the MCU_NAME */    
    lcd_display(LCD_LINE2, MCU_NAME);
    
} /* End of function statics_test(). */


/******************************************************************************
* Function name: timer_callback
* Description  : This function is called from the cmt_timer compare-match ISR.
*              : It sets a global flag that is polled for delay loop control.
* Arguments    : None
* Return value : None
******************************************************************************/
void timer_callback(void)
{
    cmt_stop(CHANNEL_0);
    g_delay_complete = true;
    
} /* End of function timer_callback() */



/******************************************************************************
* Function name: sw1_callback
* Description  : Callback function that is executed when SW1 is pressed.
*                Called by sw1_isr in r_switches.c
* Arguments    : none
* Return value : none
******************************************************************************/
void sw1_callback(void)
{
    g_sw1_press = true; /* The switch was pressed. */
} /* End of function sw1_callback() */


/******************************************************************************
* Function name: sw2_callback
* Description  : Callback function that is executed when SW2 is pressed.
*                Called by sw2_isr in r_switches.c
* Arguments    : none
* Return value : none
******************************************************************************/
void sw2_callback(void)
{
    g_sw2_press = true; /* The switch was pressed. */
} /* End of function sw2_callback() */


/******************************************************************************
* Function name: sw3_callback
* Description  : Callback function that is executed when SW3 is pressed.
*                Called by sw3_isr in r_switches.c
* Arguments    : none
* Return value : none
******************************************************************************/
void sw3_callback(void)
{
    g_sw3_press = true; /* The switch was pressed. */
} /* End of function sw3_callback() */

