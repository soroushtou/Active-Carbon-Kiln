#include <EEPROM.h>
#include "Wire.h"
#include <Stepper.h>

float feeder_RPM_set = 0;
float kiln_RPM_set = 0;
int temperature_set = 0;
int steam_mode = 0;

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution for your motor

#define MPU9250_ADDRESS 0x68
#define MPU9250_ACC_FULL_SCALE_2_G 0x00

#define Flame_mode_pot A9
#define Thermocouple A8
#define Floater 38
#define Steam_gen1 23
#define Steam_gen2 25
#define Steam_gen3 27
#define Gas_pump 29
#define Air_valve 31
#define Gas_valve 33
#define Water_inlet 35
#define Steam_pump 37
#define Flame_stepper1 14
#define Flame_stepper2 15
#define Flame_stepper3 16
#define Flame_stepper4 17
#define Hall_sensor 3 
#define Feeder_PWM 7
#define Kiln_PWM 6
#define Feeder_PWM_en 5
#define Kiln_PWM_en 4
  
#define MQ2 A2
#define MQ3 A6
#define MQ4 A4
#define MQ7 A5
#define MQ9 A1
#define MQ135 A0
#define MQ214 A3

#define V0_set_button 13   // button used for setting all V0 values
//#define next_button 22     // button used for switching between display
int temperature,temperature_counter,stepper_position,flame_mode,scale_speed;
bool temperature_condition;
unsigned long long time_prev,time_diff;
//bool V0_set_state = false;
//Values for Angle
float yangle,feeder_RPM;
//int yangle2;


//Values for MQ2
float MQ2_V0;  // Get the value of V0 via in Air
double MQ2_ratio; // Get ratio ( MQ2_volt - MQ2_V0 ) / MQ2_V0
float MQ2_ppm;
float MQ2_volt;

//Values for MQ3
float MQ3_V0;  // Get the value of V0 via in Air
double MQ3_ratio; // Get ratio ( MQ3_volt - MQ3_V0 ) / MQ3_V0
float MQ3_ppm;
float MQ3_volt;

//Values for MQ4
float MQ4_V0;  // Get the value of V0 via in Air
double MQ4_ratio; // Get ratio ( MQ4_volt - MQ4_V0 ) / MQ4_V0
float MQ4_ppm;
float MQ4_volt;

//Values for MQ7
float MQ7_V0;  // Get the value of V0 via in Air
double MQ7_ratio; // Get ratio ( MQ7_volt - MQ7_V0 ) / MQ7_V0
float MQ7_ppm;
float MQ7_volt;

//Values for MQ9
float MQ9_V0;  // Get the value of V0 via in Air
double MQ9_ratio; // Get ratio ( MQ9_volt - MQ9_V0 ) / MQ9_V0
float MQ9_ppm;
float MQ9_volt;

//Values for MQ135
float MQ135_V0;  // Get the value of V0 via in Air
double MQ135_ratio; // Get ratio ( MQ135_volt - MQ135_V0 ) / MQ135_V0
float MQ135_ppm;
float MQ135_volt;

//Values for MQ214
float MQ214_V0;  // Get the value of V0 via in Air
double MQ214_ratio; // Get ratio ( MQ214_volt - MQ214_V0 ) / MQ214_V0
float MQ214_ppm;
float MQ214_volt;

//Values for display
//int count_display = 0;
//unsigned int menu = 0;

//EEPROM addresses
const int address0 = 0;
const int address1 = address0+sizeof(float);
const int address2 = address1+sizeof(float);
const int address3 = address2+sizeof(float);
const int address4 = address3+sizeof(float);
const int address5 = address4+sizeof(float);
const int address6 = address5+sizeof(float);

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, Flame_stepper1, Flame_stepper2, Flame_stepper3, Flame_stepper4);






