#include <EEPROM.h>
#include "Wire.h"
//#include <Stepper.h>

float feeder_RPM_set = 30;   // 0.00 - 34.00 RPM
float kiln_RPM_set = 10;     // 0.00 - 12.00 RPM
int temperature_set = 90;    // 0 to 1023 C
int steam_mode = 1;         // 1 , 2 , 3

//const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution for your motor

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

//#define V0_set_button 13   // button used for setting all V0 values

int temperature,temperature_counter,floater_counter,stepper_position,flame_mode,scale_speed;
bool temperature_condition;
unsigned long long time_prev,time_diff;
//bool V0_set_state = false;
//Values for Angle
float yangle,feeder_RPM;
//int yangle2;


void write_stepper(int a,int b,int c,int d){
digitalWrite(Flame_stepper1,a);
digitalWrite(Flame_stepper2,b);
digitalWrite(Flame_stepper3,c);
digitalWrite(Flame_stepper4,d);
}
void one_step(int steps){
if(steps > 0){
for(int i=0;i<steps;i++){
write_stepper(1,0,0,0);
delay(10);
write_stepper(1,1,0,0);
delay(10);
write_stepper(0,1,0,0);
delay(10);
write_stepper(0,1,1,0);
delay(10);
write_stepper(0,0,1,0);
delay(10);
write_stepper(0,0,1,1);
delay(10);
write_stepper(0,0,0,1);
delay(10);
write_stepper(1,0,0,1);
delay(10);
}
write_stepper(0,0,0,0);
}
if(steps < 0){
for(int i=0;i>steps;i--){
write_stepper(0,0,0,1);
delay(10);
write_stepper(0,0,1,1);
delay(10);
write_stepper(0,0,1,0);
delay(10);
write_stepper(0,1,1,0);
delay(10);
write_stepper(0,1,0,0);
delay(10);
write_stepper(1,1,0,0);
delay(10);
write_stepper(1,0,0,0);
delay(10);
write_stepper(1,0,0,1);
delay(10);
}
write_stepper(0,0,0,0);
}
}

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

// initialize the stepper library on pins 8 through 11:
//Stepper myStepper(stepsPerRevolution, Flame_stepper1, Flame_stepper2, Flame_stepper3, Flame_stepper4);

// Timer5 overflow interrupt service routine
ISR(TIMER5_OVF_vect) {
// Reinitialize Timer3 value

TCNT5H=0x600 >> 8;
TCNT5L=0x600 & 0xff;
digitalWrite(test_pin,digitalRead(test_pin) ^ 1);
temperature_counter ++;
int i;
int stepper_position_array[11];
if(temperature - temperature_set >= -50 && temperature - temperature_set <= 50){ 
  if(temperature_counter == 1){set_steam_mode(HIGH);}
  if(temperature_counter == 21){digitalWrite(Steam_pump,HIGH);}
  if(temperature_counter == 31){digitalWrite(Air_valve,HIGH);digitalWrite(Gas_valve,LOW);}
  if(temperature_counter == 61){digitalWrite(Gas_valve,HIGH);digitalWrite(Air_valve,LOW);digitalWrite(Steam_pump,LOW);set_steam_mode(LOW);}
}


if(temperature_counter >= 10){
  int temperature_diff = temperature_set - temperature;
  for(i=0;i<11;i++){
    stepper_position_array[11] = analogRead(Flame_mode_pot);                  //0.6v at 0deg = 122.76       4.2v at 90deg = 859.32     each step is 1.8deg
  }
  sort_array(stepper_position_array,11);
  stepper_position = stepper_position_array[5];
  //stepper_position = map(stepper_position,122,860,0,50);              //note that 200 steps = 360deg so 90deg = 50steps
 
  if(temperature_diff >= 100)one_step(9 - stepper_position);                             //maximum : goes to 90deg  
  if(temperature_diff >= 60 && temperature_diff < 100)one_step(7 - stepper_position);    //          goes to 77deg  
  if(temperature_diff >= 20 && temperature_diff < 60)one_step(6 - stepper_position);     //          goes to 64deg
  if(temperature_diff < 20 && temperature_diff > -20);                                   //does not change the flame_mode
  if(temperature_diff <= -20 && temperature_diff > -60)one_step(5 - stepper_position);   //          goes to 53deg
  if(temperature_diff <= -60 && temperature_diff > -100)one_step(4 - stepper_position);  //          goes to 40deg
  if(temperature_diff <= -100)one_step(2 - stepper_position);                             //minimum : goes to 27deg
  temperature_counter = 0;
  }
  
for(i=0;i<11;i++){
    stepper_position_array[11] = analogRead(Flame_mode_pot);                  //0.6v at 0deg = 122.76       4.2v at 90deg = 859.32     each step is 1.8deg
  }
  sort_array(stepper_position_array,11);
  flame_mode = stepper_position_array[5];      //shows the new position
  

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
  //OCR4BL = a;                      //lazeme ke tabdilesh konim be inr ya char ???
}


void setup() {
cli();
pinMode(test_pin,OUTPUT);
// Timer/Counter 5 initialization
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
TCCR5A=(0<<COM5A1) | (0<<COM5A0) | (0<<COM5B1) | (0<<COM5B0) | (0<<COM5C1) | (0<<COM5C0) | (0<<WGM51) | (0<<WGM50);
TCCR5B=(0<<ICNC5) | (0<<ICES5) | (0<<WGM53) | (0<<WGM52) | (1<<CS52) | (0<<CS51) | (0<<CS50);
TCNT5H=0x06;
TCNT5L=0x00;
ICR5H=0x00;
ICR5L=0x00;
OCR5AH=0x00;
OCR5AL=0x00;
OCR5BH=0x00;
OCR5BL=0x00;
OCR5CH=0x00;
OCR5CL=0x00;


//TIMSK5 |= (1<<OCIE5A);              // <------------------------------- TTTHHHEEEEEEEEEEE BBBBBBBBBBBUUUUUUUUUUUUUUUUUUUUUUUUUGGGGGGGGG

// Timer/Counter 5 Interrupt(s) initialization
TIMSK5=(0<<ICIE5) | (0<<OCIE5C) | (0<<OCIE5B) | (0<<OCIE5A) | (1<<TOIE5);
sei();
}
void loop() {
  // put your main code here, to run repeatedly:
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
