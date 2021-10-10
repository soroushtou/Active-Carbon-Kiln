#include <EEPROM.h>
#include "Wire.h"

int feeder_RPM_set = 5;   // 0 - 34 RPM
float kiln_RPM_set = 5;     // 0.00 - 12.00 RPM
int temperature_set = 700;    // 0 to 1023 C
int steam_mode = 1;         // 1 , 2 , 3


#define MPU9250_ADDRESS 0x68
#define MPU9250_ACC_FULL_SCALE_2_G 0x00

#define test_pin 13
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
#define Scale_en 39
#define Flame_stepper1 14
#define Flame_stepper2 15
#define Flame_stepper3 16
#define Flame_stepper4 17
#define IR_sensor 3 //#define Hall_sensor 3 
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

long k;
uint8_t Buf[14];
//#define V0_set_button 13   // button used for setting all V0 values

int temperature,temperature_diff ,stepper_counter,analyzer_counter,floater_counter,stepper_position,flame_mode,scale_speed;
bool temperature_condition;
boolean prevstate;
unsigned long time_prev,time_prev2,time_prev3,time_diff;
//bool V0_set_state = false;
float yangle,feeder_RPM;
int RPM_count;


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


//EEPROM addresses
const int address0 = 0;
const int address1 = address0+sizeof(float);
const int address2 = address1+sizeof(float);
const int address3 = address2+sizeof(float);
const int address4 = address3+sizeof(float);
const int address5 = address4+sizeof(float);
const int address6 = address5+sizeof(float);
const int median_number = 31;


// Timer3 overflow interrupt service routine (used for feeder rpm measuring)
ISR(TIMER3_COMPA_vect){
cli();

boolean currentstate = digitalRead(IR_sensor); // Read IR sensor state
 unsigned long duration = ( micros() - time_prev ); // Time difference between revolution in microsecond
 if( prevstate != currentstate) // If there is change in input
   {
     if( currentstate == HIGH ) // If input only changes from LOW to HIGH
       {
         
         feeder_RPM = (2500000/duration); // rpm = (1/ time millis)*1000*1000*60; 
         time_prev = micros(); // store time for nect revolution calculation
         //    out1=map(out1,0,255,0,34);
        if (feeder_RPM > feeder_RPM_set) OCR4BL--;
        if (feeder_RPM < feeder_RPM_set) OCR4BL++;
       }
   }
 if (prevstate == currentstate && duration > 1000000) OCR4BL++;
 if (OCR4BL>254) OCR4BL=254;
 if (OCR4BL<60) OCR4BL=60;
 prevstate = currentstate; // store this scan (prev scan) data for next scan
 
sei();
}

void check_temperature(void){
digitalWrite(test_pin,digitalRead(test_pin) ^ 1);
stepper_counter ++;

        //if(temperature_diff > -2 && temperature_diff < 2)
        temperature_condition = 1;

  if(temperature_condition == 1){
  analyzer_counter++; 
  if(analyzer_counter == 10){set_steam_mode(HIGH);}
  if(analyzer_counter == 21){digitalWrite(Steam_pump,HIGH);}
  if(analyzer_counter == 31){digitalWrite(Air_valve,HIGH);delay(200);digitalWrite(Gas_valve,LOW);delay(200);digitalWrite(Gas_pump,LOW);}
  if(analyzer_counter == 61){digitalWrite(Gas_valve,HIGH);delay(200);digitalWrite(Air_valve,LOW);delay(200);digitalWrite(Steam_pump,LOW);delay(200);set_steam_mode(LOW);}  /////////////////////////be in khat mirese mitereke
  if(analyzer_counter == 151){digitalWrite(Gas_pump,HIGH);delay(200);digitalWrite(Air_valve,HIGH);}
  if(analyzer_counter == 240){analyzer_counter=0;}
}


  if(stepper_counter > 5){
    if(stepper_position - temperature_diff < -2) {digitalWrite(Flame_stepper2,HIGH);digitalWrite(Flame_stepper1,LOW);}
  else if(stepper_position - temperature_diff > 2){digitalWrite(Flame_stepper2,LOW);digitalWrite(Flame_stepper1,HIGH);}
  stepper_counter = 0;
  }
  
  
  
  //water level
  if(digitalRead(Floater)==LOW && floater_counter < 3) floater_counter ++;
  if(digitalRead(Floater)==HIGH && floater_counter < 3) floater_counter = 0;
  
  if(floater_counter >= 3) {digitalWrite(Water_inlet,LOW); floater_counter++;}
  if(floater_counter == 7) {digitalWrite(Water_inlet,HIGH); floater_counter = 0;}
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
  while (Wire.available()) Data[index++] = Wire.read();
}

