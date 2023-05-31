                  /*Connect 5VT to D9 and 5VR to D10*/
//* RX is digital pin 10 (connect to TX of other device)
//* TX is digital pin 11 (connect to RX of other device)
  
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10,11);
char msg;
unsigned char SensorInput1 = 9;
unsigned char SensorInput2 = 8;
boolean Flag1 = 0;
boolean Flag2 = 0;
boolean SensorState1;
boolean SensorState2;

void setup()
{
  pinMode(SensorInput1, INPUT);
  pinMode(SensorInput2, INPUT); 
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
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
    digitalWrite(7,HIGH);
    SendMessage1();
    
    
  }else if (SensorState1 == LOW && Flag1 == 1)
  {
    Flag1 = 0;
    Serial.println("Flag1 clear");
    digitalWrite(7,LOW);
    SendMessage2();
  }
  

  SensorState2 = digitalRead(SensorInput2);
  if (SensorState2 == HIGH && SensorState1 == HIGH && Flag2 == 0)
  {
    Flag2 = 1;
    Serial.println("Flag2 is set");
    digitalWrite(6,HIGH);
    SendMessage3();
    
  }
  else if (SensorState2 == LOW && Flag2 == 1)
  {
    Flag2 = 0;
    Serial.println("Flag2 clear");
    digitalWrite(6,LOW);
    SendMessage4();
  }

if (mySerial.available()>0)
 Serial.write(mySerial.read());

}
void SendMessage1()
{
  Serial.println("Sending...");
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"959253243935\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("Water level 1");// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void SendMessage2()
{
  Serial.println("Sending...");
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"959253243935\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("Water fall below level 1");// The SMS text you want to send
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void SendMessage3()
{
  Serial.println("Sending...");
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"959253243935\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("Water level 2 ");// The SMS text you want to send
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void SendMessage4()
{
  Serial.println("Sending...");
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+959253243935\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("Water fall below level 2 ");// The SMS text you want to send
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