// Timer3 overflow interrupt service routine
ISR(TIMER3_COMPA_vect) {
// Reinitialize Timer3 value
//TCNT3H=0x600 >> 8;           uncomment if needed
//TCNT3L=0x600 & 0xff;
// Place your code here
temperature_counter ++;
if(temperature - temperature_set >= -50 && temperature - temperature_set <= 50){ 
  if(temperature_counter == 1){set_steam_mode(LOW);}
  if(temperature_counter == 21){digitalWrite(Steam_pump,LOW);}
  if(temperature_counter == 31){digitalWrite(Air_valve,LOW);digitalWrite(Gas_valve,HIGH);}
  if(temperature_counter == 61){digitalWrite(Gas_valve,LOW);digitalWrite(Air_valve,HIGH);digitalWrite(Steam_pump,HIGH);set_steam_mode(HIGH);}
}


if(temperature_counter >= 180){
  int temperature_diff = temperature_set - temperature;
  stepper_position = analogRead(Flame_mode_pot);                  //0.6v at 0deg = 122.76       4.2v at 90deg = 859.32     each step is 1.8deg
  stepper_position = map(stepper_position,122,860,0,50);              //note that 200 steps = 360deg so 90deg = 50steps
  
  if(temperature_diff >= 100)myStepper.step(50 - stepper_position);                             //maximum : goes to 90deg  
  if(temperature_diff >= 60 && temperature_diff < 100)myStepper.step(43 - stepper_position);    //          goes to 77deg  
  if(temperature_diff >= 20 && temperature_diff < 60)myStepper.step(36 - stepper_position);     //minimum : goes to 64deg
  if(temperature_diff < 20 && temperature_diff > -20);                                   //does not change the flame_mode
  if(temperature_diff <= -20 && temperature_diff > -60)myStepper.step(30 - stepper_position);   //minimum : goes to 53deg
  if(temperature_diff <= -60 && temperature_diff > -100)myStepper.step(22 - stepper_position);  //minimum : goes to 40deg
  if(temperature_diff <= -100)myStepper.step(15 - stepper_position);                            //minimum : goes to 27deg
  
  flame_mode = analogRead(Flame_mode_pot);
  flame_mode = map(flame_mode,122,860,0,90);       //shows the new position
  temperature_counter = 0;
  }
}







void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();

  // Read Nbytes
  Wire.requestFrom(Address, Nbytes);
  uint8_t index = 0;
  while (Wire.available())
    Data[index++] = Wire.read();
}

void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}

void RPM_measure(void){
  time_diff = millis() - time_prev;
  time_prev = millis();
  feeder_RPM = 60.0 * (float)time_diff/ 1000;
  float feeder_RPM_diff = feeder_RPM_set - feeder_RPM;
  int a = feeder_RPM_diff * 255 / 34;
  OCR4BL = a;                      //lazeme ke tabdilesh konim be inr ya char ???
}


