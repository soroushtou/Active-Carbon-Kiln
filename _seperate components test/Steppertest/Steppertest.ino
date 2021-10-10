// Include the Arduino Stepper Library
#include <Stepper.h>

// Number of steps per output rotation
const int stepsPerRevolution = 200;

// Create Instance of Stepper library
Stepper myStepper(stepsPerRevolution, 14, 15, 16, 17);


void setup()
{
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() 
{ 
  // step one revolution in one direction:
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution/4 + stepsPerRevolution/60);
  digitalWrite(14,LOW);
  digitalWrite(15,LOW);
  digitalWrite(16,LOW);
  digitalWrite(17,LOW);
  delay(5000);
  

  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution/4 - stepsPerRevolution/60);
  digitalWrite(14,LOW);
  digitalWrite(15,LOW);
  digitalWrite(16,LOW);
  digitalWrite(17,LOW);
  delay(5000);
}
