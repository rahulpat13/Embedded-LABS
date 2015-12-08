
/*
File Name: customADC.C
Description: This file contains the code to initialize the ADC, read the value 
and convert it to appropriate voltage value. 
*/

//Include 
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "platform.h"
#include "customADC.h"


extern unsigned int adc12Flag;
extern unsigned int adc10Flag;

//Function name: adcInit
//Description: This function initializes the 12 Bit ADC

void adcInit(void){
	SYSTEM.PRCR.WORD = 0xA50B;
	SYSTEM.MSTPCRA.BIT.MSTPA17 = 0; //Power up the S12ADC
	
	PORT4.PODR.BIT.B2 = 0;    // Clear I/O pin data register to low output. 
    PORT4.PDR.BIT.B2  = 0;    // Set I/O pin direction to input. 
    PORT4.PMR.BIT.B2  = 0;    // First set I/O pin mode register to GPIO mode.     
    
	PORT4.PODR.BIT.B3 = 0;    // Clear I/O pin data register to low output. 
    PORT4.PDR.BIT.B3  = 0;    // Set I/O pin direction to input. 
    PORT4.PMR.BIT.B3  = 0;    // First set I/O pin mode register to GPIO mode.     
    
	
	MPC.P42PFS.BYTE = 0x80;   // Set port function register to analog input, no interrupt. 
	/* ADCSR: A/D Control Register  
    b7    ADST     0 a/d conversion start, Stop a scan conversion process
    b6    ADCS     0 Scan mode select, Single scan mode
    b5    Reserved 0 This bit is always read as 0. The write value should always be 0.
    b4    ADIE     0 Disables conversion complete IRQ to ICU
    b3:b2 CKS      0 A/D conversion clock select = PCLK/8
    b1    TRGE     0 Disables conversion to start w/ trigger
    b0    EXTRG    0 Trigger select, Scan conversion start by a timer source or software
	*/
	S12AD.ADCSR.BYTE = 0x00;
	/* ADADS0: A/D-converted Value Addition Mode Select Register 0
    b15:b0  ADS0: A/D-Converted Value Addition Channel Select for AN000 to AN015.
    */
	S12AD.ADADS0.WORD = 0x0000;
	/* ADADS1: A/D-converted Value Addition Mode Select Register 1
	b15:b5  Reserved: These bits are always read as 0. The write value should always be 0.
    b4:b0   ADS1: A/D-Converted Value Addition Channel Select for AN016 to AN020.
    */
	S12AD.ADADS1.WORD = 0x0000;
	/* ADADC: A/D-Converted Value Addition Count Select Register
    b1:b0   ADC: 00 = 1 time conversion (same as normal conversion)
    */
	S12AD.ADADC.BYTE = 0x00;   // 1-time conversion 
	/* ADCER: A/D Control Extended Register
    b15     ADRFMT:0  Right align the data in the result registers
    b5      ACE:0 Disables automatic clearing of ADDRn after it is read
    */
	S12AD.ADCER.WORD = 0x0000;   // Right align data, automatic clearing off. 
	/* ADSTRGR: A/D Start Triggger Select Register
    b7:b4   Reserved. Always read/write 0.
    b3:b0   ADSTRS: 0, Software trigger or ADTRG0#
    */
	S12AD.ADSTRGR.BYTE = 0x00;

	
}
//Function Name: adcRead
//Description: This function reads the channel passed to it and returns the adc value as an integer
 int adcRead(int channel){
	 volatile unsigned int adcOut;
	
	S12AD.ADANS0.WORD = channel; // Read the channel
	S12AD.ADANS1.WORD = 0x0000;
	S12AD.ADCSR.BIT.ADST = 1;    //start conversion
	//If adc12 has started conversion then set the adc12Flag as 1
	if(S12AD.ADCSR.BIT.ADST == 1){
		adc12Flag = 1;	
	}
	if(channel == AN2){
		adcOut = S12AD.ADDR2;
	}
	else if(channel == AN3){
		adcOut = S12AD.ADDR3;	
	}
	return adcOut;
 }

//Function Name: convertADC
//Description: This function converts adc value to the corresponding voltage value. It uses the appropriate formula
//depending on which adc flag is set.
 float convertADC(int adcOut){
	 float result;
	 //If adc12Flag is set then use the formula for the 12 bit adc
	 if(adc12Flag == 1){
		 result = (float) ((adcOut * (Vpref - Vnref)) / (N12 -1 ));
	 }
	 //If adc10Flag is set then use the formula for the 10 bit adc
	 if(adc10Flag == 1){
		 result = (float)  ((adcOut * (Vpref - Vnref)) / (N10 - 1));	 
	 }
	 return result;
 }