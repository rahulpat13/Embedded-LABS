#define  LED4                   PORTD.PODR.BIT.B5
#define  LED5                   PORTE.PODR.BIT.B3
#define  LED6                   PORTD.PODR.BIT.B2
#define  LED7                   PORTE.PODR.BIT.B0
#define  LED8                   PORTD.PODR.BIT.B4
#define  LED9                   PORTE.PODR.BIT.B2
#define  LED10                  PORTD.PODR.BIT.B1
#define  LED11                  PORTD.PODR.BIT.B7
#define  LED12                  PORTD.PODR.BIT.B3
#define  LED13                  PORTE.PODR.BIT.B1
#define  LED14                  PORTD.PODR.BIT.B0
#define  LED15                  PORTD.PODR.BIT.B6

#define  LED_ON                 0
#define  LED_OFF                1

void  			   LED_Init          (void);
void        LED_On            (int  led);
void        LED_Off           (int  led);

void phase1_ON(void);
void phase2_ON(void);
void phase3_ON(void);
void phase4_ON(void);
void phase5_ON(void);
void phase6_ON(void);

void phase1_OFF(void);
void phase2_OFF(void);
void phase3_OFF(void);
void phase4_OFF(void);
void phase5_OFF(void);
void phase6_OFF(void);
