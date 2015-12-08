/*
File Name: customADC.h
Description: This is the header file for customADC.c
			 It contains all the macro definitions and include files
			 and function prototypes used in customADC.c
*/

//Macro Definitions
#define AN0 0x0000
#define AN1 0x0001
#define AN2 0x0004
#define AN3 0x0008
#define AN4 0x0010
#define AN5 0x0020
#define AN6 0x0040
#define AN7 0x0080
#define AN8 0x0100
#define AN9 0x0200
#define AN10 0x0400
#define AN11 0x0800
#define AN12 0x1000
#define AN13 0x2000
#define AN14 0x4000
#define AN15 0x8000
#define Vpref 3.3		
#define Vnref 0
#define N12 4096
#define N10 1024

//Include files
#include "platform.h"

//Function Prototypes
void adcInit(void);			// This function initializes the ADC for us
int adcRead(int channel);   // This function will read an analog value from the specified channel
float convertADC(int adcOut); //This function will convert the analog value to corresponding voltage