#include <stdint.h>
#include <stdbool.h>
#include <machine.h>

#include "platform.h" // I/O Register and board definitions 

static void sw1_isr (void);
static void sw2_isr (void);
static void sw3_isr (void);

extern int count;
extern int waveDriveFlag;
extern int fullStepFlag;
extern int halfStepFlag;
volatile unsigned int directionFlag = 0;

void interrupt_init(void){
	//Switch  Port    IRQ
    //------  ----    ----
    //GPIO     PE.5    IRQ8 (Direction)
    //GPIO     PE.6    IRQ9 (Drive)

	//Make the pins inputs
	PORT4.PDR.BIT.B0 = 0; //Direction
	PORT4.PDR.BIT.B1 = 0; //Drive
	PORT4.PDR.BIT.B2 = 0;

	//Set P40, P41, and P44 for IRQx function
	PORT4.PMR.BYTE  = 0x00;    // All GPIO for now 
    MPC.P40PFS.BYTE = 0x40;    // P40 is used as IRQ pin 
    MPC.P41PFS.BYTE = 0x40;    // P40 is used as IRQ pin 
	MPC.P42PFS.BYTE = 0x40;    // P40 is used as IRQ pin

    //Set IRQ type (falling edge for direction and rising edge for drive pin)
    ICU.IRQCR[ 8].BIT.IRQMD = 0x01; 
    ICU.IRQCR[ 9].BIT.IRQMD = 0x01;
    ICU.IRQCR[12].BIT.IRQMD = 0x01; 

    //Set interrupt priority 
    IPR(ICU, IRQ8 ) = 0x03; 
    IPR(ICU, IRQ9 ) = 0x03;
    IPR(ICU, IRQ12) = 0x03;

    //Clear any pending interrupts
    IR (ICU, IRQ8 ) = 0;
    IR (ICU, IRQ9 ) = 0;
    IR (ICU, IRQ12) = 0; 

    //Enable the interrupts
    IEN(ICU, IRQ8 ) = 1; 
    IEN(ICU, IRQ9 ) = 1;
    IEN(ICU, IRQ12) = 1; 
}

/*
#pragma interrupt (sw1_isr (vect = VECT(ICU, IRQ8)))
void sw1_isr (void){
	directionFlag = 1 - directionFlag;
}
*/
/*
#pragma interrupt (sw1_isr (vect = VECT(ICU, IRQ8)))
void sw1_isr (void){
	waveDriveFlag = 1;
	fullStepFlag = 0;
	halfStepFlag = 0;
	if(directionFlag == 0){
		count = count + 1;
		if(count > 6){
			count = 1;
		}
	}
	else if(directionFlag == 1){
		count = count - 1;
		if(count < 1){
			count = 6;
		}
	}
}
*/
#pragma interrupt (sw2_isr (vect = VECT(ICU, IRQ9)))
void sw2_isr (void){
	waveDriveFlag = 0;
	fullStepFlag = 1;
	halfStepFlag = 0;
	if(directionFlag == 0){
		count = count + 1;
		if(count > 6){
			count = 1;
		}
	}
	else if(directionFlag == 1){
		count = count - 1;
		if(count < 1){
			count = 6;
		}
	}
}


#pragma interrupt (sw1_isr (vect = VECT(ICU, IRQ8)))
void sw1_isr (void){
	waveDriveFlag = 0;
	fullStepFlag = 0;
	halfStepFlag = 1;
	
	if(directionFlag == 0){
		count = count + 1;
		if(count > 12){
			count = 1;
		}
	}
	else if(directionFlag == 1){
		count = count - 1;
		if(count < 1){
			count = 12;
		}
	}
}