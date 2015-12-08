/*
 * LAB 2 code by Raj Sheth and Rahul Patil
 *
 * This program is used to glow LEDs corresponding to the input voltage
 * controlled using a potentiometer connected to a TI MSP430.
 * The analog input from the potentiometer is taken at port P1.0
 * the digital output is at pins P1.2-7 and P2.0,1,2 and 5.
 * The analog input is converted to digital by the on-chip 10 bit ADC
 * and the corresponding digital value is stored in the register ADC10MEM.
 * Since the ADC is 10 bit, there are a total of 2^10 = 1023 steps.
 * We compare this value to glow 10 LEDs as follows:
 * 0 to 93 : No LEDs
 * 93 to 186: LED1
 * 186 to 279: LED2
 * 279 to 372: LED3
 * 372 to 465: LED4
 * 465 to 558: LED5
 * 558 to 651: LED6
 * 651 to 744: LED7
 * 744 to 837: LED8
 * 837 to 930: LED9
 * 930 to 1023: LED10
 * We have Converted the above decimal values to HEX as per appropriate Voltage values
 * and used them as thresholds in the program
 */

#include <msp430.h>

void main(void){
  WDTCTL = WDTPW + WDTHOLD;                  // Stop Watch Dog Timer
  ADC10CTL1 = INCH_0 + CONSEQ_3;            // Select Channel A0, repeat multi channel
  ADC10CTL0 = ADC10SHT_3 + MSC + ADC10ON;  // Select Sample and Hold Time (64 cycles), Enable Multiple sample conversion, Turn on the ADC10
  ADC10AE0 = 0x01;                        // Analog Input Enable
  P1DIR |= 0xFC;						 // Set P1.2,P1.3,P1.4,P1.5,P1.6,P1.7 as OUTPUTS
  P2DIR |= 0X27;						// Set P2.0,P2.1,P2.2,P2.5 as OUTPUTS

  for (;;){
    ADC10CTL0 |= ENC + ADC10SC;             // Enable Conversion and Start Conversion

    if (ADC10MEM >0X00 && ADC10MEM <= 0X5D ){					// 0V < Input Voltage <= 0.3V, 0 LEDs
    	P2OUT &= 0X00;
    	P1OUT &= 0X00;
    }

    else if (ADC10MEM > 0x5D && ADC10MEM <= 0XBA ){             // 0.3V < Input Voltage <= 0.6V, Glow 1 LED
    	P1OUT &= 0X00;
		P2OUT &= 0X00;
		P1OUT &= 0x04;
  	}

    else if (ADC10MEM > 0xBA && ADC10MEM <= 0x117){				// 0.6V < Input Voltage <= 0.9V, Glow 2 LEDs
	P1OUT &= 0X00;
	P2OUT &= 0X00;
	P1OUT |= 0x0C;
    }

    else if(ADC10MEM > 0x117 && ADC10MEM <= 0X174){				// 0.9V < Input Voltage <= 1.2V, Glow 3 LEDs
	P1OUT &= 0X00;
	P2OUT &= 0X00;
	P1OUT |= 0x1C;
    }

    else if(ADC10MEM > 0x174 && ADC10MEM <= 0X1D1){				// 1.2V < Input Voltage <= 1.5V, Glow 4 LEDs
	P1OUT &= 0X00;
	P2OUT &= 0X00;
	P1OUT |= 0x3C;
    }

    else if(ADC10MEM > 0x1D1 && ADC10MEM <= 0X22E){				// 1.5V < Input Voltage <= 1.8V, Glow 5 LEDs
	P1OUT &= 0X00;
	P2OUT &= 0X00;
	P1OUT |= 0x7C;
	}

    else if(ADC10MEM > 0x22E && ADC10MEM <= 0X28B){				// 1.8V < Input Voltage <= 2.1V, Glow 6 LEDs
	P1OUT &= 0X00;
	P2OUT &= 0X00;
	P1OUT |= 0xFC;
	}

    else if(ADC10MEM > 0x28B && ADC10MEM <= 0X2E8){				// 2.1V < Input Voltage <= 2.4V, Glow 7 LEDs
	P2OUT &= 0X00;
	P1OUT |= 0xFC;
	P2OUT |= 0X01;
	}

    else if(ADC10MEM > 0x2E8 && ADC10MEM <= 0X345){				// 2.4V < Input Voltage <= 2.7V, Glow 8 LEDs
	P2OUT &= 0X00;
	P1OUT |= 0xFC;
	P2OUT |= 0X03;
	}

    else if(ADC10MEM > 0x345 && ADC10MEM <= 0X3A2){				// 2.7V < Input Voltage <= 3.0V, Glow 9 LEDs
	P2OUT &= 0X00;
	P1OUT |= 0xFC;
	P2OUT |= 0X07;
	}

    else if(ADC10MEM > 0x3A2 && ADC10MEM <= 0X3FF){				// 3V < Input Voltage <= 3.3V, Glow 10 LEDs
	P2OUT &= 0X00;
	P1OUT |= 0xFC;
	P2OUT |= 0X27;
	}

  } // close for loop

} //close main
