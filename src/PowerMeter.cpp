#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include "INA226.h"

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
INA226 INA(0x40); //TODO: what address?

void setup()
{
  Serial.begin(115200);

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Power Analyzer");
  lcd.setCursor(0, 1);
  lcd.print("By Chris Weiland");

  delay(2000);

  Wire.begin();
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

  // TODO: what should MaxCurrent be?  What does this do?
  int err = INA.setMaxCurrentShunt(5, 0.001);
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
  //INA.setAverage();
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

void loop()
{
  char out[8] = {0};
  lcd.setCursor(0, 0);
  snprintf(out, 7, "%6.3fV", (double)INA.getBusVoltage());
  lcd.print(out);
  lcd.setCursor(8, 0);
  snprintf(out, 7, "%6.3fA", (double)INA.getCurrent());
  lcd.print(out);
  lcd.setCursor(0, 1);
  snprintf(out, 7, "%5.3fmV", (double)INA.getShuntVoltage_mV());
  lcd.print(out);
  lcd.setCursor(8, 1);
  snprintf(out, 7, "%6.3fW", (double)INA.getPower());
  lcd.print(out);

  delay(500);
}
