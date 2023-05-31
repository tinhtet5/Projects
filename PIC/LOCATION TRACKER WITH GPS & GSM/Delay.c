#include "Delay.h"

	
void delay_ms(int MS)
{
     do 
     {
		delay_us(990);
     }

	  while(--MS);
}