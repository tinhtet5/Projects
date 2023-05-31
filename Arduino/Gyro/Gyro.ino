#include <Encoder.h>
#include <TaskScheduler.h>

#include "Wire.h"
#include <MPU6050_light.h>

#include <math.h>

#include <PIDController.h>

// We declare the function that we are going to use
void UltrasonicTask();

// We create the Scheduler that will be in charge of managing the tasks
Scheduler sensorTimer;

// We create the task indicating that it runs every 500 milliseconds, forever, and call the led_blink function
Task Ultrasonic1(500, TASK_FOREVER, &UltrasonicTask);


Encoder rightencoder(3, 2);
Encoder leftencoder(19, 18);

MPU6050 mpu(Wire);

#define leftMotorA 22  // Left Motor
#define leftMotorB 23
#define rightMotorA 24  // right Motor
#define rightMotorB 25
#define leftMotorEN 5   // left pwm
#define rightMotorEN 4  // right pwm

#define serialOut mpu.getAngleZ()

int numberOfCoordinate = 7;
int coordinate[7][2] = { { 0, 0 },
                         { 300, 0 },
                         { 300, 300 },
                         { 600, 300 },
                         { 600, 0 },
                         { 900, 0 },
                         { 900, 600 } };

struct direction {
  int distance;
  float angle;
};

direction dir = { 0, 0 };

PIDController pid_LeftMotor;
PIDController pid_RightMotor;

unsigned int leftMotorSpeed, rightMotorSpeed = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(leftMotorEN, OUTPUT);
  pinMode(leftMotorA, OUTPUT);
  pinMode(leftMotorB, OUTPUT);
  pinMode(rightMotorEN, OUTPUT);
  pinMode(rightMotorA, OUTPUT);
  pinMode(rightMotorB, OUTPUT);

  pid_LeftMotor.begin();  // initialize the PID instance
  // pid_LeftMotor.setpoint(1024);  // The "goal" the PID controller tries to "reach"
  pid_LeftMotor.tune(1, 1.2, 0.2);  // Tune the PID, arguments: kP, kI, kD
  pid_LeftMotor.limit(0, 255);      // Limit the PID output between 0 and 255, this is important to get rid of integral windup!

  pid_RightMotor.begin();  // initialize the PID instance
  // pid_RightMotor.setpoint(1024);  // The "goal" the PID controller tries to "reach"
  pid_RightMotor.tune(1, 1.2, 0.2);  // Tune the PID, arguments: kP, kI, kD
  pid_RightMotor.limit(0, 255);      // Limit the PID output between 0 and 255, this is important to get rid of integral windup!


  // We add the task to the task scheduler
  sensorTimer.addTask(Ultrasonic1);

  // We activate the task
  Ultrasonic1.enable();

  Wire.begin();

  byte status = mpu.begin();
  // Serial.print(F("MPU6050 status: "));
  // Serial.println(status);
  while (status != 0) {}  // stop everything if could not connect to MPU6050

  // Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(3000);
  mpu.calcOffsets(true, true);  // gyro and accelero
  // Serial.println("Done!\n");
  // driveWheel(10);  //cm
  // Serial.println(mpu.getAngleZ());
}

void loop() {
  mpu.update();
  // Serial.println(mpu.getAngleZ());
  sensorTimer.execute();

  followRoute();
  // driveWheel(30000);
  // turn(-90);

  // driveWheel(300);
  // turn(0);

  // driveWheel(300);
  // turn(90);

  // driveWheel(300);
  // turn(0);

  // driveWheel(300);
  // turn(-90);

  // driveWheel(600);
  // turn(90);
  // turn(-90);

  // Serial.println(mpu.getAngleZ());

  delay(10000);
}

void calculateDistanceAngle(int x1, int y1, int x2, int y2) {
  float xd, yd;

  xd = x2 - x1;
  yd = y2 - y1;
  dir.distance = sqrt(pow(xd, 2) + pow(yd, 2));
  dir.angle = atan(yd / xd);
  dir.angle = (dir.angle * 180) / 3.142;
  dir.distance = round(dir.distance);
  dir.angle = round(dir.angle);
}

