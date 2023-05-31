#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.


int SensorS1 = A0;
int SensorS2 = A1;
int SensorS4 = A2;
int SensorS5 = A3;
int Down_Limit = A5;
int motorA1 = 7;
int motorA2 = 8;
int motorB1 = 4;
int motorB2 = 5;
int mSpeed = 6;

int motorC1 = 2; //red0
int motorC2 = 3; //black1

int SensorValueS1 = 0;
int SensorValueS2 = 0;
int SensorValueS4 = 0;
int SensorValueS5 = 0;
int Down_Limit_State = 0;
int flag = 0;

void setup() {
  Serial.begin(9600);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  pinMode(SensorS1, INPUT);
  pinMode(SensorS2, INPUT);
  pinMode(SensorS4, INPUT);
  pinMode(SensorS5, INPUT);
  pinMode(Down_Limit, INPUT);
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(motorC1, OUTPUT);
  pinMode(motorC2, OUTPUT);
  pinMode(mSpeed, OUTPUT);

  //pinMode(mSpeed3, OUTPUT);
  ForkLiftDown();
  analogWrite(mSpeed, 255);

}

void loop() {
  SensorValueS1 = digitalRead(SensorS1);
  SensorValueS2 = digitalRead(SensorS2);
  SensorValueS4 = digitalRead(SensorS4);
  SensorValueS5 = digitalRead(SensorS5);
  LineFollow();


  if (SensorValueS1 == LOW && SensorValueS2 == LOW && SensorValueS4 == LOW && SensorValueS5 == LOW) {
    Stop();
    // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent())
    {
      return;
    }
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
    {
      return;
    }
    //Show UID on serial monitor
    Serial.print("UID tag :");
    String content = "";
    byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println();
    Serial.print("Message : ");
    content.toUpperCase();

    if (content.substring(1) == "29 25 00 6F")
    {
      delay(500);
      ForkLiftUp();
      delay(500);
      Forward();
      delay(500);
      SensorValueS2 = digitalRead(SensorS2);
      SensorValueS4 = digitalRead(SensorS4);
      while (SensorValueS2 == HIGH || SensorValueS4 == HIGH) {
        LineFollow();
        SensorValueS2 = digitalRead(SensorS2);
        SensorValueS4 = digitalRead(SensorS4);

      }

      Stop();
      delay(1000);
      Forward();
      delay(1200);
      Stop();
      delay(1000);
      ForkLiftDown();
      delay(1000);
      Backward();
      delay(500);
      Stop();
      delay(500);
      //Forward();
      //delay(200);
      SensorValueS2 = digitalRead(SensorS2);
      SensorValueS4 = digitalRead(SensorS4);
      while (SensorValueS2 == HIGH || SensorValueS4 == HIGH) {
        LineFollow();
        SensorValueS2 = digitalRead(SensorS2);
        SensorValueS4 = digitalRead(SensorS4);

      }
      Stop();
      delay(500);
      Forward();
      delay(100);
    }

    else if (content.substring(1) == "EA 56 38 2E")
    {
      delay(500);
      ForkLiftUp();
      delay(500);
      Forward();
      delay(500);
      SensorValueS2 = digitalRead(SensorS2);
      SensorValueS4 = digitalRead(SensorS4);
      while (SensorValueS2 == HIGH || SensorValueS4 == HIGH) {
        LineFollow();
        SensorValueS2 = digitalRead(SensorS2);
        SensorValueS4 = digitalRead(SensorS4);

      }

      Stop();
      delay(1000);
      Forward();
      delay(200);
      SensorValueS2 = digitalRead(SensorS2);
      SensorValueS4 = digitalRead(SensorS4);
      while (SensorValueS2 == HIGH || SensorValueS4 == HIGH) {
        LineFollow();
        SensorValueS2 = digitalRead(SensorS2);
        SensorValueS4 = digitalRead(SensorS4);

      }
      Stop();
      delay(1000);
      Forward();
      delay(1200);
      Stop();
      delay(1000);
      ForkLiftDown();
      delay(1000);
      Backward();
      delay(500);
      Stop();
      delay(500);
    }

    else   {
      Stop();
      delay(3000);
    }
  }
}

void LineFollow() {

  if (SensorValueS2 == HIGH && SensorValueS4 == HIGH) {
    Forward();
  }

  if ( SensorValueS2 == HIGH && SensorValueS4 == LOW ) {
    TurnRight();
  }

  if ( SensorValueS2 == LOW && SensorValueS4 == HIGH ) {
    TurnLeft();
  }


  //  if ( SensorValueS1 == LOW && SensorValueS2 == LOW && SensorValueS4 == LOW && SensorValueS5 == LOW) {
  //    Stop();
  //  }
}

void InitialForward() {
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
  analogWrite(mSpeed, 200);

}

void Forward() {
  if (flag == 0)
  {
    InitialForward();
    delay(100);
    flag = 1;
  }
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
  analogWrite(mSpeed, 100);

}

void Backward() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);
  analogWrite(mSpeed, 150);
}

void TurnRight() {
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);
  analogWrite(mSpeed, 200);

}

void TurnLeft() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
  analogWrite(mSpeed, 200);
}

void Stop() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
  analogWrite(mSpeed , 0);
  flag = 0;
}



void ForkLiftUp() {

  digitalWrite(motorC1, HIGH);
  digitalWrite(motorC2, LOW);
  analogWrite(mSpeed, 175);
  delay(1000);
  digitalWrite(motorC1, LOW);
  digitalWrite(motorC2, LOW);
  analogWrite(mSpeed, 0);

}

void ForkLiftDown() {

  Down_Limit_State = digitalRead(Down_Limit);
  do {
    digitalWrite(motorC1, LOW);
    digitalWrite(motorC2, HIGH);
    analogWrite(mSpeed, 175);
    Down_Limit_State = digitalRead(Down_Limit);
  } while (!Down_Limit_State);

  digitalWrite(motorC1, LOW);
  digitalWrite(motorC2, LOW);
  analogWrite(mSpeed, 0);
  delay(1000);
}