void setup() {
attachInterrupt(digitalPinToInterrupt(Hall_sensor), RPM_measure, FALLING);

// set the speed at 60 rpm:
myStepper.setSpeed(60);

// Timer/Counter 3 initialization
// Clock source: System Clock
// Clock value: 62.500 kHz
// Mode: Normal top=0xFFFF
// OC3A output: Disconnected
// OC3B output: Disconnected
// OC3C output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer Period: 1.024 s
// Timer3 Overflow Interrupt: On
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
// Compare C Match Interrupt: Off
TCCR3A=(0<<COM3A1) | (0<<COM3A0) | (0<<COM3B1) | (0<<COM3B0) | (0<<COM3C1) | (0<<COM3C0) | (0<<WGM31) | (0<<WGM30);
TCCR3B=(0<<ICNC3) | (0<<ICES3) | (0<<WGM33) | (0<<WGM32) | (1<<CS32) | (0<<CS31) | (0<<CS30);
TCNT3H=0x06;
TCNT3L=0x00;
ICR3H=0x00;
ICR3L=0x00;
OCR3AH=0x00;
OCR3AL=0x00;
OCR3BH=0x00;
OCR3BL=0x00;
OCR3CH=0x00;
OCR3CL=0x00;

// Timer/Counter 4 initialization
// Clock source: System Clock
// Clock value: 2000.000 kHz
// Mode: Fast PWM top=0x00FF
// OC4A output: Non-Inverted PWM
// OC4B output: Non-Inverted PWM
// OC4C output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer Period: 0.128 ms
// Output Pulse(s):
// OC4A Period: 0.128 ms Width: 0.032125 ms
// OC4B Period: 0.128 ms Width: 0.064251 ms
// Timer4 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
// Compare C Match Interrupt: Off
TCCR4A=(1<<COM4A1) | (0<<COM4A0) | (1<<COM4B1) | (0<<COM4B0) | (0<<COM4C1) | (0<<COM4C0) | (0<<WGM41) | (1<<WGM40);
TCCR4B=(0<<ICNC4) | (0<<ICES4) | (0<<WGM43) | (1<<WGM42) | (0<<CS42) | (1<<CS41) | (0<<CS40);
TCNT4H=0x00;
TCNT4L=0x00;
ICR4H=0x00;
ICR4L=0x00;
OCR4AH=0x00;
OCR4AL=0x00;
OCR4BH=0x00;
OCR4BL=0x00;
OCR4CH=0x00;
OCR4CL=0x00;

// Timer/Counter 3 Interrupt(s) initialization
TIMSK3=(0<<ICIE3) | (0<<OCIE3C) | (0<<OCIE3B) | (0<<OCIE3A) | (0<<TOIE3);

EEPROM.get(address0,MQ2_V0);
EEPROM.get(address1,MQ3_V0);
EEPROM.get(address2,MQ4_V0);
EEPROM.get(address3,MQ7_V0);
EEPROM.get(address4,MQ9_V0);
EEPROM.get(address5,MQ135_V0);
EEPROM.get(address6,MQ214_V0);

pinMode(V0_set_button,INPUT_PULLUP);
pinMode(Floater,INPUT);
pinMode(Steam_gen1,OUTPUT);
pinMode(Steam_gen2,OUTPUT);
pinMode(Steam_gen3,OUTPUT);
pinMode(Steam_pump,OUTPUT);
pinMode(Gas_pump,OUTPUT);
pinMode(Air_valve,OUTPUT);
pinMode(Gas_valve,OUTPUT);
pinMode(Water_inlet,OUTPUT);
pinMode(Flame_stepper1,OUTPUT);
pinMode(Flame_stepper2,OUTPUT);
pinMode(Flame_stepper3,OUTPUT);
pinMode(Flame_stepper4,OUTPUT);
pinMode(Hall_sensor,INPUT);
pinMode(Feeder_PWM,OUTPUT);
pinMode(Kiln_PWM,OUTPUT);
pinMode(Feeder_PWM_en,OUTPUT);
pinMode(Kiln_PWM_en,OUTPUT);

digitalWrite(Feeder_PWM_en,HIGH);
digitalWrite(Kiln_PWM_en,HIGH);
digitalWrite(Gas_valve,HIGH);
digitalWrite(Steam_pump,LOW);
digitalWrite(Air_valve,LOW);
digitalWrite(Gas_pump,LOW);
digitalWrite(Feeder_PWM_en,HIGH);
digitalWrite(Kiln_PWM_en,HIGH);

Wire.begin();
I2CwriteByte(MPU9250_ADDRESS, 29, 0x06);
I2CwriteByte(MPU9250_ADDRESS, 28, MPU9250_ACC_FULL_SCALE_2_G);
I2CwriteByte(MPU9250_ADDRESS, 108, 0x2F);
get_angle();


Serial.begin(9600);
Serial1.begin(9600);


Serial.println("CLEARDATA"); //clears up any data left from previous projects
//Serial.println("DATA,Temperature_set,Feeder_RPM_set,Body_RPM_set,Angle_set,Steam_mode_set"); 
Serial.println("LABEL,Zaman,MQ2_volt,MQ3_volt,MQ4_volt,MQ7_volt,MQ9_volt,MQ135_volt,MQ214_volt,MQ2_ratio,MQ3_ratio,MQ4_ratio,MQ7_ratio,MQ9_ratio,MQ135_ratio,MQ214_ratio,Temperature,Flame_mode,Feeder_RPM,Scale_Speed,Angle"); //LABEL, so excel knows the next things will be the names of the columns
Serial.print("DATA,TIME,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,");
Serial.println(yangle);

 

//if(temperature_set < 250) temperature_start = 250;
//else temperature_start = temperature_set - 150; 
Serial.println("RESETTIMER"); //resets timer to 0



temperature = analogRead(Thermocouple);
while(temperature < 100){
  stepper_position = analogRead(Flame_mode_pot);          
  stepper_position = map(stepper_position,122,860,0,50);              
  myStepper.step(15 - stepper_position);
  flame_mode = analogRead(Flame_mode_pot);
  flame_mode = map(flame_mode,122,860,0,90);
  temperature = analogRead(Thermocouple);
  Serial.print("DATA,TIME,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,");
  Serial.print(temperature);
  Serial.println(",*,*,*");
}
while(temperature < 180){
  stepper_position = analogRead(Flame_mode_pot);          
  stepper_position = map(stepper_position,122,860,0,50);              
  myStepper.step(33 - stepper_position);
  flame_mode = analogRead(Flame_mode_pot);
  flame_mode = map(flame_mode,122,860,0,90);
  temperature = analogRead(Thermocouple);
  Serial.print("DATA,TIME,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,");
  Serial.print(temperature);
  Serial.println(",*,*,*");
}
while(temperature < 250){
  stepper_position = analogRead(Flame_mode_pot);          
  stepper_position = map(stepper_position,122,860,0,50);              
  myStepper.step(50 - stepper_position);
  flame_mode = analogRead(Flame_mode_pot);
  flame_mode = map(flame_mode,122,860,0,90);
  temperature = analogRead(Thermocouple);
  Serial.print("DATA,TIME,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,*,");
  Serial.print(temperature);
  Serial.println(",*,*,*");
}
TIMSK3 |= (1<<OCIE3A);
temperature_condition = 1;
}
 bool get_excel;
