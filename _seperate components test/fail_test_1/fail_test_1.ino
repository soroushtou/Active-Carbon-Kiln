#define test_pin 12
#define Flame_mode_pot A9
#define Thermocouple A8
#define Flame_stepper1 14
#define Flame_stepper2 15
#define Flame_stepper3 16
#define Flame_stepper4 17
#define MQ2 A2
#define MQ3 A6
#define MQ4 A4
#define MQ7 A5
#define MQ9 A1
#define MQ135 A0
#define MQ214 A3

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


float feeder_RPM_set = 30;   // 0.00 - 34.00 RPM
float kiln_RPM_set = 12;     // 0.00 - 12.00 RPM
int temperature_set = 90;    // 0 to 1023 C
int steam_mode = 1;         // 1 , 2 , 3

int temperature,temperature_array[51],temperature_counter,floater_counter,stepper_position,flame_mode_array[51],flame_mode,scale_speed;
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

void setup() {
  // put your setup code here, to run once:
pinMode(test_pin,OUTPUT);
Serial.begin(9600);
Serial.println("CLEARDATA"); //clears up any data left from previous projects
Serial.println("RESETTIMER"); //resets timer to 0

}

void loop() {
  // put your main code here, to run repeatedly:
    digitalWrite(test_pin,LOW);
  
  int i;
  for(i=0;i<51;i++){
  temperature_array[i] = analogRead(Thermocouple);  
  }
  //analogWrite(Kiln_PWM,200);
  //analogWrite(Feeder_PWM,200);
  // put your main code here, to run repeatedly:
  sort_array(temperature_array,51);
  temperature = temperature_array[25];
  int temperature_diff = temperature_set - temperature;
  stepper_position = analogRead(Flame_mode_pot);                  //0.6v at 0deg = 122.76       4.2v at 90deg = 859.32     each step is 1.8deg
  stepper_position = map(stepper_position,122,860,0,50);              //note that 200 steps = 360deg so 90deg = 50steps
 
  if(temperature_diff >= 100)one_step(9 - stepper_position);                             //maximum : goes to 90deg  
  if(temperature_diff >= 60 && temperature_diff < 100)one_step(7 - stepper_position);    //          goes to 77deg  
  if(temperature_diff >= 20 && temperature_diff < 60)one_step(6 - stepper_position);     //          goes to 64deg
  if(temperature_diff < 20 && temperature_diff > -20);                                   //does not change the flame_mode
  if(temperature_diff <= -20 && temperature_diff > -60)one_step(5 - stepper_position);   //          goes to 53deg
  if(temperature_diff <= -60 && temperature_diff > -100)one_step(4 - stepper_position);  //          goes to 40deg
  if(temperature_diff <= -100)one_step(2 - stepper_position);                            //minimum : goes to 27deg
 

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
  digitalWrite(test_pin,HIGH);
  delay(10);
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
  Serial.print(",");*/
  Serial.print(temperature);
  Serial.print(",");
  Serial.print(flame_mode);
  Serial.print(",");
  Serial.print(feeder_RPM);
  Serial.print(",");
  Serial.println(scale_speed);

  sei(); //enable all interrupts       !!!!!!!!!!!!!!dar soorate niaz uncomment konid
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
