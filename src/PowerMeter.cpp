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

  delay(1000);

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
  INA.setMaxCurrentShunt(5, 0.001);

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
  snprintf(out, 7, "%6.3fmV", (double)INA.getShuntVoltage_mV());
  lcd.print(out);
  lcd.setCursor(8, 1);
  snprintf(out, 7, "%6.3fW", (double)INA.getPower());
  lcd.print(out);

  delay(500);
}
