#include <VirtualWire.h>
int x1=0;
int y1=0;
char mss[10];
unsigned char GetGPS = 9; //connect with pin 12
unsigned char SendGPS = 8; //connect with pin 11
boolean GetGPS_state=0;
boolean SendGPS_state=0;

void setup() 
{
  pinMode(GetGPS,INPUT);
  pinMode(SendGPS,INPUT);
  vw_setup(2000);
  vw_set_tx_pin(7);
  Serial.begin(9600);
  Serial.println(" Data Transmitting : ");
 }

void loop() {
  GetGPS_state = digitalRead(GetGPS);
  if(GetGPS_state == HIGH)
  {
     mss[0]='G';
     vw_send((uint8_t*)mss,strlen(mss));
     vw_wait_tx();
     Serial.println(mss[0]);
  }

  SendGPS_state = digitalRead(SendGPS);
  if(SendGPS_state == HIGH)
  {
     mss[0]='S';
     vw_send((uint8_t*)mss,strlen(mss));
     vw_wait_tx();
     Serial.println(mss[0]);
  }
}
