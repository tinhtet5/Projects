#include <pic.h>

void Init_PIC(void)
{
	ANSEL  = 0;  // Configure AN pins as digital I/O
	ANSELH = 0;
	TRISB = 0;
	TRISD = 0x00;
	PORTD = 0;
	TRISC = 0b11110000;
	PORTC = 0;	
			 
}