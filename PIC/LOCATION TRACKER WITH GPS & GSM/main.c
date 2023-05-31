/*Home Automation Main Code*/

#include<pic.h>
#include"Delay.h"
#include"Init_PIC.h"
#include"usart.h"

__CONFIG( MCLRDIS & WDTDIS & PWRTEN & HS);

double recdata;

void main(void)
{
 Init_PIC();
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



void interrupt gps(void)
{
 if(RCIF)
	{
		RCIF = 0;
		recdata = RCREG;
		if(RC0==0)
		{
		Send_Char(recdata);
		}
	} 
}

