int value;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  Serial.println("CLEARDATA"); //clears up any data left from previous projects
//Serial.println("DATA,Temperature_set,Feeder_RPM_set,Body_RPM_set,Angle_set,Steam_mode_set"); 
Serial.println("LABEL,Zaman,MQ2_volt,MQ3_volt,MQ4_volt,MQ7_volt,MQ9_volt,MQ135_volt,MQ214_volt,MQ2_ratio,MQ3_ratio,MQ4_ratio,MQ7_ratio,MQ9_ratio,MQ135_ratio,MQ214_ratio,Temperature,Flame_mode,Feeder_RPM,Output_weight,Scale_Speed,Angle,"); //LABEL, so excel knows the next things will be the names of the columns


Serial.println("RESETTIMER"); //resets timer to 0
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("CELL,GET,U2"); 
  
while(Serial.available()==0);
  value = Serial.parseFloat();
  Serial.print("DATA,TIME,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,");


  Serial.println(value);

  delay(100);
}
