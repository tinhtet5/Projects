/*********************
 *7 to GPS Module TX*
 *8 to GPS Module RX*
 *********************/

#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <LiquidCrystal.h>
#include <VirtualWire.h>

SoftwareSerial mySerial(7, 8);
TinyGPS gps;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);// initialize the library with the numbers of the interface pins

void gpsdump(TinyGPS &gps);
void printFloat(double f, int digits = 2);
int receive_pin = 6;

unsigned char pinLED1 = 9;
long lat, lon;
float flat, flon;
boolean GetGPS_state;
boolean SendGPS_state;
void setup()  
{
  
  pinMode(pinLED1, OUTPUT);
  
  lcd.begin(16, 2);// set up the LCD's number of columns and rows:
  Serial.begin(9600);// Oploen serial communications and wait for port to open:
  mySerial.begin(9600);// set the data rate for the SoftwareSerial port
  delay(1000);
  Serial.println("Studying MATLAB GUI Based LOCATION POINT SYSTM USING ARDUINO");
  Serial.print("Testing TinyGPS library v. "); Serial.println(TinyGPS::library_version());
  Serial.println();
  Serial.print("Sizeof(gpsobject) = "); 
  Serial.println(sizeof(TinyGPS));
  Serial.println(); 
  vw_set_rx_pin(receive_pin);
  vw_setup(2000);   // Bits per sec
  vw_rx_start();
}

void loop() // run over and over
{
  byte buf[VW_MAX_MESSAGE_LEN];
  byte buflen =VW_MAX_MESSAGE_LEN;
  bool newdata = false;
  unsigned long start = millis();
  // Every 5 seconds we print an update
  while (millis() - start < 5000) 
  {
    if (mySerial.available()) 
    
    {
      char c = mySerial.read();
      //Serial.print(c);  // uncomment to see raw GPS data
      if (gps.encode(c)) 
      {
        newdata = true;
        break;  // uncomment to print new data immediately!
      }
    }
  }
  
  if (newdata) 
  {
    digitalWrite(pinLED1,HIGH);
    Serial.println("Acquired Data");
    Serial.println("-------------");
    gpsdump(gps);
    Serial.println("-------------");
    Serial.println();
    
    
    
    if(vw_get_message(buf, &buflen))
    {
      if(buf[0]=='G')
      {
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("GPS Signal");
    
        lcd.setCursor(1,0);
        lcd.print("LAT:");
        lcd.setCursor(5,0);
        lcd.print(flat);
   
        lcd.setCursor(0,1);
        lcd.print(",LON:");
        lcd.setCursor(5,1);
        lcd.print(flon);
      }
      
      if(buf[0]=='S')
      {
        
        SendMessage();
        
      }
        delay(2000);
    }
    
    
  }
  else
  {digitalWrite(pinLED1,LOW);}
  
}

void gpsdump(TinyGPS &gps)
{
  unsigned long age;

  gps.f_get_position(&flat, &flon, &age);
  Serial.print("Lat/Long(float): "); printFloat(flat, 5); Serial.print(", "); printFloat(flon, 5);
   
}

void printFloat(double number, int digits)
{
  // Handle negative numbers
  if (number < 0.0) 
  {
     Serial.print('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  Serial.print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    Serial.print("."); 

  // Extract digits from the remainder one at a time
  while (digits-- > 0) 
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    Serial.print(toPrint);
    remainder -= toPrint;
  }
}

void SendMessage()
{

  Serial.println("Sending...");
  Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  Serial.println("AT+CMGS=\"+959969859417\"\r"); // Replace x with mobile number
  delay(1000);
  Serial.print("Lat/Long(float): "); printFloat(flat, 5); Serial.print(", "); printFloat(flon, 5);
  delay(100);
  Serial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
 
}
