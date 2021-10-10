//This sketch is designed to control the shield after receiving signal from the Processing sketch through serial communication
//Upload this sketch on Arduino first before opening the Processing sketch
//
// - 06/25/2015 -


#include <AFMotor.h>
#include <Servo.h> 

int angle1 = 90;
int angle2 = 90;

AF_DCMotor motor1(3);
AF_DCMotor motor2(4);

Servo servo1;
Servo servo2;

void setup()
{
    servo1.attach(9);
    servo2.attach(10); 
    
    // Start up serial connection
    Serial.begin(9600); // baud rate
    Serial.flush();
}
     
void loop()
{
    String input = "";
     
    // Read any serial input
    while (Serial.available() > 0)
    {
        input += (char) Serial.read(); // Read in one char at a time
        delay(5); // Delay for 5 ms so the next char has time to be received
    }
     
    if (input == "0")
    {   
        motor1.run(RELEASE);
        motor2.run(RELEASE);
      
    }
    
    else if (input == "5")
    {
        angle1 = angle1 + 1;
        servo1.write(angle1);
    }
    
    else if (input == "6")
    {
        angle1 = angle1 - 1;
        servo1.write(angle1);
    }
    
    else if (input == "7")
    {
        angle2 = angle2 + 1;
        servo2.write(angle2);
    }

    else if (input == "8")
    {
        angle2 = angle2 - 1;
        servo2.write(angle2);
    }
    
    else if (input == "1")
    {
       motor1.setSpeed(200); //I'm not sure if the motor is turning left or not
       motor1.run(FORWARD); //If it is not, change this function to run(BACKWARD)
    }

    else if (input == "2")
    {
       motor1.setSpeed(200);
       motor1.run(BACKWARD); // and change this function to .run(FORWARD)
    }
    
    else if (input == "3")
    {
       motor2.setSpeed(200); //I'm not sure if the motor is turning left or not
       motor2.run(FORWARD); //If it is not, change this function to run(BACKWARD)
    }

    else if (input == "4")
    {
       motor2.setSpeed(200); 
       motor2.run(BACKWARD); // and change this function to .run(FORWARD)
    }
}


