

#define RPWM 7
#define LPWM 6
#define REN 5
#define LEN 9
int out1=27;
int volt=180;
const int dataIN = 3; //IR sensor INPUT
unsigned long prevmillis; // To store time
unsigned long duration; // To store time difference
unsigned long lcdrefresh; // To store time for lcd to refresh
int rpm=1; // RPM value
int target=28;
boolean currentstate; // Current state of IR input scan
boolean prevstate; // State of IR sensor in previous scan

void setup() {
  Serial.begin(9600);
  pinMode(RPWM,OUTPUT);
  pinMode(LPWM,OUTPUT);
  pinMode(LEN,OUTPUT);
  pinMode(REN,OUTPUT);
  digitalWrite(REN,HIGH);
  digitalWrite(LEN,HIGH);
  
  pinMode(dataIN,INPUT);     
  prevmillis = 0;
  prevstate = LOW; 

}
 
 
void loop() {
    Serial.print(rpm);
    Serial.print("  ");
    Serial.println(out1);
    volt=map(out1,0,100,0,255);
    analogWrite(RPWM,volt);
    analogWrite(LPWM,0);
     // RPM Measurement
  currentstate = digitalRead(dataIN); // Read IR sensor state
 if( prevstate != currentstate) // If there is change in input
   {
     if( currentstate == HIGH ) // If input only changes from LOW to HIGH
       {
         duration = ( micros() - prevmillis ); // Time difference between revolution in microsecond
         rpm = (2500000/duration); // rpm = (1/ time millis)*1000*1000*60; 
         prevmillis = micros(); // store time for nect revolution calculation
         //    out1=map(out1,0,255,0,34);
       }
   }
  prevstate = currentstate; // store this scan (prev scan) data for next scan
  if (millis()%10==0){
    if (rpm > target) out1--;
    if (rpm < target) out1++;
    if (out1>100) out1=100;
    if (out1<15) out1=15;
  } 
  }
