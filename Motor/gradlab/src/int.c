#include <stdint.h>
#include <stdbool.h>
#include <machine.h>

#include "platform.h" // I/O Register and board definitions 



extern int count;
extern int waveDriveFlag;
extern int fullStepFlag;
extern int halfStepFlag;
volatile unsigned int directionFlag = 0;

void interrupt_init(void){

	//Make the pins inputs
	PORT4.PDR.BIT.B0 = 0; //Direction
	PORT4.PDR.BIT.B1 = 0; //Drive
	PORT4.PDR.BIT.B4 = 0;
	
	PORT4.PDR.BIT.B3 = 0;
	PORT4.PDR.BIT.B6 = 0;	

	//Set P40, P41, and P44 for IRQx function
	PORT4.PMR.BYTE  = 0x00;    // All GPIO for now 
    MPC.P40PFS.BYTE = 0x40;    // P40 is used as IRQ pin 
    MPC.P41PFS.BYTE = 0x40;    // P40 is used as IRQ pin 
	MPC.P44PFS.BYTE = 0x40;    // P40 is used as IRQ pin
	
	MPC.P43PFS.BYTE = 0x40; 
	MPC.P46PFS.BYTE = 0x40;
	
    //Set IRQ type (falling edge for direction and rising edge for drive pin)
    ICU.IRQCR[ 8].BIT.IRQMD = 0x01; 
    ICU.IRQCR[ 9].BIT.IRQMD = 0x01;
    ICU.IRQCR[12].BIT.IRQMD = 0x01; 
	
	ICU.IRQCR[11].BIT.IRQMD = 0x01;
	ICU.IRQCR[14].BIT.IRQMD = 0x01;
	
    //Set interrupt priority 
    IPR(ICU, IRQ8 ) = 0x03; 
    IPR(ICU, IRQ9 ) = 0x03;
    IPR(ICU, IRQ12) = 0x03;
	
	IPR(ICU, IRQ11) = 0x03;
	IPR(ICU, IRQ14) = 0x03;
	
    //Clear any pending interrupts
    IR (ICU, IRQ8 ) = 0;
    IR (ICU, IRQ9 ) = 0;
    IR (ICU, IRQ12) = 0;
	
	IR (ICU, IRQ11) = 0;
	IR (ICU, IRQ14) = 0; 

    //Enable the interrupts
    IEN(ICU, IRQ8 ) = 1; 
    IEN(ICU, IRQ9 ) = 1;
    IEN(ICU, IRQ12) = 1; 
	
	IEN(ICU, IRQ11) = 1;
	IEN(ICU, IRQ14) = 1;
}



#pragma interrupt (int_isr (vect = VECT(ICU, IRQ11)))
void int_isr (void){
	if(directionFlag == 0){
		if(waveDriveFlag == 1 || fullStepFlag == 1){
			count = count + 2;
		}
		else if(halfStepFlag == 1){
			count = count + 1;
		}
		
		if(count > 12){
			count = 1;
		}
		LED_Off(0);
	}
	else if(directionFlag == 1){
		if(waveDriveFlag == 1 || fullStepFlag == 1){
			count = count - 2;
		}
		else if(halfStepFlag == 1){
			count = count - 1;
		}
		if(count < 1){
			count = 12;
		}
		LED_Off(0);
	}
}

#pragma interrupt (dir_isr (vect = VECT(ICU, IRQ14)))
void dir_isr (void){
	directionFlag = 1 - directionFlag;
}

#pragma interrupt (sw1_isr (vect = VECT(ICU, IRQ8)))
void sw1_isr (void){
	waveDriveFlag = 1;
	fullStepFlag = 0;
	halfStepFlag = 0;
//	count = 1;
	if(count % 2 == 0){
		count = count + 1;
	}
	lcd_display(LCD_LINE3, "Wave Drive");
}


#pragma interrupt (sw2_isr (vect = VECT(ICU, IRQ9)))
void sw2_isr (void){
	waveDriveFlag = 0;
	fullStepFlag = 1;
	halfStepFlag = 0;
//	count = 1;
	if(count % 2 == 1){
		count = count + 1;
	}
	lcd_display(LCD_LINE3, "Full Step");
}

#pragma interrupt (sw3_isr (vect = VECT(ICU, IRQ12)))
void sw3_isr (void){
	waveDriveFlag = 0;
	fullStepFlag = 0;
	halfStepFlag = 1;
//	count = 1;
	lcd_display(LCD_LINE3, "Half Step");
}