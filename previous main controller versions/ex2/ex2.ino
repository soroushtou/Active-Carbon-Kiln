#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {
 lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
}

void loop() {
    float sensor_volt;
    float sensorValue;

    sensorValue = analogRead(A1);
    sensor_volt = sensorValue/1024*5.0;
    lcd.clear();
    lcd.print("Sensor volt :");
    lcd.setCursor(0,1);
    lcd.print(sensor_volt);
    delay(1000);
}