void loop() {
 
  if(get_excel==0){

  Serial.println("CELL,GET,U3");
  //while(Serial.available()==0); 
  temperature_set = Serial.parseInt();

  Serial.println("CELL,GET,V3");
  //while(Serial.available()==0); 
  feeder_RPM_set = Serial.parseFloat();

  Serial.println("CELL,GET,W3");
  //while(Serial.available()==0); 
  kiln_RPM_set = Serial.parseFloat();

  Serial.println("CELL,GET,X3");
  steam_mode = Serial.parseInt();
  //while(Serial.available()==0);
  get_excel = 1;
  
  float b = kiln_RPM_set * 255 / 12;
  //OCR4AL = (int)b;
  // Global enable interrupts
  set_steam_mode(LOW);
  sei(); //enable all interrupts
  }
  // put your main code here, to run repeatedly:
  temperature = analogRead(Thermocouple);
  get_scale_data();
  get_MQ_data();
  send_data();
  
  if(digitalRead(V0_set_button)==LOW) {
    while(digitalRead(V0_set_button)==LOW);

    MQ2_V0 = MQ2_volt ;
    MQ3_V0 = MQ3_volt ;
    MQ4_V0 = MQ4_volt ;
    MQ7_V0 = MQ7_volt ;
    MQ9_V0 = MQ9_volt ;
    MQ135_V0 = MQ135_volt ;
    MQ214_V0 = MQ214_volt ;
    EEPROM.put(address0,MQ2_V0);
    EEPROM.put(address1,MQ3_V0);
    EEPROM.put(address2,MQ4_V0);
    EEPROM.put(address3,MQ7_V0);
    EEPROM.put(address4,MQ9_V0);
    EEPROM.put(address5,MQ135_V0);
    EEPROM.put(address6,MQ214_V0);
  }
  
  delay(100);
}

void get_angle(void){
  uint8_t Buf[14];
  I2Cread(MPU9250_ADDRESS, 0x3B, 14, Buf);
  int16_t ay = -(Buf[2] << 8 | Buf[3]);
  yangle = ay / 182.04;  
  yangle = float(int(yangle*2)) / 2;
}

void set_steam_mode(bool state){
  switch (steam_mode){
    case 1:
      digitalWrite(Steam_gen1,state);
      //digitalWrite(Steam_pump,state);
      break;
    case 2:
      digitalWrite(Steam_gen1,state);
      digitalWrite(Steam_gen2,state);
      //digitalWrite(Steam_pump,state);
      break;
    case 3:
      digitalWrite(Steam_gen1,state);
      digitalWrite(Steam_gen2,state);
      digitalWrite(Steam_gen3,state);
      //digitalWrite(Steam_pump,state);
      break;
  }
}
void send_data(void){
  cli(); //disable all interrupts         !!!!!!!!!!!!!!dar soorate niaz uncomment konid
  
  Serial.print("DATA,TIME,");
  Serial.print(MQ2_volt);
  Serial.print(",");
  Serial.print(MQ3_volt);
  Serial.print(",");
  Serial.print(MQ4_volt);
  Serial.print(",");
  Serial.print(MQ7_volt);
  Serial.print(",");
  Serial.print(MQ9_volt);
  Serial.print(",");
  Serial.print(MQ135_volt);
  Serial.print(",");
  Serial.print(MQ214_volt);
  Serial.print(",");
  Serial.print(MQ2_ratio);
  Serial.print(",");
  Serial.print(MQ3_ratio);
  Serial.print(",");
  Serial.print(MQ4_ratio);
  Serial.print(",");
  Serial.print(MQ7_ratio);
  Serial.print(",");
  Serial.print(MQ9_ratio);
  Serial.print(",");
  Serial.print(MQ135_ratio);
  Serial.print(",");
  Serial.print(MQ214_ratio);
  Serial.print(",");
  Serial.print(temperature);
  Serial.print(",");
  Serial.print(flame_mode);
  Serial.print(",");
  Serial.print(feeder_RPM);
  Serial.print(",");
  Serial.println(scale_speed);
  
  sei(); //enable all interrupts       !!!!!!!!!!!!!!dar soorate niaz uncomment konid
}

