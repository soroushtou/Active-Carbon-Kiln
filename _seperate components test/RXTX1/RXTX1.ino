int a;
void setup() {
  // put your setup code here, to run once:
Serial1.begin(9600);
Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    a = Serial1.parseInt();
    Serial.println(a);   // read it and send it out Serial (USB)
  delay(100);
  }
}
