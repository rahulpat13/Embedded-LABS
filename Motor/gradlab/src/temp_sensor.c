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
* File Name     : thermal_sensor.c
* Version       : 1.0
* H/W Platform  : YRDKRX63N
* Description   : Driver for the ADT7420 I2C thermal sensor.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 07.02.2012     1.00        First release
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <string.h>
#include <stdio.h>
#include <machine.h>

#include "platform.h"
#include "r_riic_rx600.h"
#include "r_riic_rx600_master.h"
//#include "riic_master_main.h"

#include "temp_sensor.h"
/* Defines ADT7420 parameters */
#include "ADT7420.h"


/*******************************************************************************
* Function name: thermal_sensor_init
* Description  : This function configures the ADT7420 thermal device. 
* Argument     : none
* Return value : riic_ret_t -
*                   RIIC return code
*******************************************************************************/
riic_ret_t thermal_sensor_init(void)
{
    uint8_t     target_data;
    uint8_t     addr_and_register[2]; /* Storage for the slave address and target register. */
    riic_ret_t  ret = RIIC_OK;

    /* To write to a specific register in the thermal sensor, first transmit its 
       I2C slave address together with the register number. */
    /* The 7-bit I2C address of the ADT7420 and the R/W bit. */            
    addr_and_register[0] = ADT7420_ADDR; 
    
    /* The register address of the configuration register. */
    addr_and_register[1] = ADT7420_CONFIG_REG;

    ret |= R_RIIC_MasterTransmitHead(CHANNEL_0, addr_and_register, 2);

    if (RIIC_OK == ret)
    {       
	    /* Configuration data: 0x00 = 13-bit resolution. */
	    /* Sign bit + 12 bits gives a temperature resolution of 0.0625°C.  */	
        target_data = 0x00; 
    
        /* Now write the data from the source buffer into the target register. */   
        ret |= R_RIIC_MasterTransmit(CHANNEL_0, &target_data, 1);   
    }
    
    return ret;                  
} /* End of function thermal_sensor_init()  */


/*******************************************************************************
* Function name: thermal_sensor_read
* Description  : This function fetches the thermal sensor temperature data and 
*                displays it on the LCD. Called by the CMT interrupt callback 
*                function, this function is executed after every period of the 
*                CMT timer.
* Argument     : none
* Return value : int16_t -
*                   signed temperature value in nearest 10ths of °C.
*******************************************************************************/
int16_t thermal_sensor_read(void)
{
    uint8_t     target_data[2]; /* Room for 2 bytes of data read from thermal sensor */
    uint8_t     addr_and_register[2]; /* Storage for the slave address and target register. */
    riic_ret_t  ret = RIIC_OK;

    uint16_t	raw_temp; /* signed */
	int16_t		signed_temp;
    
    /* To read from a specific register in the thermal sensor, first transmit the 
       its I2C slave address together with the register number. */         
    /* The 7-bit I2C address of the ADT7420 and the R/W bit. */
    addr_and_register[0] = ADT7420_ADDR;     
    /* The register address of the first byte of temperature data. */ 
    addr_and_register[1] = ADT7420_TEMP_MSB_REG;   
    
    /* Send the I2C slave address and register number (2 bytes). */
    ret |= R_RIIC_MasterTransmitHead(CHANNEL_0, addr_and_register, 2);
    
    /* Now read 2 bytes of data from the target register into the destination buffer. */    
    ret |= R_RIIC_MasterReceive(CHANNEL_0, ADT7420_ADDR, target_data, 2);  

	/* 	
	After right-shift 3 to drop lowest 3 bits,
	Sign bit + 12 bits gives a temperature resolution of 0.0625°C.  	
	Positive Temperature = ADC Code (dec)/16
	Negative Temperature = (ADC Code (dec) - 8192)/16
	where Bit 13 (sign bit) is included.
	*/

    /* Convert the device measurement into a signed decimal value. */
    raw_temp = ((uint16_t)(target_data[0]<<8)) + target_data[1];
    raw_temp = raw_temp >> 3;				/* drop 3 lowest bits of status data */       

    signed_temp = raw_temp;
	
	if (signed_temp & 0x1000) /* 13th bit is 1, so temperature is negative */
	{
		/* all this is round about way of dealing with odd 2-s complement value */
		signed_temp -= 8192;				/* becomes negative int value */
		signed_temp *= -1; 					/* work on it as positive for now */
		signed_temp *= 100;					/* value as 100ths */
		signed_temp = (uint16_t)signed_temp/16;		/* convert from 0.0625 units to ones */
		signed_temp = (signed_temp + 5)/10;	/* convert to tenths rounded to nearest */
		signed_temp *= -1; 					/* convert back to negative */	
	}
	else /* positive temperature */
	{
		signed_temp *= 100;						/* value as 100ths */
		signed_temp = (uint16_t)signed_temp/16;	/* convert from 0.0625 units to ones */
		signed_temp = (signed_temp + 5)/10;	/* convert to tenths rounded to nearest */
	}
            
	return signed_temp;  
} /* End of function thermal_sensor_read()  */

/*******************************************************************************
* Function name: temperature_display()
* Description  : Gets thermal sensor temperature data, rounds to nearest tenth,
*              : then converts it to a string and displays it on LCD.
* Argument     : none
* Return value : none
*******************************************************************************/
void temperature_display(void)
{
	int16_t temperature;
	uint8_t	i;
    
	/* The output display string buffer. */
 	uint8_t lcd_buffer[13] = {0}; /* Space for 12 characters + null. */

    /* Read the temperature */
	temperature = thermal_sensor_read();
 	
    /* Convert temperature value into a string. */
    sprintf((char *)lcd_buffer, "TEMP:%+d", temperature);

	/* Temperature value is in 10ths C. */
    /* Fit the decimal point character in before last character */
    i = strlen((char *)lcd_buffer);
    lcd_buffer[i] = lcd_buffer[i-1];    /* Move last character over by 1. */ 
	lcd_buffer[i-1] = '.';	 
	lcd_buffer[i+1] = 'C';              /* Celsius char. */ 

    /* Display the contents of lcd_buffer onto the debug LCD */
    lcd_display(LCD_LINE8, lcd_buffer);    
    
}
