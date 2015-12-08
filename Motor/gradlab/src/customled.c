#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "platform.h"
#include "customled.h"




void  LED_Init(void)
{
    PORTD.PDR.BIT.B0 = 1;                                       //Select port function.                                
    PORTD.PDR.BIT.B1 = 1;
    PORTD.PDR.BIT.B2 = 1;
    PORTD.PDR.BIT.B3 = 1;
    PORTD.PDR.BIT.B4 = 1;
    PORTD.PDR.BIT.B5 = 1;
    PORTD.PDR.BIT.B6 = 1;
    PORTD.PDR.BIT.B7 = 1;
    PORTE.PDR.BIT.B0 = 1;
    PORTE.PDR.BIT.B1 = 1;
    PORTE.PDR.BIT.B2 = 1;
    PORTE.PDR.BIT.B3 = 1;

    LED_Off(0);                                                 // Turn OFF all LEDs.                                   
}

void  LED_On (int  led)
{
    switch (led) {
        case 0:
            LED4  = LED_ON;
            LED5  = LED_ON;
            LED6  = LED_ON;
            LED7  = LED_ON;
            LED8  = LED_ON;
            LED9  = LED_ON;
            LED10 = LED_ON;
            LED11 = LED_ON;
            LED12 = LED_ON;
            LED13 = LED_ON;
            LED14 = LED_ON;
            LED15 = LED_ON;
            break;

        case 4:
            LED4  = LED_ON;
            break;

        case 5:
            LED5  = LED_ON;
            break;

        case 6:
            LED6  = LED_ON;
            break;

        case 7:
            LED7  = LED_ON;
            break;

        case 8:
            LED8  = LED_ON;
            break;

        case 9:
            LED9  = LED_ON;
            break;

        case 10:
            LED10 = LED_ON;
            break;

        case 11:
            LED11 = LED_ON;
            break;

        case 12:
            LED12 = LED_ON;
            break;

       case 13:
            LED13 = LED_ON;
            break;

       case 14:
            LED14 = LED_ON;
            break;

       case 15:
            LED15 = LED_ON;
            break;

       default:
            break;
    }
}


void  LED_Off(int  led)
{
    switch (led) {
        case 0:
            LED4  = LED_OFF;
            LED5  = LED_OFF;
            LED6  = LED_OFF;
            LED7  = LED_OFF;
            LED8  = LED_OFF;
            LED9  = LED_OFF;
            LED10 = LED_OFF;
            LED11 = LED_OFF;
            LED12 = LED_OFF;
            LED13 = LED_OFF;
            LED14 = LED_OFF;
            LED15 = LED_OFF;
            break;

        case 1:
            LED4  = LED_OFF;
            break;

        case 2:
            LED5  = LED_OFF;
            break;

        case 3:
            LED6  = LED_OFF;
            break;

        case 4:
            LED7  = LED_OFF;
            break;

        case 5:
            LED8 = LED_OFF;
            break;

        case 6:
            LED9  = LED_OFF;
            break;

        case 7:
            LED10 = LED_OFF;
            break;

        case 8:
            LED11 = LED_OFF;
            break;

        case 9:
            LED12 = LED_OFF;
            break;

       case 10:
            LED13 = LED_OFF;
            break;

       case 11:
            LED14 = LED_OFF;
            break;

       case 12:
            LED15 = LED_OFF;
            break;

       default:
            break;
    }
}

void phase1_ON(void){
	LED4  = LED_ON;
	LED5  = LED_ON;
}

void phase2_ON(void){
	LED6  = LED_ON;
	LED7  = LED_ON;
}

void phase3_ON(void){
	LED8  = LED_ON;
	LED9  = LED_ON;
}

void phase4_ON(void){
	LED10  = LED_ON;
	LED11  = LED_ON;
}

void phase5_ON(void){
	LED12  = LED_ON;
	LED13  = LED_ON;
}

void phase6_ON(void){
	LED14  = LED_ON;
	LED15  = LED_ON;
}

void phase1_OFF(void){
    LED4  = LED_OFF;
    LED5  = LED_OFF;
}

void phase2_OFF(void){
    LED6  = LED_OFF;
    LED7  = LED_OFF;
}

void phase3_OFF(void){
    LED8  = LED_OFF;
    LED9  = LED_OFF;
}

void phase4_OFF(void){
    LED10  = LED_OFF;
    LED11  = LED_OFF;
}

void phase5_OFF(void){
    LED12  = LED_OFF;
    LED13  = LED_OFF;
}

void phase6_OFF(void){
    LED14  = LED_OFF;
    LED15  = LED_OFF;
}