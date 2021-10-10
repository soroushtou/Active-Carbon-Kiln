//position can be vaaried from 0 to 90 degrees
//steps can be varied from -50 to +50
#define ACoil 14
#define BCoil 15
#define CCoil 16
#define DCoil 17
//#define NUMBER_OF_STEPS_PER_REV 512
int last_position =0;



void setup() {
  // put your setup code here, to run once:
pinMode(ACoil,OUTPUT);
pinMode(BCoil,OUTPUT);
pinMode(CCoil,OUTPUT);
pinMode(DCoil,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  



one_step(8);
delay(1000);
one_step(-8);
delay(1000);
}


void write_stepper(int a,int b,int c,int d){
digitalWrite(ACoil,a);
digitalWrite(BCoil,b);
digitalWrite(CCoil,c);
digitalWrite(DCoil,d);
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
last_position += steps;
}