void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}

  
void get_angle(void){
  I2Cread(MPU9250_ADDRESS, 0x3B, 14, Buf);
  int16_t ay = -(Buf[2] << 8 | Buf[3]);
  yangle = ay / 182.04;  
  yangle = float(int(yangle*2)) / 2;
}


void setup() {

Wire.begin();       //THE NEVER ENDING HHHHHAAAAAAAAAAAAAANNNNNNNNNNNNGGGGGGGGGGGGGG
I2CwriteByte(MPU9250_ADDRESS, 29, 0x06);
I2CwriteByte(MPU9250_ADDRESS, 28, MPU9250_ACC_FULL_SCALE_2_G);
I2CwriteByte(MPU9250_ADDRESS, 108, 0x2F);
get_angle();   
Wire.end();

cli(); //another part of HAAAAAAAAAAAANNNNNNNNNNGGGGGGGGGGG

//**********************************Unused pins:*****///
pinMode(A10,OUTPUT);
digitalWrite(A10,HIGH);
pinMode(A11,OUTPUT);
digitalWrite(A11,HIGH);
pinMode(A12,OUTPUT);
digitalWrite(A12,HIGH);
pinMode(A13,OUTPUT);
digitalWrite(A13,HIGH);
pinMode(A14,OUTPUT);
digitalWrite(A14,HIGH);
pinMode(A15,OUTPUT);
digitalWrite(A15,HIGH);
pinMode(2,OUTPUT);
digitalWrite(2,HIGH);
for(int i=8;i<=12;i++){pinMode(i,OUTPUT);digitalWrite(i,HIGH);}
for(int i=22;i<=36;i=i+2){pinMode(i,OUTPUT);digitalWrite(i,HIGH);}
for(int i=39;i<=53;i++){pinMode(i,OUTPUT);digitalWrite(i,HIGH);}
pinMode(Flame_stepper3,OUTPUT);
digitalWrite(Flame_stepper3,HIGH);
pinMode(Flame_stepper4,OUTPUT);
digitalWrite(Flame_stepper4,HIGH);
//***********************///////////////////////////////

pinMode(test_pin,OUTPUT);
pinMode(Scale_en,OUTPUT);
//pinMode(V0_set_button,INPUT_PULLUP);
pinMode(Floater,INPUT_PULLUP);
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
pinMode(IR_sensor,INPUT); //pinMode(Hall_sensor,INPUT_PULLUP);
pinMode(Feeder_PWM,OUTPUT);
pinMode(Kiln_PWM,OUTPUT);
pinMode(Feeder_PWM_en,OUTPUT);
pinMode(Kiln_PWM_en,OUTPUT);

digitalWrite(Feeder_PWM_en,HIGH);
digitalWrite(Kiln_PWM_en,HIGH);
digitalWrite(Gas_valve,HIGH);
digitalWrite(Steam_pump,HIGH);
digitalWrite(Air_valve,HIGH);
digitalWrite(Gas_pump,HIGH);
digitalWrite(Feeder_PWM_en,HIGH);
digitalWrite(Kiln_PWM_en,HIGH);
digitalWrite(Water_inlet,HIGH);
digitalWrite(Steam_gen1,HIGH);
digitalWrite(Steam_gen2,HIGH);
digitalWrite(Steam_gen3,HIGH);
digitalWrite(Scale_en,HIGH);


// TIMER 3 for interrupt frequency 100 Hz:
TCCR3A = 0; // set entire TCCR1A register to 0
TCCR3B = 0; // same for TCCR1B
TCNT3  = 0; // initialize counter value to 0
// set compare match register for 100 Hz increments
OCR3A = 19999; // = 16000000 / (8 * 100) - 1 (must be <65536)
// turn on CTC mode
TCCR3B |= (1 << WGM32);
// Set CS12, CS11 and CS10 bits for 8 prescaler
TCCR3B |= (0 << CS32) | (1 << CS31) | (0 << CS30);
// enable timer compare interrupt
TIMSK3 |= (1 << OCIE3A);

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



EEPROM.get(address0,MQ2_V0);
EEPROM.get(address1,MQ3_V0);
EEPROM.get(address2,MQ4_V0);
EEPROM.get(address3,MQ7_V0);
EEPROM.get(address4,MQ9_V0);
EEPROM.get(address5,MQ135_V0);
EEPROM.get(address6,MQ214_V0);


Serial.begin(9600);
Serial1.begin(9600);

Serial.println("LABEL,Zaman,MQ2_volt,MQ3_volt,MQ4_volt,MQ7_volt,MQ9_volt,MQ135_volt,MQ214_volt,Temperature,Flame_mode,Feeder_RPM,Scale_Speed");//,Angle,MQ2_ratio,MQ3_ratio,MQ4_ratio,MQ7_ratio,MQ9_ratio,MQ135_ratio,MQ214_ratio"); //LABEL, so excel knows the next things will be the names of the columns
Serial.print("DATA,TIME,*,*,*,*,*,*,*,*,*,*,*,");
Serial.print(yangle);
Serial.print(",");
Serial.print(temperature_set);
Serial.print(",");
Serial.print(feeder_RPM_set);
Serial.print(",");
Serial.print(kiln_RPM_set);
Serial.print(",");
Serial.println(steam_mode);
 
Serial.println("RESETTIMER"); //resets timer to 0

digitalWrite(Gas_valve,HIGH);
digitalWrite(Steam_pump,LOW);
digitalWrite(Air_valve,HIGH);
digitalWrite(Gas_pump,HIGH);
digitalWrite(Feeder_PWM_en,HIGH);
digitalWrite(Kiln_PWM_en,HIGH);
set_steam_mode(LOW);

int b = kiln_RPM_set * 255 / 12;
OCR4AL = b;
int c = feeder_RPM_set * 255 / 34;
OCR4BL = c;






//************ Machine Pre-Heating Mode *************//
/*
//Turns Flame mode valve to 90deg
while(stepper_position < 88){
digitalWrite(Flame_stepper2,HIGH);digitalWrite(Flame_stepper1,LOW);
get_stepper_position();
}

//starts closing the valve until 45deg(start)
while(stepper_position > 45){
digitalWrite(Flame_stepper2,LOW);digitalWrite(Flame_stepper1,HIGH);
get_stepper_position(); 
}
//stops the valve
digitalWrite(Flame_stepper1,LOW);digitalWrite(Flame_stepper2,LOW);

while(temperature < 100){
  get_temperature();
  digitalWrite(test_pin,digitalRead(test_pin) ^ 1);
  delay(1000);
}

//starts opening the valve until 70deg
while(stepper_position < 70){
digitalWrite(Flame_stepper2,HIGH);digitalWrite(Flame_stepper1,LOW);
get_stepper_position(); 
}
//stops the valve
digitalWrite(Flame_stepper1,LOW);digitalWrite(Flame_stepper2,LOW);

while(temperature < 250){
  get_temperature();
  digitalWrite(test_pin,digitalRead(test_pin) ^ 1);
  delay(1000);  
}

//starts opening the valve until 90deg
while(stepper_position < 90){
digitalWrite(Flame_stepper2,HIGH);digitalWrite(Flame_stepper1,LOW);
get_stepper_position(); 
}
//stops the valve
digitalWrite(Flame_stepper1,LOW);digitalWrite(Flame_stepper2,LOW);

while(temperature < 400){
  get_temperature();
  digitalWrite(test_pin,digitalRead(test_pin) ^ 1);
  delay(1000);
}
*/

sei();
}








