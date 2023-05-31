/*CCP_PWM mode*/

#include <pic.h>
	
void Init_CCP_PWM(void)
{
	CCP2CON = 0x0c;  // PWM mode
	CCP1CON = 0x0c;

	PR2 = 255;
	T2CKPS0 = 0;  // Timer 2 prescaler = 16
	T2CKPS1 = 1;
	TMR2ON = 1;
	
	CCPR2L = 0; 
	CCPR1L = 125;

}	
	