void followRoute(void) {
  long init_time = 0;
  float linear_velocity = 0;
  float angular_velocity = 0;
  float distance = 0;
  for (int row = 0; row < numberOfCoordinate; row++) {
    if (row + 1 == numberOfCoordinate) break;

    // Serial.print("First Coordinate: ");
    // Serial.print(coordinate[row][0]);
    // Serial.print(" , ");
    // Serial.println(coordinate[row][1]);
    // Serial.print("Second Coordinate: ");
    // Serial.print(coordinate[row + 1][0]);
    // Serial.print(" , ");
    // Serial.println(coordinate[row + 1][1]);

    int x1 = coordinate[row][0];
    int y1 = coordinate[row][1];
    int x2 = coordinate[row + 1][0];
    int y2 = coordinate[row + 1][1];

    calculateDistanceAngle(x1, y1, x2, y2);
    // Serial.print("First Path: ");
    // Serial.print(dir.distance);
    // Serial.print(" , ");
    // Serial.println(dir.angle);

    // if (y2 - y1 == 0) {
    //   Serial.println("Heading to Zero");
    //   headToZero();
    //   Serial.println("Got to Zero");
    // } else {
    leftencoder.write(0);
    rightencoder.write(0);
    init_time = millis();
    turn(-1 * dir.angle);
    distance = (float)(leftencoder.read() + rightencoder.read()) / 37.88;  //Distance in mm from Encoder Pulse
    angular_velocity = (float)distance / (millis() - init_time);
    // }
    // driveWheel_PID(dir.distance);
    init_time = millis();
    Serial.print(millis());
    Serial.print(',');
    Serial.println(serialOut);
    driveWheel(dir.distance);
    linear_velocity = (float)dir.distance / (millis() - init_time);
    delay(100);
  }
}

void stopBothMotor(void) {
  digitalWrite(leftMotorEN, HIGH);
  digitalWrite(rightMotorEN, HIGH);
  digitalWrite(leftMotorA, HIGH);  // left Motor
  digitalWrite(leftMotorB, HIGH);
  digitalWrite(rightMotorA, HIGH);  // right Motor
  digitalWrite(rightMotorB, HIGH);
  delay(5);
}

void turnRight(void) {
  leftencoder.write(0);
  rightencoder.write(0);

  // Serial.println(leftencoder.read());
  digitalWrite(leftMotorA, HIGH);  // left Motor Forward
  digitalWrite(leftMotorB, LOW);

  digitalWrite(rightMotorA, HIGH);  // right Motor Backward
  digitalWrite(rightMotorB, LOW);
  analogWrite(leftMotorEN, 200);  // left
  analogWrite(rightMotorEN, 200);
  Serial.print(millis());
  Serial.print(',');
  Serial.println(serialOut);

  delay(5);
}

void turnLeft(void) {
  leftencoder.write(0);
  rightencoder.write(0);

  // Serial.println(leftencoder.read());
  digitalWrite(leftMotorA, LOW);  // left Motor Backward
  digitalWrite(leftMotorB, HIGH);

  digitalWrite(rightMotorA, LOW);  // right Motor Forward
  digitalWrite(rightMotorB, HIGH);
  analogWrite(leftMotorEN, 200);  // left
  analogWrite(rightMotorEN, 200);
  Serial.print(millis());
  Serial.print(',');
  Serial.println(serialOut);

  delay(5);
}

void driveWheel(float distance) {  // distance in mm
  long pulseCnt;
  int initDegree, error;
  mpu.update();
  initDegree = mpu.getAngleZ();
  pulseCnt = 7500 * (distance / 198);  //201.1 mm for one rotation (7500 pulses)
  leftencoder.write(0);
  rightencoder.write(0);

  // Serial.println(leftencoder.read());
  digitalWrite(leftMotorA, HIGH);  // left Motor
  digitalWrite(leftMotorB, LOW);

  digitalWrite(rightMotorA, LOW);  // right MOtor
  digitalWrite(rightMotorB, HIGH);
  analogWrite(leftMotorEN, 100);  // left
  analogWrite(rightMotorEN, 100);

  delay(5);

  while (leftencoder.read() <= pulseCnt & rightencoder.read() <= pulseCnt) {
    mpu.update();
    error = (mpu.getAngleZ() - initDegree);
    // Serial.println(error);
    if (error < 0) {
      analogWrite(leftMotorEN, 100 - abs(error));
      analogWrite(rightMotorEN, 145 + abs(error));
    } else {
      analogWrite(leftMotorEN, 100 + abs(error));
      analogWrite(rightMotorEN, 145 - abs(error));
    }
    Serial.print(millis());
    Serial.print(',');
    Serial.println(serialOut);
  }

  stopBothMotor();
}

