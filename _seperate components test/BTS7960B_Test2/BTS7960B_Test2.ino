  #define RPWM 3
  #define LPWM 11
  #define REN 8
  #define LEN 8
void setup() {
  pinMode(RPWM,OUTPUT);
  pinMode(LPWM,OUTPUT);
  pinMode(REN,OUTPUT);

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: 2000.000 kHz
// Mode: Fast PWM top=0xFF
// OC2A output: Non-Inverted PWM
// OC2B output: Disconnected
// Timer Period: 0.128 ms
// Output Pulse(s):
// OC2A Period: 0.128 ms Width: 0.038651 ms
ASSR=(0<<EXCLK) | (0<<AS2);
TCCR2A=(1<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (1<<WGM21) | (1<<WGM20);
TCCR2B=(0<<WGM22) | (0<<CS22) | (1<<CS21) | (0<<CS20);
TCNT2=0x00;
//OCR2A=0x4D;
OCR2A=0x00;
OCR2B=0x00;

/*
// Timer/Counter 4 initialization
// Clock source: System Clock
// Clock value: 2000.000 kHz
// Mode: Fast PWM top=0x00FF
// OC4A output: Non-Inverted PWM
// OC4B output: Non-Inverted PWM
// OC4C output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer Period: 0.128 ms
// Output Pulse(s):
// OC4A Period: 0.128 ms Width: 0.032125 ms
// OC4B Period: 0.128 ms Width: 0.064251 ms
// Timer4 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
// Compare C Match Interrupt: Off
TCCR4A=(1<<COM4A1) | (0<<COM4A0) | (1<<COM4B1) | (0<<COM4B0) | (0<<COM4C1) | (0<<COM4C0) | (0<<WGM41) | (1<<WGM40);
TCCR4B=(0<<ICNC4) | (0<<ICES4) | (0<<WGM43) | (1<<WGM42) | (0<<CS42) | (1<<CS41) | (0<<CS40);
TCNT4H=0x00;
TCNT4L=0x00;
ICR4H=0x00;
ICR4L=0x00;
OCR4AH=0x00;
OCR4AL=0x40;
OCR4BH=0x00;
OCR4BL=0x80;
OCR4CH=0x00;
OCR4CL=0x00;
 */
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(REN,HIGH);
  for(int i=0 ; i < 256 ; i++){
    OCR2A = i;
    delay(20);
  }
  delay(1000);
}
