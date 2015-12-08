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
* File Name     : cmt_periodic_multi.c
* Version       : 1.0
* Description   : Functions to provide periodic timers using a selected CMT channel
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 24.02.2012     1.00        First release
*******************************************************************************/

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include <stdint.h>

/* I/O Register and board definitions */
#include "platform.h"  /* Brings in the appropriate iodefines file. */
#include "cmt_periodic_multi.h"


/******************************************************************************
Local function declarations and local global variables
******************************************************************************/
static void (*cmt_callback_0)(void) = 0;
static void (*cmt_callback_1)(void) = 0;


/******************************************************************************
* Function name : cmt_callback_set
* Description 	: Receives a pointer to a function and sets it as the callback
*				  function for the CMT interrupt. The callback is
*				  executed when the CMT interrupt ISR is invoked on a compare match.
*
* Notes         : This callback will be executing within an ISR, holding off interrupts;  
*                 Be aware of the timing impact of this on the rest of the system 
*                 and keep it short!
*
* Arguments     : channel -
*               :    Which channel number to set
*               : func -
*               :    The pointer to the callback function.
* Return Value  : none
******************************************************************************/
void cmt_callback_set(uint8_t channel, void(*func)(void))
{
    switch (channel)
    {
        case 0:
            cmt_callback_0 = func; /* Assign the pointer to the callback function. */
        break;
        
        case 1:
            cmt_callback_1 = func; /* Assign the pointer to the callback function. */
        break;
        
        default:
        break;
    }       

} /* End of function cmt_callback_set() */


/******************************************************************************
* Function name : cmt_init
* Description   : Create one-shot timers based on PCLK / 3
* Arguments     : None
* Return Value  : None
******************************************************************************/
void cmt_init(void)
{
    #ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
    #endif
    
    /* Cancel CMT module stop state. */
    MSTP(CMT0) = 0;  
    
    #ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA500; /* Protect on  */
    #endif  

    /* Compare Match Timer Control Register (CMCR) 
    b15:b8  reserved:   Read/Write value always 0
    b7      reserved:   Read undef. Write value always 1
    b6      CMIE:       Compare Match Interrupt Enable                
    b5:b2   reserved:   Read/Write value always 0
    b1:b0   CKS:        clock select   3 = count on PCLK / 512  
    */
    CMT0.CMCR.WORD = 0x0003; /* Just set clock divisor for now. */
    CMT1.CMCR.WORD = 0x0003; 
        
    /* Compare Match Timer Counter (CMCNT)
    b15:b0  16-bit readable/writable up-counter to generate interrupt requests.
    */
    CMT0.CMCNT = 0x00;      /* Clear the count (default value). */
    CMT1.CMCNT = 0x00;      
        
    /* Compare Match Timer Constant Register (CMCOR))
    b15:b0  16-bit register sets the value for a compare match with CMCNT.
    */ 
    CMT0.CMCOR = 0xFFFF;    /* Set to max (default value). */
    CMT1.CMCOR = 0xFFFF;    

    IR (CMT0, CMI0);        /* Interrupt reset.  */ 
    IR (CMT1, CMI1);
    
    IPR(CMT0, CMI0) = 4;    /* Interrupt priority set. */
    IPR(CMT1, CMI1) = 4; 
           
    IEN(CMT0, CMI0) = 1;    /* Interrupt enable. */
    IEN(CMT1, CMI1) = 1;        
    
} /* End of function cmt_init() */


/******************************************************************************
* Function name : cmt_start
* Description   : Sets the compare match value then start the timer.
* Arguments     : channel -
*               :    Which channel number to start
*               : match_constant - 
*                     The counter compare match value.                                 .
* Return Value  : None
******************************************************************************/
void cmt_start(uint8_t channel, uint16_t match_constant)
{
    switch (channel)
    {
        case 0:
            CMT0.CMCNT          = 0x00;           /* Clear counter register. */
            CMT0.CMCOR          = match_constant; /* Set the value for count match interrupt. */
            CMT0.CMCR.BIT.CMIE  = 1;              /* Enable count match interrupt. */
            CMT.CMSTR0.BIT.STR0 = 1;              /* Start counting. */
        break;
        
        case 1:
            CMT1.CMCNT          = 0x00;           /* Clear counter register. */
            CMT1.CMCOR          = match_constant; /* Set the value for count match interrupt. */
            CMT1.CMCR.BIT.CMIE  = 1;              /* Enable count match interrupt. */
            CMT.CMSTR0.BIT.STR1 = 1;              /* Start counting. */
        break;
        
        default:
        break;
    }    

} /* End of function cmt_start() */


/******************************************************************************
* Function name : cmt_stop
* Description   : Stops the Compare match timer and disables its interrupt.
* Arguments     : channel -
*               :    Which channel number to stop
* Return Value  : None
******************************************************************************/
void cmt_stop (uint8_t channel)
{
    switch (channel)
    {
        case 0:
            CMT0.CMCR.BIT.CMIE  = 0;      /* Disable count match interrupt. */
            CMT.CMSTR0.BIT.STR0 = 0;      /* Stop counting. */
        break;
        
        case 1:
            CMT1.CMCR.BIT.CMIE  = 0;      /* Disable count match interrupt. */
            CMT.CMSTR0.BIT.STR1 = 0;      /* Stop counting. */
        break;
        
        default:
        break;
    }
} /* End of function cmt_stop() */


/******************************************************************************
* Function name: CMT0_isr
* Description  : ISR for CMT channel 0 interrupt. Optionally calls a callback 
*                function if one has been assigned.
* Arguments    : None
* Return value : None
******************************************************************************/
#pragma interrupt (CMT0_isr (vect = VECT(CMT0, CMI0)))
static void CMT0_isr (void) 
{
    if (cmt_callback_0)
    {
        cmt_callback_0();
    }
} /* End of CMT0_isr */

/******************************************************************************
* Function name: CMT1_isr
* Description  : ISR for CMT channel 1 interrupt. Optionally calls a callback 
*                function if one has been assigned.
* Arguments    : None
* Return value : None
******************************************************************************/
#pragma interrupt (CMT1_isr (vect = VECT(CMT1, CMI1)))
static void CMT1_isr (void) 
{
    if (cmt_callback_1)
    {
        cmt_callback_1();
    }
} /* End of CMT1_isr */


/*******************************************************************************
End of file: cmt_periodic.c
*******************************************************************************/