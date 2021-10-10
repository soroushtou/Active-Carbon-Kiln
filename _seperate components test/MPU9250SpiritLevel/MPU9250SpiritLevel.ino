//#include <Arduino.h>
#include "Wire.h"
#include <LiquidCrystal.h>
#define MPU9250_ADDRESS 0x68
#define ACC_FULL_SCALE_2_G 0x00
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
uint8_t Buf[14];
float yangle;
int yangle2;
void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();

  // Read Nbytes
  Wire.requestFrom(Address, Nbytes);
  uint8_t index = 0;
  while (Wire.available())
    Data[index++] = Wire.read();
}

void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lcd.begin(16, 2);
  I2CwriteByte(MPU9250_ADDRESS, 29, 0x06);
  I2CwriteByte(MPU9250_ADDRESS, 28, ACC_FULL_SCALE_2_G);
  I2CwriteByte(MPU9250_ADDRESS, 108, 0x2F);
}

void loop() {
  lcd.clear();
  I2Cread(MPU9250_ADDRESS, 0x3B, 14, Buf);
  int16_t ay = -(Buf[2] << 8 | Buf[3]);
  yangle = ay / 182.04;  
  //yangle2 = int(yangle * 10);
  lcd.print("Angle = ");
  Serial.print("Angle = ");
  Serial.print(yangle);
  lcd.print(yangle);
  lcd.setCursor(0,1);
  yangle = float(int(yangle*2)) / 2;
  lcd.print("Angle = ");
  lcd.print(yangle);
  Serial.print(" Angle = ");
  Serial.println(yangle);
  delay(100);

}
