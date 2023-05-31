

#define MHz *1e6
#ifndef FOSC
#define FOSC 20MHz
#endif
#define delay_us(x)   {unsigned int cycles;\
					   cycles=(unsigned int) ((x*1e-6)/(9.0*4/(FOSC)));\
                       while(--cycles);\
					  }
					  
void delay_ms(int MS);					  	