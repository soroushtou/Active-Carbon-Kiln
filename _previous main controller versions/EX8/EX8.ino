#include <EEPROM.h>
#include <LiquidCrystal.h>

const int RS = 2;
const int E = 3;
const int D4 = 4;
const int D5 = 5;
const int D6 = 6;
const int D7 = 7;

#define V0_set_button 9   // button used for setting all V0 values
#define next_button 8     // button used for switching between display

LiquidCrystal lcd(RS,E,D4,D5,D6,D7);

bool V0_set_state = false;

//Values for MQ7
float MQ7_V0;  // Get the value of V0 via in Air
double MQ7_ratio; // Get ratio ( MQ2_volt - MQ2_V0 ) / MQ2_V0
float MQ7_ppm;
float MQ7_volt;

//Values for MQ2
float MQ2_V0;  // Get the value of V0 via in Air
double MQ2_ratio; // Get ratio ( MQ2_volt - MQ2_V0 ) / MQ2_V0
float MQ2_ppm;
float MQ2_volt;

//Values for display
int count_display = 0;
unsigned int menu = 0;

//EEPROM addresses
const int address0 = 0;
const int address1 = address0+sizeof(float);

//Display function attach
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
EEPROM.get(address0,MQ2_V0);
EEPROM.get(address1,MQ7_V0);

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

Serial.begin(9600);

pinMode(V0_set_button,INPUT_PULLUP);
pinMode(next_button,INPUT_PULLUP);
lcd.begin(16, 2);

lcd.print("Starting ...");
Serial.println("Starting ...");

}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(V0_set_button)==LOW && V0_set_state==true) {
    while(digitalRead(V0_set_button)==LOW);
    V0_set_state = false;
    lcd.clear();
    lcd.print("V0 values saved !");
    Serial.println("V0 values saved !");
    EEPROM.put(address0,MQ2_V0);
    EEPROM.put(address1,MQ7_V0);
  }
  
  if(digitalRead(V0_set_button)==LOW && V0_set_state==false) {
    while(digitalRead(V0_set_button)==LOW);
    V0_set_state = true;
  }  

  if(digitalRead(next_button)==LOW) {
    while(digitalRead(next_button)==LOW);
    menu++;
  }

}

//Display function : 0 for MQ2 1 for MQ7
// *****IMPORTANT***** : The display function is also used for getting the values from sensors
void display_lcd(void){

  if(V0_set_state == false){
    
    //get the values :
    get_MQ7_ratio();
    get_MQ2_ratio();

    lcd.clear();
    if(menu>2) menu=0;
    switch(menu){
      
      case 0:
      lcd.print(MQ2_volt);
      lcd.setCursor(5,0);
      lcd.print(MQ7_volt);
      break;
      
      case 1:
      lcd.print("MQ2 for smoke:");
      lcd.setCursor(0,1);
      lcd.print(MQ2_volt);
      lcd.setCursor(5,1);
      lcd.print("Ratio=");
      lcd.setCursor(11,1);
      lcd.print(MQ2_ratio);
      /*
      Serial.println("MQ2 for smoke:");
      Serial.print(MQ2_ppm);
      Serial.println("ppm");
      */
      break;
      
      case 2:
      lcd.print("MQ7 for CO:");
      lcd.setCursor(0,1);
      lcd.print(MQ7_volt);
      lcd.setCursor(5,1);
      lcd.print("Ratio=");
      lcd.setCursor(11,1);
      lcd.print(MQ7_ratio);
      /*
      Serial.println("MQ7 for CO:");
      Serial.print(MQ7_ppm);
      Serial.println("ppm");
      */
      break;
    }
  }
  
  if(V0_set_state == true){
    
    //get the values :
    get_MQ7_V0();
    get_MQ2_V0();

    lcd.clear();
    if(menu>2) menu=0;
    switch(menu){

      case 0:
      lcd.print(MQ2_V0);
      lcd.setCursor(5,0);
      lcd.print(MQ7_V0);
      break;
      
      case 1:
      lcd.print("New V0 for MQ2:");
      lcd.setCursor(0,1);
      lcd.print(MQ2_V0);
      /*
      Serial.println("New V0 for MQ2:");
      Serial.println(MQ2_V0);
      */
      break;
      
      case 2:
      lcd.print("New V0 for MQ7:");
      lcd.setCursor(0,1);
      lcd.print(MQ7_V0);
      /*
      Serial.println("New V0 for MQ7:");
      Serial.println(MQ7_V0);
      */
      break;
    }
  }

}

void get_MQ7_V0(void){
    
    int sensorValue[101];
  // Get a average data by testing 101 times and pick the Median :
    for(int x = 0 ; x < 101 ; x++)
    {
        sensorValue[x] = analogRead(A1);
    }
    sort_array(sensorValue,101);
    MQ7_V0 = sensorValue[50]*5.0/1023; 
}

void get_MQ7_ratio(void){
      
  int sensorValue[101];
  // Get a average data by testing 100 times
    for(int x = 0 ; x < 100 ; x++)
    {
        sensorValue[x] = analogRead(A1);
    }
    sort_array(sensorValue,101);
    MQ7_volt = sensorValue[50]*5.0/1023;
    MQ7_ratio = (MQ7_volt - MQ7_V0)/MQ7_V0; 
}

void get_MQ2_V0(void){
    
    int sensorValue[101];
  // Get a average data by testing 101 times and pick the Median :
    for(int x = 0 ; x < 101 ; x++)
    {
        sensorValue[x] = analogRead(A0);
    }
    sort_array(sensorValue,101);
    MQ2_V0 = sensorValue[50]*5.0/1023; 
}

void get_MQ2_ratio(void){
  
    int sensorValue[101]; 
  // Get a average data by testing 100 times
    for(int x = 0 ; x < 100 ; x++)
    {
        sensorValue[x] = analogRead(A0);
    }
    sort_array(sensorValue,101);
    MQ2_volt = sensorValue[50]*5.0/1023;
    MQ2_ratio = (MQ2_volt - MQ2_V0)/MQ2_V0; 
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

