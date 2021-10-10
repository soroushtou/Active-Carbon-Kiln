#include <EEPROM.h>
#include <LiquidCrystal.h>

const int RS = 2;
const int E = 3;
const int D4 = 4;
const int D5 = 5;
const int D6 = 6;
const int D7 = 7;

#define reset_button 9
#define next_button 8

LiquidCrystal lcd(RS,E,D4,D5,D6,D7);

bool save_r0 = false;
float R0_MQ7;  // Get the value of R0 via in H2
//float R0_MQ7_temp;
float ratio_MQ7; // Get ratio RS_GAS/RS_air
float MQ7_ppm;
float R0_MQ2;  // Get the value of R0 via in H2
//float R0_MQ2_temp;
float ratio_MQ2; // Get ratio RS_GAS/RS_air
float MQ2_ppm;
int count_display = 0;
unsigned int menu = 0;
const int address0 = 0;
const int address1 = address0+sizeof(float);

ISR(TIMER2_COMPA_vect){
   //interrupt commands for TIMER 2 here
   
   count_display ++;
   if(count_display==250) {
    display_lcd();
    count_display = 0;
   }
}



void setup() {
  // put your setup code here, to run once:
EEPROM.get(address0,R0_MQ2);
EEPROM.get(address1,R0_MQ7);

  
// TIMER 2 for interrupt frequency 1000 Hz:
cli(); // stop interrupts
TCCR2A = 0; // set entire TCCR2A register to 0
TCCR2B = 0; // same for TCCR2B
TCNT2  = 0; // initialize counter value to 0
// set compare match register for 1000 Hz increments
OCR2A = 249; // = 16000000 / (64 * 1000) - 1 (must be <256)
// turn on CTC mode
TCCR2B |= (1 << WGM21);
// Set CS22, CS21 and CS20 bits for 64 prescaler
TCCR2B |= (1 << CS22) | (0 << CS21) | (0 << CS20);
// enable timer compare interrupt
TIMSK2 |= (1 << OCIE2A);
sei(); // allow interrupts

Serial.begin(9600);

pinMode(reset_button,INPUT_PULLUP);
pinMode(next_button,INPUT_PULLUP);
lcd.begin(16, 2);

lcd.println("Hello");
Serial.print("Hello");

}

void loop() {
  // put your main code here, to run repeatedly:
if(digitalRead(reset_button)==LOW && save_r0==true) {
  while(digitalRead(reset_button)==LOW);
  save_r0 = false;
  //R0_MQ2=R0_MQ2_temp;
  //R0_MQ7=R0_MQ7_temp;
  lcd.clear();
  lcd.print("R0 values saved !");
  Serial.println("R0 values saved !");
  EEPROM.put(address0,R0_MQ2);
  EEPROM.put(address1,R0_MQ7);
  }
if(digitalRead(reset_button)==LOW && save_r0==false) {
  while(digitalRead(reset_button)==LOW);
  save_r0 = true;
  }  

if(digitalRead(next_button)==LOW) {
  while(digitalRead(next_button)==LOW);
  menu++;
}

if(save_r0 == true){
  get_R0_MQ7();
  get_R0_MQ2();
}
else{
  get_ratio_MQ7();
  get_ratio_MQ2();
  MQ7_ppm = log(ratio_MQ7);
  MQ7_ppm -= 1.38;
  MQ7_ppm /= -0.743;
  MQ7_ppm = pow(10 ,MQ7_ppm);
  MQ2_ppm = log(ratio_MQ2);
  MQ2_ppm -= 1.57;
  MQ2_ppm /= -0.447;
  MQ2_ppm = pow(10 ,MQ2_ppm);  
}

}

void display_lcd(void){
  lcd.clear();
  if(save_r0 == false){
    if(menu>1) menu=0;
    switch(menu){
      case 0:
      lcd.print("MQ2 for smoke:");
      lcd.setCursor(0,1);
      lcd.print(MQ2_ppm);
      lcd.print("ppm");
      lcd.setCursor(11,1);
      lcd.print(ratio_MQ2);
      Serial.println("MQ2 for smoke:");
      Serial.print(ratio_MQ2);
      Serial.println("ppm");
      break;
      
      case 1:
      lcd.print("MQ7 for CO:");
      lcd.setCursor(0,1);
      lcd.print(MQ7_ppm);
      lcd.print("ppm");
      lcd.setCursor(11,1);
      lcd.print(ratio_MQ7);
      Serial.println("MQ7 for CO:");
      Serial.print(MQ7_ppm);
      Serial.println("ppm");
      break;
    }
  }
  if(save_r0 == true){
    
    if(menu>1) menu=0;
    switch(menu){
      
      case 0:
      lcd.print("New R0 for MQ2:");
      lcd.setCursor(0,1);
      lcd.print(R0_MQ2);
      Serial.println("New R0 for MQ2:");
      Serial.println(R0_MQ2);
      break;
      
      case 1:
      lcd.print("New R0 for MQ7:");
      lcd.setCursor(0,1);
      lcd.print(R0_MQ7);
      Serial.println("New R0 for MQ7:");
      Serial.println(R0_MQ7);
      break;
  }
}

}

void get_R0_MQ7(void){
    
    float sensor_volt;
    float RS_air; //  Get the value of RS via in a clear air
    float sensorValue = 0.0;

  // Get a average data by testing 100 times
    for(int x = 0 ; x < 100 ; x++)
    {
        sensorValue = sensorValue + analogRead(A1);
    }
    sensorValue = sensorValue/100.0;

    sensor_volt = sensorValue/1024*5.0;
    RS_air = (5.0-sensor_volt)/sensor_volt; // omit * RL
    R0_MQ7 = RS_air/9.8; // The ratio of RS/R0 is 9.8 in a clear air from Graph (Found using WebPlotDigitizer)
}

void get_ratio_MQ7(void){
      
    float sensor_volt;
    float RS_gas; // Get value of RS in a GAS
    int sensorValue = analogRead(A1);
    sensor_volt=(float)sensorValue/1024*5.0;
    RS_gas = (5.0-sensor_volt)/sensor_volt; // omit * RL

    ratio_MQ7 = RS_gas/R0_MQ7;  // ratio = RS/R0
}

void get_R0_MQ2(void){
    
    float sensor_volt;
    float RS_air; //  Get the value of RS via in a clear air
    float sensorValue = 0.0;

  // Get a average data by testing 100 times
    for(int x = 0 ; x < 100 ; x++)
    {
        sensorValue = sensorValue + analogRead(A0);
    }
    sensorValue = sensorValue/100.0;

    sensor_volt = sensorValue/1024*5.0;
    RS_air = (5.0-sensor_volt)/sensor_volt; // omit * RL
    R0_MQ2 = RS_air/9.8; // The ratio of RS/R0 is 9.8 in a clear air from Graph (Found using WebPlotDigitizer)
}

void get_ratio_MQ2(void){
      
    float sensor_volt;
    float RS_gas; // Get value of RS in a GAS
    int sensorValue = analogRead(A0);
    sensor_volt=(float)sensorValue/1024*5.0;
    RS_gas = (5.0-sensor_volt)/sensor_volt; // omit * RL

    ratio_MQ2 = RS_gas/R0_MQ2;  // ratio = RS/R0
}

