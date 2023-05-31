/*Home Automation Main Code*/

#include<pic.h>
#include"Delay.h"
#include"Init_PIC.h"
#include"Servo.h"
#include"CCP_PWM.h"
 #include"usart.h"

__CONFIG( MCLRDIS & WDTDIS & PWRTEN & HS);

unsigned char recdata;

void main(void)
{
 Init_PIC();
 Init_CCP_PWM();
 Init_USART();

 while(1)
  {
 	if(OERR) // check for over run error 
     {
        CREN = 0;
        CREN = 1; //Reset CREN
     }
  }
}



void interrupt bluetooth(void)
{
 if(RCIF)
	{
		RCIF = 0;
		recdata = RCREG;
		switch(recdata)
		{
			case 'a':
			RD0=1;
			break;
	
			case 'b':
			RD0=0;
			break;
		
			case 'c':
			Rotation0();
			break;
		
			case 'd':
			Rotation90();
			break;

			case 'e':
			Rotation180();
			break;

			case '1':
			CCPR2L = 0;
			break;

			case '2':
			CCPR2L = 50;
			break;

			case '3':
			CCPR2L = 100;
			break;

			case '4':
			CCPR2L = 175;
			break;

			case '5':
			CCPR2L = 255;
			break;

			case '6':
			CCPR2L = 0;
			break;

			case '7':
			CCPR2L = 70;
			break;

			case '8':
			CCPR2L = 100;
			break;

			case '9':
			CCPR2L = 175;
			break;

			case '0':
			CCPR2L = 255;
			break;
	
			default:
			break;	
		}
	} 
}