void loop() {
  k++;
  get_temperature();
  temperature_diff = temperature_set - temperature;
  
  if(millis() > (time_prev2 + 1000)){
  time_prev2 = millis(); 
  check_temperature();
  }
  
  get_stepper_position();
  flame_mode = stepper_position;
  if(stepper_position < 45 && temperature_diff < -2){digitalWrite(Flame_stepper1,LOW);digitalWrite(Flame_stepper2,LOW);}
  if(stepper_position - temperature_diff < 2 && stepper_position - temperature_diff > -2){digitalWrite(Flame_stepper1,LOW);digitalWrite(Flame_stepper2,LOW);}
  
  get_scale_data();
  get_MQ_data();
  send_data();
  /*
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
    
  }*/

  //delay(10);
}

void set_steam_mode(bool state){
  switch (steam_mode){
    case 1:
      digitalWrite(Steam_gen1,state);
      break;
    case 2:
      digitalWrite(Steam_gen1,state);
      digitalWrite(Steam_gen2,state);
      break;
    case 3:
      digitalWrite(Steam_gen1,state);
      digitalWrite(Steam_gen2,state);
      digitalWrite(Steam_gen3,state);
      break;
  }
}
void send_data(void){
  //cli(); //disable all interrupts         !!!!!!!!!!!!!!dar soorate niaz uncomment konid
  if(temperature_condition == 1){
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
  /*
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
  */
  Serial.print(temperature);
  Serial.print(",");
  Serial.print(flame_mode);
  Serial.print(",");
  Serial.print(feeder_RPM);
  Serial.print(",");
  Serial.print(scale_speed);
  Serial.print(",");
  Serial.print(OCR4BL);
  Serial.print(",");
  Serial.println(temperature_diff);
  }
  //sei(); //enable all interrupts       !!!!!!!!!!!!!!dar soorate niaz uncomment konid
}

