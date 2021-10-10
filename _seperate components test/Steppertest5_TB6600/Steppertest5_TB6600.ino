/* Example sketch to control a stepper motor with TB6600 stepper motor driver and Arduino without a library: continuous rotation. More info: https://www.makerguides.com */
// Define stepper motor connections:
#define enPin 4
#define dirPin 2
#define stepPin 3
void setup() {
  // Declare pins as output:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  // Set the spinning direction CW/CCW:
  
}
void loop() {
  // These four lines result in 1 step:
  digitalWrite(enPin, LOW);
  digitalWrite(dirPin, HIGH);
  for(int i = 0; i < 400 ; i++){
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(500);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(500);}
  digitalWrite(enPin, HIGH);
  delay(5000);

  digitalWrite(enPin, LOW);
  digitalWrite(dirPin, LOW);
  for(int i = 0; i < 400 ; i++){
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(500);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(500);}
  digitalWrite(enPin, HIGH);
  delay(5000);
}
