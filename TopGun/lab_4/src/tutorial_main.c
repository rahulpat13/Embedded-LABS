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

/*******************************************************************************
Macro Definitions
*******************************************************************************/
#define TIMER_COUNT_600MS 56250  /* 600mS with 48MHz pclk/512 */
#define MCU_NAME "   RX63N    "
#define CHANNEL_0   0
#define TIMER_COUNT_400MS 9375*4
#define TIMER_COUNT_200MS 9375*2
/*******************************************************************************
External functions and public global variables. 
*******************************************************************************/
/* Statics test replacement variable */
uint8_t g_working_string[29] = " Designed by Rahul & Raj "; /* 12 characters plus NULL terminator. */

/* Statics test const variable */
const uint8_t g_replacement_str[] = "TEST TEST"; /* Must be 12 chars or less. */

volatile bool g_sw1_press = false;
volatile bool g_sw2_press = false;
volatile bool g_sw3_press = false;


/*******************************************************************************
Private function prototypes and global variables
*******************************************************************************/
static void statics_test(void);
static void timer_callback(void);
volatile unsigned int switch_press = 0;
volatile static bool g_delay_complete = false;
volatile unsigned int state = 0;
volatile unsigned int count = 0;
volatile  bool pause_flag = false;

static void RESET_ALL_LEDS(void);
static void ALL_RED_LEDS_ON(void);
static void ALL_GREEN_LEDS_ON(void);
static void BLINK_RED_LEDS(void);
static void BLOCK_UNTIL_SWITCH_PRESS(void);
static void BLOCK_UNTIL_SWITCH_PRESS_1(void);
static void BLINK_GREEN_LEDS (void);
volatile unsigned int x_obs = 0;
volatile unsigned int y_obs = 0;
volatile unsigned int jet_x = 0;
volatile unsigned int jet_y = 0;
volatile unsigned int i = 0;
volatile static bool enemy_flag = 0; //variable for creating enemy plane
volatile static bool hit_flag = 0; //variable for tracking hits and hit animation
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
    lcd_display(LCD_LINE1, " Hello World ");
    lcd_display(LCD_LINE2, MCU_NAME);

    cmt_init(); /* Initialize the CMT unit for application timing tasks. */ 

    R_SWITCHES_Init(); /* Prepare the board switches for use. */
	
	Set_Font_Bitmap();
	Set_LCD_Pos(0,0);
	Set_LCD_Char(0);  
	 
	/* Set up the callback function on cmt channel 0 */   
    cmt_callback_set(CHANNEL_0, &BLINK_RED_LEDS);
	
	/* Start 400mS count on cmt channel 0. */
    cmt_start(CHANNEL_0, TIMER_COUNT_400MS);
	
	BLOCK_UNTIL_SWITCH_PRESS();
	
	/* Sctop counting on cmt channel 0. */
    	cmt_stop(CHANNEL_0);
		
		lcd_clear();
	
		
		Set_Font_8_by_8();
		statics_test();
			   
    /* Set up the callback function on cmt channel 0 */   
	cmt_callback_set(CHANNEL_0, &BLINK_GREEN_LEDS);
	
	/* Start 200mS count on cmt channel 0. */
    cmt_start(CHANNEL_0, TIMER_COUNT_200MS);
	
	
	BLOCK_UNTIL_SWITCH_PRESS();
	
	/* Stop counting on cmt channel 0. */
    cmt_stop(CHANNEL_0);

	lcd_clear();
	
	timer_adc();

    while (1)
    {	
		if(g_sw3_press == true){
			for(i=0;i<1000;i++){
			}
			if(g_sw3_press ==true){
				pause_flag = !pause_flag;
				g_sw3_press = false;
				lcd_clear();
			}	
		}
		
		
		if (pause_flag){
			RESET_ALL_LEDS();
 			Set_Font_Bitmap();			 
		}

		else if(!(pause_flag)){	
			ALL_RED_LEDS_ON();
			Set_Font_8_by_8();			
			lcd_display(LCD_LINE1, "    PAUSED   ");
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
    lcd_display(LCD_LINE1, "   TOP GUN   ");
	lcd_display(LCD_LINE2, "     BY:   ");
	lcd_display(LCD_LINE3, "    RAHUL   ");
	lcd_display(LCD_LINE4, "     RAJ   ");
	
	    /* Declare loop count variable */
    
//	uint8_t loop_counter;

    /* Write the initial contents of g_working_string to the debug LCD */
  //  lcd_display(LCD_LINE2, g_working_string);

    /* Set up the callback function on cmt channel 0 */   
  //  cmt_callback_set(CHANNEL_0, &timer_callback);
    
    /* Replace each element of g_working_string in each loop iteration with the contents 
       of g_replacement_str */
   // for (loop_counter = 0; loop_counter < strlen((char*)g_replacement_str); loop_counter++)
   // {
        /* Replace character from g_working_string with characer from g_replacement_str */
     //   g_working_string[loop_counter] = g_replacement_str[loop_counter];

        /* Write current contents of g_working_string to the debug LCD */
       // lcd_display(LCD_LINE2, g_working_string);
   
      //  g_delay_complete = false; /* Clear this flag variable. */
        
        /* Use a timer to create the delay between each loop iteration */         
     //   cmt_start(CHANNEL_0, TIMER_COUNT_600MS); /* 600mS period. */
        
      //  while(!g_delay_complete)
      //  {
            /* wait until the delay has completed. */
      //  }               
   // }

    /* Revert the debug LCD back to displaying the MCU_NAME */    
  //  lcd_display(LCD_LINE2, "TOP GUN");
    
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



/*BLOCK UNTIL SWITCH PRESS*/
void BLOCK_UNTIL_SWITCH_PRESS(void) {

    // loop until one of the switches is pressed
    while (!g_sw1_press && !g_sw2_press && !g_sw3_press){ 
       // do nothing
    }
    g_sw1_press = false;
    g_sw2_press = false;
    g_sw3_press = false;
}

/*BLINK RED LEDS */
void BLINK_RED_LEDS(void){
	if (state == 0){
		ALL_RED_LEDS_ON();
		state = 1;
	}
	else if(state == 1){
		RESET_ALL_LEDS();
		state = 0;	
	}	
}

/*Reset all LEDs */
void RESET_ALL_LEDS(void){
	PORTD.PODR.BYTE = 0xFF;       /* Turn off all LEDs on port D.*/
    PORTE.PODR.BYTE |= 0x0F;      /* Turn off all LEDs on port E.*/

}

/*All RED LEDS ON */
void ALL_RED_LEDS_ON(void){
	PORTD.PODR.BYTE = ~0xC7;
	PORTE.PODR.BYTE = ~0x01;
	
}

void ALL_GREEN_LEDS_ON(void){
	PORTD.PODR.BYTE = ~0x38;
	PORTE.PODR.BYTE = ~0xFE;	
}

/*CUSTOM PATTERN */
void BLINK_GREEN_LEDS (void){
	
	if (state == 0){
		ALL_GREEN_LEDS_ON();
		state = 1;
	}
	
	else if(state == 1){
		RESET_ALL_LEDS();
		state = 0;	
	}
}

/*Create Obstacle */
void create_obstacle(void){
	lcd_clear();
	if(!enemy_flag){ //create enemy plane
		x_obs = 80; //spawn on the other side of the screen
		y_obs = 8*(rand() % 8);
		enemy_flag = 1;
	}
	else{
		if(x_obs <= 23){ //check for collision 
			if((y_obs <= (jet_y + 16)) && (y_obs >= jet_y)){
				enemy_flag = 0;//respawn enemy
				hit_flag = 1;
			}
		}
		if(x_obs == 0)
		enemy_flag = 0;
		else{
		x_obs = x_obs - 5;
		
		if(hit_flag == 0){
			Set_LCD_Pos(x_obs, y_obs);
			Set_LCD_Char(7); //draw jet
			Set_LCD_Pos(jet_x, jet_y);
			Set_LCD_Char(3);
		}
		else{
			Set_LCD_Pos(x_obs,y_obs);
			Set_LCD_Char(7);
			Set_LCD_Pos(jet_x, jet_y);
			Set_LCD_Char(1);
			hit_flag = 1;
		}
	}
}	
	if(g_sw1_press == true){

		if(g_sw1_press == true){
	
			if(jet_y > 0x08 ){
					jet_y = jet_y - 8;
			}	
			else {
					jet_y = 0x00;
			}
		}
	g_sw1_press = false;
	lcd_clear();
	}		
	

	if(g_sw2_press == true){
		
		if(g_sw2_press == true){
			if(jet_y < 0x32){
					jet_y = jet_y + 8;	
			}	
			else{
					jet_y = 0x32;
			}
		}
	g_sw2_press = false;
	lcd_clear();	
	}	
			
		
}