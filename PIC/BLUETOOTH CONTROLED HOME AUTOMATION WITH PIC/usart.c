/*USART Register Configuration*/

#include <pic.h>

#define FOSC 20e6
#define BAUDRATE 9600
#define  CALC_BAUDRATE  ((FOSC/BAUDRATE)/64)-1

void Init_USART(void)
{
	//CSRC = 1;

//Set 8-bit reception and transmission//
	TX9 = 0;
	RX9 = 0;

//Enable transmission and reception//
	TXEN = 1;
	CREN = 1;

 //Turn on Asyc. Serial Port//
	SYNC = 0;
	SPEN = 1;

//Set the baud rate//
	SPBRG = CALC_BAUDRATE;
	BRGH = 0;  /*BRGH: High Baud Rate Select bit
                Asynchronous mode:
				1 = High speed
				0 = Low speed
				Synchronous mode:
				Unused in this mode*/
	BRG16 = 0;  //8-bit Baud Rate Generator is used//


	TRMT = 0;  /*TRMT: Transmit Shift Register Status bit
					1 = TSR empty
					0 = TSR full*/


	SENDB = 0;  /*SENDB: Send Break Character bit
					Asynchronous mode:
					1 = Send Sync Break on next transmission (cleared by hardware upon completion)
					0 = Sync Break transmission completed
					Synchronous mode:
					Don’t care*/
	SREN = 1;
	
//Enable interrupts for Tx. and Rx.//
	TXIE = 1;
	RCIE = 1;
	RCIF = 0; // clear rec flag//

//Enable global and ph. interrupts//
    GIE = 1;
    PEIE= 1;

}

/*usart transmit*/
//void Send_Char(char value)
//{
//	TXREG =value;
//	while(!TRMT); 
//}



