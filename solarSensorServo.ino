// Colton Blume
// solar sensor servo
// March 13, 2015

#include <Servo.h>
Servo vServo;
Servo hServo;

int TOLERANCE = 5;
int verticalMovementDegrees = 1;
int horizontalMovementDegrees = 1;

// max and mins for servos, to make sure we don't overextend
int vertMin = 40;
int vertMax = 145;

int horiMin = 0;
int horiMax = 175;

// store value of servo location
int vServoVal = 100;
int hServoVal = 60;

// declare the pins we are using to move servos
int vServoPin = 9;
int hServoPin = 10;

int lightSensorPin0 = A0;
int lightSensorPin1 = A1;
int lightSensorPin2 = A2;
int lightSensorPin3 = A3;

// declare variables to store values
int sensorValue0;
int sensorValue1;
int sensorValue2;
int sensorValue3;

// store raw value from light sensors for debugging
int rawSensorValue0;
int rawSensorValue1; 
int rawSensorValue2; 
int rawSensorValue3; 

// averages to compare and see what direction to move servo
int avgTop;
int avgBottom;
int avgLeft;
int avgRight;

// difference between the averages
int horizontal;
int vertical;

void setup() {
  Serial.begin(9600);
  vServo.attach(vServoPin);
  hServo.attach(hServoPin);
  
  // ensure that the servo starts at default values
  vServo.write(vServoVal);
  hServo.write(hServoVal);
}

void loop(){
  
  readSensors(); 
  moveServos();
  
  delay(300000); 
}

void readSensors(){
  
  // read raw values from analog pins
  rawSensorValue0 = analogRead(lightSensorPin0); // assumed top left
  rawSensorValue1 = analogRead(lightSensorPin1); // assumed bottom left
  rawSensorValue2 = analogRead(lightSensorPin2); // assumed top right
  rawSensorValue3 = analogRead(lightSensorPin3); // assumed bottom right
  
  
  // Map those values to a smaller value to handle tolerance of small changes in values
  sensorValue0 = map(rawSensorValue0, 0, 1023, 1, 180);
  sensorValue1 = map(rawSensorValue1, 0, 1023, 1, 180);
  sensorValue2 = map(rawSensorValue2, 0, 1023, 1, 180);
  sensorValue3 = map(rawSensorValue3, 0, 1023, 1, 180);
  
  // Get averages of all 4 sides
  avgTop = (sensorValue0 + sensorValue2)/2;
  avgBottom = (sensorValue1 + sensorValue3)/2;
  avgLeft = (sensorValue0 + sensorValue1)/2;
  avgRight = (sensorValue3 + sensorValue2)/2;
  
  // determine difference for tolerance
  vertical = avgTop - avgBottom;
  horizontal = avgLeft - avgRight;

}

void moveServos(){
  
  while( (abs(vertical) > TOLERANCE) || (abs(horizontal) > TOLERANCE) ) {
    
    // move vertical axis
    if(vertical > 10 && vServoVal < vertMax) { // changed to 10 from 0 for sensitivity reasons
      vServoVal += 1;
      Serial.println("moving vServo Up");
    } else if(vServoVal > vertMin) {
      vServoVal -= verticalMovementDegrees;
      Serial.println("moving vServo Down");
    }
    
    // Move horizontal axis
    if(horizontal < 0 && hServoVal < horiMax) { 
      hServoVal += 1;
      Serial.println("moving hServo Left");
    } else if(hServoVal > horiMin) {
      hServoVal -= horizontalMovementDegrees;
      Serial.println("moving hServo Right");
    }
    vServo.write(vServoVal);
    hServo.write(hServoVal);
    readSensors();
  }
  
  
}
