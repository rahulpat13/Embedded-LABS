/******************************************************************************
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
******************************************************************************/
/******************************************************************************
* File Name    : riic_master_main.h
* Version      : 1.00  
* Device(s)    : Renesas RX600 family
* Description  : 
******************************************************************************/
/******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 06.02.2012 1.00    First Release
******************************************************************************/

#ifndef RIIC_MASTER_MAIN_H   /* Multiple inclusion prevention. */
#define RIIC_MASTER_MAIN_H

#define MASTER_IIC_ADDRESS_LO	0x20	/* Master address low bits [7:0] */
#define MASTER_IIC_ADDRESS_HI	0x00	/* Master address high bits [2:0] */
#define RIIC_CHANNEL            CHANNEL_0
#define RW_BIT                  0x01     /* Read / #write bit value. 1 = read. */


/******************************************************************************
Public functions prototypes
******************************************************************************/


#endif /* Multiple inclusion prevention. */