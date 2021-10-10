#include <EEPROM.h>

/*#include <LiquidCrystal.h>

const int RS = 2;
const int E = 3;
const int D4 = 4;
const int D5 = 5;
const int D6 = 6;
const int D7 = 7;
*/

#define MQ2 A6
#define MQ3 A5
#define MQ4 A4
#define MQ7 A3
#define MQ9 A2
#define MQ135 A1
#define MQ214 A0

#define V0_set_button 23   // button used for setting all V0 values
//#define next_button 22     // button used for switching between display

/*LiquidCrystal lcd(RS,E,D4,D5,D6,D7);*/

//bool V0_set_state = false;


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

/*
//Display function attach
ISR(TIMER2_COMPA_vect){
   //interrupt commands for TIMER 2 here
   
   count_display ++;
   if(count_display==250) {
    display_lcd();
    count_display = 0;
   }
}
*/

void setup() {
  // put your setup code here, to run once:
EEPROM.get(address0,MQ2_V0);
EEPROM.get(address1,MQ3_V0);
EEPROM.get(address2,MQ4_V0);
EEPROM.get(address3,MQ7_V0);
EEPROM.get(address4,MQ9_V0);
EEPROM.get(address5,MQ135_V0);
EEPROM.get(address6,MQ214_V0);

/*
// TIMER 2 used for display 
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
*/

Serial.begin(9600);

pinMode(V0_set_button,INPUT_PULLUP);
/*pinMode(next_button,INPUT_PULLUP);
lcd.begin(16, 2);*/

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

  /*
  if(digitalRead(V0_set_button)==LOW && V0_set_state==false) {
    while(digitalRead(V0_set_button)==LOW);
    V0_set_state = true;
    delay(10);
  } */ 
  /*
  if(digitalRead(next_button)==LOW) {
    while(digitalRead(next_button)==LOW);
    menu++;
    delay(10);
  }*/
    
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

/*
//Display function : 0 for MQ2 1 for MQ7
// *****IMPORTANT***** : The display function is also used for getting the values from sensors
void display_lcd(void){

  if(V0_set_state == false){
    
    lcd.clear();
    if(menu>7) menu=0;
    switch(menu){
      
      case 0:
      lcd.print(int(MQ2_volt*100));
      lcd.setCursor(4,0);
      lcd.print(int(MQ3_volt*100));
      lcd.setCursor(8,0);
      lcd.print(int(MQ4_volt*100));
      lcd.setCursor(12,0);
      lcd.print(int(MQ7_volt*100));
      lcd.setCursor(0,1);
      lcd.print(int(MQ9_volt*100));
      lcd.setCursor(4,1);
      lcd.print(int(MQ135_volt*100));
      lcd.setCursor(8,1);
      lcd.print(int(MQ214_volt*100));
      break;
      
      case 1:
      lcd.print("MQ2 for Smoke:");
      lcd.setCursor(0,1);
      lcd.print(MQ2_volt);
      lcd.setCursor(5,1);
      lcd.print("Ratio=");
      lcd.setCursor(11,1);
      lcd.print(MQ2_ratio);
      break;

      case 2:
      lcd.print("MQ3 for Alcohol:");
      lcd.setCursor(0,1);
      lcd.print(MQ3_volt);
      lcd.setCursor(5,1);
      lcd.print("Ratio=");
      lcd.setCursor(11,1);
      lcd.print(MQ3_ratio);
      break;

      case 3:
      lcd.print("MQ4 for CNG:");
      lcd.setCursor(0,1);
      lcd.print(MQ4_volt);
      lcd.setCursor(5,1);
      lcd.print("Ratio=");
      lcd.setCursor(11,1);
      lcd.print(MQ4_ratio);
      break;
      
      case 4:
      lcd.print("MQ7 for CO:");
      lcd.setCursor(0,1);
      lcd.print(MQ7_volt);
      lcd.setCursor(5,1);
      lcd.print("Ratio=");
      lcd.setCursor(11,1);
      lcd.print(MQ7_ratio);
      break;

      case 5:
      lcd.print("MQ9 for Combust:");
      lcd.setCursor(0,1);
      lcd.print(MQ9_volt);
      lcd.setCursor(5,1);
      lcd.print("Ratio=");
      lcd.setCursor(11,1);
      lcd.print(MQ9_ratio);
      break;

      case 6:
      lcd.print("MQ135 for Air Q:");
      lcd.setCursor(0,1);
      lcd.print(MQ135_volt);
      lcd.setCursor(5,1);
      lcd.print("Ratio=");
      lcd.setCursor(11,1);
      lcd.print(MQ135_ratio);
      break;

      case 7:
      lcd.print("MQ214 for Natur:");
      lcd.setCursor(0,1);
      lcd.print(MQ214_volt);
      lcd.setCursor(5,1);
      lcd.print("Ratio=");
      lcd.setCursor(11,1);
      lcd.print(MQ214_ratio);
      break;
    }
  }

  
  if(V0_set_state == true){
    
    //get the values :
    get_MQ2_V0();
    get_MQ3_V0();
    get_MQ4_V0();
    get_MQ7_V0();
    get_MQ9_V0();
    get_MQ135_V0();
    get_MQ214_V0();

    lcd.clear();
    if(menu>7) menu=0;
    switch(menu){

      case 0:
      lcd.print(int(MQ2_V0*100));
      lcd.setCursor(4,0);
      lcd.print(int(MQ3_V0*100));
      lcd.setCursor(8,0);
      lcd.print(int(MQ4_V0*100));
      lcd.setCursor(12,0);
      lcd.print(int(MQ7_V0*100));
      lcd.setCursor(0,1);
      lcd.print(int(MQ9_V0*100));
      lcd.setCursor(4,1);
      lcd.print(int(MQ135_V0*100));
      lcd.setCursor(8,1);
      lcd.print(int(MQ214_V0*100));
      break;
      
      case 1:
      lcd.print("New V0 for MQ2:");
      lcd.setCursor(0,1);
      lcd.print(MQ2_V0);
      break;

      case 2:
      lcd.print("New V0 for MQ3:");
      lcd.setCursor(0,1);
      lcd.print(MQ3_V0);
      break;

      case 3:
      lcd.print("New V0 for MQ4:");
      lcd.setCursor(0,1);
      lcd.print(MQ4_V0);
      break;
      
      case 4:
      lcd.print("New V0 for MQ7:");
      lcd.setCursor(0,1);
      lcd.print(MQ7_V0);
      break;
      
      case 5:
      lcd.print("New V0 for MQ9:");
      lcd.setCursor(0,1);
      lcd.print(MQ9_V0);
      break;

      case 6:
      lcd.print("New V0 for MQ135:");
      lcd.setCursor(0,1);
      lcd.print(MQ135_V0);
      break;

      case 7:
      lcd.print("New V0 for MQ214:");
      lcd.setCursor(0,1);
      lcd.print(MQ214_V0);
      break;
    }
  }

}
*/
/*void get_MQ2_V0(void){
    
    int sensorValue[101];
  // Get a average data by testing 101 times and pick the Median :
    for(int x = 0 ; x < 101 ; x++)
    {
        sensorValue[x] = analogRead(MQ2);
    }
    sort_array(sensorValue,101);
    MQ2_V0 = sensorValue[50]*5.0/1023; 
}*/

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

/*void get_MQ3_V0(void){
    
    int sensorValue[101];
  // Get a average data by testing 101 times and pick the Median :
    for(int x = 0 ; x < 101 ; x++)
    {
        sensorValue[x] = analogRead(MQ3);
    }
    sort_array(sensorValue,101);
    MQ3_V0 = sensorValue[50]*5.0/1023; 
}*/

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

/*void get_MQ4_V0(void){
    
    int sensorValue[101];
  // Get a average data by testing 101 times and pick the Median :
    for(int x = 0 ; x < 101 ; x++)
    {
        sensorValue[x] = analogRead(MQ4);
    }
    sort_array(sensorValue,101);
    MQ4_V0 = sensorValue[50]*5.0/1023; 
}*/

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

/*void get_MQ7_V0(void){
    
    int sensorValue[101];
  // Get a average data by testing 101 times and pick the Median :
    for(int x = 0 ; x < 101 ; x++)
    {
        sensorValue[x] = analogRead(MQ7);
    }
    sort_array(sensorValue,101);
    MQ7_V0 = sensorValue[50]*5.0/1023; 
}*/

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

/*void get_MQ9_V0(void){
    
    int sensorValue[101];
  // Get a average data by testing 101 times and pick the Median :
    for(int x = 0 ; x < 101 ; x++)
    {
        sensorValue[x] = analogRead(MQ9);
    }
    sort_array(sensorValue,101);
    MQ9_V0 = sensorValue[50]*5.0/1023; 
}*/

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

/*void get_MQ135_V0(void){
    
    int sensorValue[101];
  // Get a average data by testing 101 times and pick the Median :
    for(int x = 0 ; x < 101 ; x++)
    {
        sensorValue[x] = analogRead(MQ135);
    }
    sort_array(sensorValue,101);
    MQ135_V0 = sensorValue[50]*5.0/1023; 
}*/

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

/*void get_MQ214_V0(void){
    
    int sensorValue[101];
  // Get a average data by testing 101 times and pick the Median :
    for(int x = 0 ; x < 101 ; x++)
    {
        sensorValue[x] = analogRead(MQ214);
    }
    sort_array(sensorValue,101);
    MQ214_V0 = sensorValue[50]*5.0/1023; 
}*/

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

