/*     Simple Stepper Motor Control Exaple Code
 *      
 *  by Dejan Nedelkovski, www.HowToMechatronics.com
 *  
 */
// defines pins numbers
const int stepPin = 3; 
const int dirPin = 4; 
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
}
void loop() {
  digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 20; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(3); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(3); 
  }
  delay(1000); // One second delay
  
  digitalWrite(dirPin,LOW); //Changes the rotations direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 20; x++) {
    digitalWrite(stepPin,HIGH);
    delay(3);
    digitalWrite(stepPin,LOW);
    delay(3);
  }
  delay(1000);
}
