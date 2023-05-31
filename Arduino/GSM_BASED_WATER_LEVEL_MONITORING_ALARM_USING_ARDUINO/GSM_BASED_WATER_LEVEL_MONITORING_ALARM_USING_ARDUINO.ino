                  /*Connect 5VT to D9 and 5VR to D10*/
//* RX is digital pin 10 (connect to TX of other device)
//* TX is digital pin 11 (connect to RX of other device)
  
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10,11);

int SensorInput1 = 5;
int SensorInput2 = 6;
int SensorInput3 = 7;
int pinLED1 = 2;
int pinLED2 = 3;
int pinLED3 = 4;
int Flag1 = 0;
int Flag2 = 0;
int Flag3 = 0;
int SensorState1;
int SensorState2;
int SensorState3;

void setup()
{
  pinMode(SensorInput1, INPUT);
  pinMode(SensorInput2, INPUT); 
  pinMode(SensorInput3, INPUT);
  pinMode(pinLED1,OUTPUT);
  pinMode(pinLED2,OUTPUT);
  pinMode(pinLED3,OUTPUT);
  mySerial.begin(9600);
  Serial.begin(9600);
  Serial.println("GSM SIM900A BEGIN");
  delay(100);
}

void loop()
{
   SensorState1 = digitalRead(SensorInput1);
   if (SensorState1 == HIGH && Flag1 == 0)
  {
    Flag1 = 1;
    Serial.println("Flag1 is set");
    digitalWrite(pinLED1,HIGH);
    SendMessage("AT+CMGS=\"+959402512293\"\r", "Water level 1");
    delay(1000);
    
        
  }else if (SensorState1 == LOW && Flag1 == 1)
  {
    Flag1 = 0;
    Serial.println("Flag1 clear");
    digitalWrite(pinLED1,LOW);
    SendMessage("AT+CMGS=\"+959402512293\"\r", "Water falls below level 1");
    delay(1000);
    
  }
  

  SensorState2 = digitalRead(SensorInput2);
  if (SensorState2 == HIGH && SensorState1 == HIGH && Flag2 == 0)
  {
    Flag2 = 1;
    Serial.println("Flag2 is set");
    digitalWrite(pinLED2,HIGH);
    SendMessage("AT+CMGS=\"+959402512293\"\r", "Water level 2");
    delay(1000);
    
    
    
  }
  else if (SensorState2 == LOW && Flag2 == 1)
  {
    Flag2 = 0;
    Serial.println("Flag2 clear");
    digitalWrite(pinLED2,LOW);
    SendMessage("AT+CMGS=\"+959402512293\"\r", "Water falls below level 2 ");
    delay(1000);
    
   
  }

  SensorState3 = digitalRead(SensorInput3);
  if (SensorState1 == HIGH && SensorState2 == HIGH && SensorState3 == HIGH && Flag3 == 0)
  {
    Flag3 = 1;
    Serial.println("Flag3 is set");
    digitalWrite(pinLED3,HIGH);
    SendMessage("AT+CMGS=\"+959402512293\"\r", "Water level 3");
    delay(1000);
    
    
    
  }
  else if (SensorState3 == LOW && Flag3 == 1)
  {
    Flag3 = 0;
    Serial.println("Flag3 clear");
    digitalWrite(pinLED3,LOW);
    SendMessage("AT+CMGS=\"+959402512293\"\r", "Water falls below level 3 ");
    delay(1000);
    
   
  }

if (mySerial.available()>0)
 Serial.write(mySerial.read());

}


void SendMessage(String phNumber, String Msg)
{
  Serial.println("Sending...");
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println(phNumber); // Replace x with mobile number
  delay(1000);
  mySerial.println(Msg);// The SMS text you want to send
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}