void get_scale_data(void){
  //while(Serial1.available()==0);
  if(Serial1.available()==1) scale_speed = Serial1.parseInt();
}

void get_MQ_data(void){
        if(temperature_condition == 1){
  //get the values :
    get_MQ2_ratio();      
    get_MQ3_ratio();   
    get_MQ4_ratio();  
    get_MQ7_ratio();
    get_MQ9_ratio();
    get_MQ135_ratio();
    get_MQ214_ratio();
    }
    else{
    MQ2_volt = 0;
    MQ3_volt = 0;
    MQ4_volt = 0;
    MQ7_volt = 0;
    MQ9_volt = 0;
    MQ135_volt = 0;
    MQ214_volt = 0;
    }
    MQ2_ratio = (MQ2_volt - MQ2_V0)/MQ2_V0;
    MQ3_ratio = (MQ3_volt - MQ3_V0)/MQ3_V0;
    MQ4_ratio = (MQ4_volt - MQ4_V0)/MQ4_V0;
    MQ7_ratio = (MQ7_volt - MQ7_V0)/MQ7_V0;
    MQ9_ratio = (MQ9_volt - MQ9_V0)/MQ9_V0;
    MQ135_ratio = (MQ135_volt - MQ135_V0)/MQ135_V0;
    MQ214_ratio = (MQ214_volt - MQ214_V0)/MQ214_V0;
}



void get_MQ2_ratio(void){
  
    int sensorValue[101]; 
  // Get a average data by testing 100 times
    for(int x = 0 ; x < 100 ; x++)
    {
        sensorValue[x] = analogRead(MQ2);
    }
    sort_array(sensorValue,101);
    MQ2_volt = sensorValue[50]*5.0/1023;
     
}

void get_MQ3_ratio(void){
  
    int sensorValue[101]; 
  // Get a average data by testing 100 times
    for(int x = 0 ; x < 100 ; x++)
    {
        sensorValue[x] = analogRead(MQ3);
    }
    sort_array(sensorValue,101);
    MQ3_volt = sensorValue[50]*5.0/1023;
     
}


void get_MQ4_ratio(void){
  
    int sensorValue[101]; 
  // Get a average data by testing 100 times
    for(int x = 0 ; x < 100 ; x++)
    {
        sensorValue[x] = analogRead(MQ4);
    }
    sort_array(sensorValue,101);
    MQ4_volt = sensorValue[50]*5.0/1023;
     
}



void get_MQ7_ratio(void){
      
  int sensorValue[101];
  // Get a average data by testing 100 times
    for(int x = 0 ; x < 100 ; x++)
    {
        sensorValue[x] = analogRead(MQ7);
    }
    sort_array(sensorValue,101);
    MQ7_volt = sensorValue[50]*5.0/1023;
     
}


void get_MQ9_ratio(void){
  
    int sensorValue[101]; 
  // Get a average data by testing 100 times
    for(int x = 0 ; x < 100 ; x++)
    {
        sensorValue[x] = analogRead(MQ9);
    }
    sort_array(sensorValue,101);
    MQ9_volt = sensorValue[50]*5.0/1023;
     
}

void get_MQ135_ratio(void){
  
    int sensorValue[101]; 
  // Get a average data by testing 100 times
    for(int x = 0 ; x < 100 ; x++)
    {
        sensorValue[x] = analogRead(MQ135);
    }
    sort_array(sensorValue,101);
    MQ135_volt = sensorValue[50]*5.0/1023;
     
}


void get_MQ214_ratio(void){
  
    int sensorValue[101]; 
  // Get a average data by testing 100 times
    for(int x = 0 ; x < 100 ; x++)
    {
        sensorValue[x] = analogRead(MQ214);
    }
    sort_array(sensorValue,101);
    MQ214_volt = sensorValue[50]*5.0/1023;
     
}

void sort_array(int a[], int size) {
    for(int i=0; i<(size-1); i++) {
        for(int o=0; o<(size-(i+1)); o++) {
                if(a[o] > a[o+1]) {
                    int t = a[o];
                    a[o] = a[o+1];
                    a[o+1] = t;
                }
        }
    }
}




