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
* File Name     : main.c
* Version       : 1.0
* Device(s)     : RX63N
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.0
* OS            : None
* H/W Platform  : YRDKRX63N
* Description   : Empty application project
*                 This application provides a handy framework to serve as a
*                 generic basis for any application. The MCU startup procedures
*                 are provided, as well as some basic means for I/O. The LCD is 
*                 initialized, board switches and their interrupt ISR stubs are
*                 defined, and a simple loop blinks an LED to indicate that the
*                 board is operating.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 22.09.2011     1.00        First release              
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <machine.h>
#include "platform.h"
#include "r_switches.h"
#include "customADC.h"
/*******************************************************************************
* Prototypes for local functions
*******************************************************************************/
volatile bool g_sw1_press = false;
volatile bool g_sw2_press = false;
volatile bool g_sw3_press = false;

volatile unsigned int adcResult;
volatile  float adcCon;
const char * string;
const char * string1;
const char * string2;
volatile unsigned int adc12Flag;
volatile unsigned int adc10Flag;
volatile unsigned int i;
void adcInit(void);			// This function initializes the ADC for us
int adcRead(int channel);   // This function will read an analog value from the specified channel
/******************************************************************************
* Function name: main
* Description  : Main program function
* Arguments    : none
* Return value : none
******************************************************************************/
void main(void)
{
    /* Used to pace toggling of the LED */
    uint32_t led_counter;
    
    /* Initialize LCD */
    lcd_initialize();
    
    /* Clear LCD */
    lcd_clear();
    
    /* Display message on LCD */
    lcd_display(LCD_LINE1, "DARK KNIGHT");
    lcd_display(LCD_LINE2, " ADCTest  ");

    /* 
        STDOUT is routed through the virtual console window tunneled through the JTAG debugger.
        Open the console window in HEW to see the output 
    */
    printf("This is the debug console\r\n");
    
    /* The three pushbuttons on the YRDK board are tied to interrupt lines, set them up here */
    R_SWITCHES_Init();
    adcInit(); //nitialize ADC
   
    while (1)
    {
		
	// If switch 1 is pressed then display the appropriate channel number on the LCD and
	//read the adc value from the potentiometer and display it on the LCD.
		if(g_sw1_press == true){
				g_sw2_press = false;
				g_sw3_press = false;
				lcd_clear();
				adcResult = adcRead(AN2);		
				sprintf(string, "%d", adcResult);
				lcd_display(LCD_LINE3, "  Channel 2:  ");
				lcd_display(LCD_LINE4+4, (uint8_t *)string);
		}
	//If switch 2 is pressed then along with the channel number display the corresponding 
	//voltage value on the LCD	
		if(g_sw2_press == true){
				g_sw1_press = false;
				g_sw3_press = false;
				lcd_clear();
				adcResult = adcRead(AN2);		
				sprintf(string, "%d", adcResult);
				adcCon = convertADC(adcResult);
				sprintf(string1, "%.04f Volts", adcCon);
				lcd_display(LCD_LINE3, "  Channel 2:  ");			
				lcd_display(LCD_LINE4, (float *)string1);
				
		}
		//If switch 3 is pressed then display the channel number being read by the ADC
		// and also display the corresonding ADc value on the LCD
		if(g_sw3_press == true){
				g_sw1_press = false;
				g_sw2_press = false;
				lcd_clear();
				adcResult = adcRead(AN3);		
				sprintf(string2, "%d", adcResult);
				//adcCon = convertADC(adcResult);
				//sprintf(string1, "%.04f Volts", adcCon);
				lcd_display(LCD_LINE3, "  Channel 3:  ");			
				lcd_display(LCD_LINE4+4 , (uint8_t *)string2);
		}
		
	}
} /* End of function main() */

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
