unsigned int Feeder_speed[10];
void setup() {
  // put your setup code here, to run once:
  for(int i = 2;i<12;i++){
    pinMode(i,OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 2;i<12;i++){
    digitalWrite(i,HIGH);
    delay(2000);
    digitalWrite(i,LOW);
  }
}
