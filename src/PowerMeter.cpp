#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include "INA226.h"

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
INA226 INA(0x40);

void setup()
{
  Serial.begin(115200);

  lcd.begin(16, 2);
  lcd.noCursor();
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Power Analyzer");
  lcd.setCursor(0, 1);
  lcd.print("By Chris Weiland");

  Wire.begin();
  delay(2000);

  Serial.println("Starting INA226...");
  if (!INA.begin())
  {
    Serial.println("could not connect. Fix and Reboot");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("INA226 not found");
    lcd.setCursor(0, 1);
    lcd.print("Fix the address?");
    while (true)
      delay(1000);
  }
  Serial.println("INA Started!");

  // TODO: what should MaxCurrent be?  What does this do?
  int err = INA.setMaxCurrentShunt(2.5, 0.03);
  if (err != INA226_ERR_NONE)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MaxCurrentShunt");
    lcd.setCursor(0, 1);
    lcd.print("Failed: ");
    char out[8] = {0};
    snprintf(out, 7, "%x", err);
    lcd.print(out);
    while (true)
      delay(1000);
  }
  //average:
    // INA226_1_SAMPLE     = 0, //default?
    // INA226_4_SAMPLES    = 1,
    // INA226_16_SAMPLES   = 2,
    // INA226_64_SAMPLES   = 3,
    // INA226_128_SAMPLES  = 4,
    // INA226_256_SAMPLES  = 5,
    // INA226_512_SAMPLES  = 6,
    // INA226_1024_SAMPLES = 7
  INA.setAverage(INA226_256_SAMPLES);
  //conversion times:
    // INA226_140_us  = 0,
    // INA226_204_us  = 1,
    // INA226_332_us  = 2,
    // INA226_588_us  = 3,
    // INA226_1100_us = 4, //default?
    // INA226_2100_us = 5,
    // INA226_4200_us = 6,
    // INA226_8300_us = 7
  //INA.setBusVoltageConversionTime();
  //INA.setShuntVoltageConversionTime();


  lcd.clear();
}

void PrintFloat(float val, int* tens, int* millis)
{
  *tens = (int)val;
  *millis = abs((int)((val - *tens) * 1000));
}

void loop()
{
  if (!INA.waitConversionReady(1000))
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Conversion");
    lcd.setCursor(0, 1);
    lcd.print("Not ready");
    Serial.println("Conversion not ready");
    return;
  }
  char out[9] = {0};
  int tens, millis;

  lcd.setCursor(0, 0);
  PrintFloat(INA.getBusVoltage(), &tens, &millis);
  snprintf(out, 8, "%2d.%03dV", tens, millis);
  lcd.print(out);
  Serial.print(out);

  lcd.setCursor(8, 0);
  PrintFloat(INA.getCurrent(), &tens, &millis);
  snprintf(out, 8, "%2d.%03dA", tens, millis);
  lcd.print(out);
  Serial.print(out);

  lcd.setCursor(0, 1);
  PrintFloat(INA.getShuntVoltage_mV(), &tens, &millis);
  snprintf(out, 8, "%2d.%03dm", tens, millis);
  lcd.print(out);
  Serial.print(out);

  lcd.setCursor(8, 1);
  PrintFloat(INA.getPower(), &tens, &millis);
  snprintf(out, 8, "%2d.%03dW", tens, millis);
  lcd.print(out);
  Serial.println(out);

  char sout[256] = {0};
  snprintf(sout, 255, "%6.3f V, %6.3fA, %5.3f mV, %6.3f W\n", (double)INA.getBusVoltage(), (double)INA.getCurrent(), (double)INA.getShuntVoltage_mV(), (double)INA.getPower());
  Serial.print(sout);

  delay(500);
}
