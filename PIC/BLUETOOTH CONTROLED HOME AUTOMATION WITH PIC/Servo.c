/*Servo Setup*/

#include<pic.h>
#include"Delay.h"

void Rotation0(void) //0 Degree
{
 unsigned int i;
 for(i=0;i<50;i++)
 	{
		RD1 = 1;
		delay_us(800); // pulse of 800us
		RD1 = 0;
		delay_us(19200);
 	}
}

void Rotation90(void) //90 Degree
{
 unsigned int i;
 for(i=0;i<50;i++)
	{
		RD1 = 1;
		delay_us(1500); // pulse of 1500us
		RD1 = 0;
		delay_us(18500);
	}
}

void Rotation180(void) //180 Degree
{
 unsigned int i;
 for(i=0;i<50;i++)
	{
		RD1 = 1;
		delay_us(2200); // pulse of 2200us
		RD1 = 0;
		delay_us(17800);
	}
}

