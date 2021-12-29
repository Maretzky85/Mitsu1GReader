/*
 * Displays text sent over the serial port (e.g. from the Serial Monitor) on
 * an attached LCD.
 * YWROBOT
 *Compatible with the Arduino IDE 1.0
 *Library version:1.1
 */
#include <Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <helpers.h>

LiquidCrystal_I2C lcd(0x3F,16,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  Serial.begin(2000);
}

void loop()
{
  for (int i = 0; i < REQUESTS_SIZE; i++)
  {
    lcd.setCursor(0,0);
    lcd.print(requests[i].name);
    lcd.print("           ");
    lcd.setCursor(0,1);
    // Serial.write(allSensors[i]);
    bool successSend = sendAndRemoveEcho(requests[i].addr);
    if (successSend == false)
    {
      lcd.print("COMM ERROR");
      delay(2000);
    };
    delay(10);
    // when characters arrive over the serial port...
  if (Serial.available()) {
    // lcd.print("res: ");
    while (Serial.available() > 0) {
      // Serial.read();
      
      int readData = Serial.read();
      int parser = requests[i].parser;
      lcd.print(parseData(readData, parser));
      lcd.print(" ");
      lcd.print(requests[i].unit);
      lcd.print("               ");
      delay(500);
    }
  } else {
      lcd.print("RESPONSE ERROR");
      lcd.print("               ");
      delay(500);
    }
}
}