void get_scale_data(void){
  if(millis() > (time_prev3 + 6700)){
  time_prev3 = millis(); 
  //while(Serial1.available()==0);
  digitalWrite(Scale_en,LOW);
  //if(Serial1.available()) scale_speed = Serial1.parseInt();
  digitalWrite(Scale_en,HIGH);
  } 
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
    
    /*MQ2_ratio = (MQ2_volt - MQ2_V0)/MQ2_V0;
    MQ3_ratio = (MQ3_volt - MQ3_V0)/MQ3_V0;
    MQ4_ratio = (MQ4_volt - MQ4_V0)/MQ4_V0;
    MQ7_ratio = (MQ7_volt - MQ7_V0)/MQ7_V0;
    MQ9_ratio = (MQ9_volt - MQ9_V0)/MQ9_V0;
    MQ135_ratio = (MQ135_volt - MQ135_V0)/MQ135_V0;
    MQ214_ratio = (MQ214_volt - MQ214_V0)/MQ214_V0;*/
    }
    
}



void get_MQ2_ratio(void){
  
    int sensorValue[median_number]; 
  // Get a average data by testing 100 times
    for(int x = 0 ; x < median_number ; x++)
    {
        sensorValue[x] = analogRead(MQ2);
    }
    sort_array(sensorValue,median_number);
    MQ2_volt = sensorValue[median_number/2]*5.0/1023;
     
}

void get_MQ3_ratio(void){
  
    int sensorValue[median_number]; 
  // Get a average data by testing 100 times
    for(int x = 0 ; x < median_number ; x++)
    {
        sensorValue[x] = analogRead(MQ3);
    }
    sort_array(sensorValue,median_number);
    MQ3_volt = sensorValue[median_number/2]*5.0/1023;
     
}


void get_MQ4_ratio(void){
  
    int sensorValue[median_number]; 
  // Get a average data by testing 100 times
    for(int x = 0 ; x < median_number ; x++)
    {
        sensorValue[x] = analogRead(MQ4);
    }
    sort_array(sensorValue,median_number);
    MQ4_volt = sensorValue[median_number/2]*5.0/1023;
     
}



void get_MQ7_ratio(void){
      
  int sensorValue[median_number];
  // Get a average data by testing 100 times
    for(int x = 0 ; x < median_number ; x++)
    {
        sensorValue[x] = analogRead(MQ7);
    }
    sort_array(sensorValue,median_number);
    MQ7_volt = sensorValue[median_number/2]*5.0/1023;
     
}


void get_MQ9_ratio(void){
  
    int sensorValue[median_number]; 
  // Get a average data by testing 100 times
    for(int x = 0 ; x < median_number ; x++)
    {
        sensorValue[x] = analogRead(MQ9);
    }
    sort_array(sensorValue,median_number);
    MQ9_volt = sensorValue[median_number/2]*5.0/1023;
     
}

void get_MQ135_ratio(void){
  
    int sensorValue[median_number]; 
  // Get a average data by testing 100 times
    for(int x = 0 ; x <  median_number ; x++)
    {
        sensorValue[x] = analogRead(MQ135);
    }
    sort_array(sensorValue,median_number);
    MQ135_volt = sensorValue[median_number/2]*5.0/1023;
     
}


void get_MQ214_ratio(void){
  
    int sensorValue[median_number]; 
  // Get a average data by testing 100 times
    for(int x = 0 ; x < median_number ; x++)
    {
        sensorValue[x] = analogRead(MQ214);
    }
    sort_array(sensorValue,median_number);
    MQ214_volt = sensorValue[median_number/2]*5.0/1023;
     
}


void get_temperature(void){
  int i;
  int temperature_array[median_number];
  for(i=0;i<median_number;i++){
  temperature_array[i] = analogRead(Thermocouple);  
  }
  sort_array(temperature_array,median_number);
  temperature = temperature_array[median_number/2];
}


void get_stepper_position(void){
  int j;
  int stepper_position_array[median_number];
  for(j=0;j<median_number;j++){
    stepper_position_array[j] = analogRead(Flame_mode_pot);                  //0.45v at 0deg = 92       4.63v at 90deg = 947     each step is 1.8deg
  }
  sort_array(stepper_position_array,median_number);
  stepper_position = map(stepper_position_array[median_number/2],205,532,0,90);
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