void turn(int degree) {  //Pos Degree => Clockwise
  int initialDegree;
  if (degree == 0) {
    initialDegree = mpu.getAngleZ();
    // Serial.print("Initial Degree: ");
    // Serial.println(initialDegree);

    if (initialDegree < 0) {
      turnLeft();
      while (mpu.getAngleZ() <= 0) {
        mpu.update();
        Serial.print(millis());
        Serial.print(',');
        Serial.println(serialOut);
        delay(5);
        // Serial.println(mpu.getAngleZ());
      }
      stopBothMotor();
      // Serial.print("Final Degree: ");
      // Serial.println(mpu.getAngleZ());
    } else if (initialDegree >= 0) {
      turnRight();

      while (mpu.getAngleZ() >= 0) {
        mpu.update();
        Serial.print(millis());
        Serial.print(',');
        Serial.println(serialOut);
        delay(5);
        // Serial.println(mpu.getAngleZ());
      }
      stopBothMotor();
      // Serial.print("Final Degree: ");
      // Serial.println(mpu.getAngleZ());
    }

  } else if (degree < 0) {  //Turn Left
    degree = degree + 4;
    initialDegree = mpu.getAngleZ();
    Serial.print(millis());
    Serial.print(',');
    Serial.println(serialOut);
    // Serial.print("Initial Degree: ");
    // Serial.println(initialDegree);

    turnLeft();

    while (mpu.getAngleZ() <= (-1 * degree)) {
      mpu.update();
      Serial.print(millis());
      Serial.print(',');
      Serial.println(serialOut);
      delay(5);
      // Serial.println(mpu.getAngleZ());
    }
    stopBothMotor();
    // Serial.print("Final Degree: ");
    // Serial.println(mpu.getAngleZ());
  } else {  //Turn Right
    degree = degree - 6;
    initialDegree = mpu.getAngleZ();
    Serial.print(millis());
    Serial.print(',');
    Serial.println(serialOut);
    // Serial.print("Initial Degree: ");
    // Serial.println(initialDegree);

    turnRight();

    while (mpu.getAngleZ() >= (-1 * degree)) {
      mpu.update();
      Serial.print(millis());
      Serial.print(',');
      Serial.println(serialOut);
      delay(5);
      // Serial.println(mpu.getAngleZ());
    }
    stopBothMotor();
    // Serial.print("Final Degree: ");
    // Serial.println(mpu.getAngleZ());
  }
}

void headToZero(void) {
  mpu.update();
  Serial.print(millis());
  Serial.print(',');
  turn(mpu.getAngleZ());
}

void UltrasonicTask() {
  // Serial.println("500 millisecond task");
  mpu.update();
  Serial.print(millis());
  Serial.print(',');
  Serial.println(serialOut);
}


// // ********   PID Controller For Right Motor **************

// void R_PIDCalculate(unsigned long Rcount) {
//   delay(5);
//   f_Rcount = rightencoder.read();
//   RInput = f_Rcount - Rcount;
//   RPID.Compute();
//   analogWrite(ENA, ROutput);
// }

// // *********************************************************

// // ********   PID Controller For Left Motor **************

// void L_PIDCalculate(unsigned long Lcount) {
//   delay(5);
//   f_Lcount = leftencoder.read();
//   LInput = f_Lcount - Lcount;
//   LPID.Compute();
//   analogWrite(ENB, LOutput);
// }
// // *********************************************************

void driveWheel_PID(float distance) {  // distance in mm
  long pulseCnt;
  int initDegree, error;
  mpu.update();
  initDegree = mpu.getAngleZ();
  pulseCnt = 7500 * (distance / 198);  //201.1 mm for one rotation (7500 pulses)
  pid_RightMotor.setpoint(pulseCnt);
  pid_LeftMotor.setpoint(pulseCnt);

  leftencoder.write(0);
  rightencoder.write(0);

  // Serial.println(leftencoder.read());
  digitalWrite(leftMotorA, HIGH);  // left Motor
  digitalWrite(leftMotorB, LOW);

  digitalWrite(rightMotorA, LOW);  // right MOtor
  digitalWrite(rightMotorB, HIGH);
  analogWrite(leftMotorEN, 100);  // left
  analogWrite(rightMotorEN, 100);

  delay(5);

  while (leftencoder.read() <= pulseCnt & rightencoder.read() <= pulseCnt) {
    mpu.update();
    error = (mpu.getAngleZ() - initDegree);
    // Serial.println(error);
    leftMotorSpeed = pid_LeftMotor.compute(leftencoder.read());
    rightMotorSpeed = pid_RightMotor.compute(rightencoder.read());
    // Serial.print("Left Motor Speed: ");
    // Serial.println(leftMotorSpeed);
    // Serial.print("Right Motor Speed: ");
    // Serial.println(rightMotorSpeed);
    if (error < 0) {
      analogWrite(leftMotorEN, leftMotorSpeed - 45);
      analogWrite(rightMotorEN, rightMotorSpeed);
    } else {
      analogWrite(leftMotorEN, leftMotorSpeed - 45);
      analogWrite(rightMotorEN, rightMotorSpeed);
    }
    Serial.print(millis());
    Serial.print(',');
    Serial.println(serialOut);
  }

  stopBothMotor();
}

void calculate_velocity() {
  long init_time = millis();
}
