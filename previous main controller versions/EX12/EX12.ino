#include <EEPROM.h>
#include "Wire.h"

#define MPU9250_ADDRESS 0x68
#define MPU9250_ACC_FULL_SCALE_2_G 0x00

#define MQ2 A6
#define MQ3 A5
#define MQ4 A4
#define MQ7 A3
#define MQ9 A2
#define MQ135 A1
#define MQ214 A0

#define V0_set_button 23   // button used for setting all V0 values
//#define next_button 22     // button used for switching between display

//bool V0_set_state = false;
//Values for Angle
float yangle;
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
int count_display = 0;
unsigned int menu = 0;

//EEPROM addresses
const int address0 = 0;
const int address1 = address0+sizeof(float);
const int address2 = address1+sizeof(float);
const int address3 = address2+sizeof(float);
const int address4 = address3+sizeof(float);
const int address5 = address4+sizeof(float);
const int address6 = address5+sizeof(float);

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

void setup() {
  // put your setup code here, to run once:
EEPROM.get(address0,MQ2_V0);
EEPROM.get(address1,MQ3_V0);
EEPROM.get(address2,MQ4_V0);
EEPROM.get(address3,MQ7_V0);
EEPROM.get(address4,MQ9_V0);
EEPROM.get(address5,MQ135_V0);
EEPROM.get(address6,MQ214_V0);

Serial.begin(9600);

pinMode(V0_set_button,INPUT_PULLUP);

Wire.begin();
I2CwriteByte(MPU9250_ADDRESS, 29, 0x06);
I2CwriteByte(MPU9250_ADDRESS, 28, MPU9250_ACC_FULL_SCALE_2_G);
I2CwriteByte(MPU9250_ADDRESS, 108, 0x2F);
  
Serial.println("CLEARDATA"); //clears up any data left from previous projects
Serial.println("LABEL,Zaman,MQ2_volt,MQ3_volt,MQ4_volt,MQ7_volt,MQ9_volt,MQ135_volt,MQ214_volt,MQ2_ratio,MQ3_ratio,MQ4_ratio,MQ7_ratio,MQ9_ratio,MQ135_ratio,MQ214_ratio"); //LABEL, so excel knows the next things will be the names of the columns
Serial.println("RESETTIMER"); //resets timer to 0

}

void loop() {
  // put your main code here, to run repeatedly:
  //if(digitalRead(V0_set_button)==LOW && V0_set_state==true) {
    if(digitalRead(V0_set_button)==LOW) {
    while(digitalRead(V0_set_button)==LOW);
    //lcd.clear();
    //lcd.print("V0 values saved !");
    //Serial.println("V0 values saved !");

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
    
  //get the values :
  
    get_MQ2_ratio();
    Serial.print("DATA,TIME,");
    Serial.print(MQ2_volt);
    get_MQ3_ratio();
    Serial.print(",");
    Serial.print(MQ3_volt);
    get_MQ4_ratio();
    Serial.print(",");
    Serial.print(MQ4_volt);
    get_MQ7_ratio();
    Serial.print(",");
    Serial.print(MQ7_volt);
    get_MQ9_ratio();
    Serial.print(",");
    Serial.print(MQ9_volt);
    get_MQ135_ratio();
    Serial.print(",");
    Serial.print(MQ135_volt);
    get_MQ214_ratio();
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
    Serial.println(MQ214_ratio);
    
    //Serial.print("DATA,",MQ2_volt,",",MQ3_volt,",",MQ4_volt,",",MQ7_volt,",",MQ9_volt,",",MQ135_volt,",",MQ214_volt,",",MQ2_ratio,",",MQ3_ratio,",",MQ4_ratio,",",MQ7_ratio,",",MQ9_ratio,",",MQ135_ratio,",",MQ214_ratio);
    //Serial.println();
    delay(100);
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
    MQ2_ratio = (MQ2_volt - MQ2_V0)/MQ2_V0; 
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
    MQ3_ratio = (MQ3_volt - MQ3_V0)/MQ3_V0; 
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
    MQ4_ratio = (MQ4_volt - MQ4_V0)/MQ4_V0; 
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
    MQ7_ratio = (MQ7_volt - MQ7_V0)/MQ7_V0; 
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
    MQ9_ratio = (MQ9_volt - MQ9_V0)/MQ9_V0; 
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
    MQ135_ratio = (MQ135_volt - MQ135_V0)/MQ135_V0; 
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
    MQ214_ratio = (MQ214_volt - MQ214_V0)/MQ214_V0; 
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






/*lcd.clear();
  I2Cread(MPU9250_ADDRESS, 0x3B, 14, Buf);
  int16_t ay = -(Buf[2] << 8 | Buf[3]);
  yangle = ay / 182.04;  
  //yangle2 = int(yangle * 10);
  lcd.print("Angle = ");
  lcd.print(yangle);
  lcd.setCursor(0,1);
  yangle = float(int(yangle*2)) / 2;
  lcd.print("Angle = ");
  lcd.print(yangle);
  delay(100);*/

