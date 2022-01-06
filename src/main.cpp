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

long start = millis();
int receivedResponses = 0;
int requestPerSecond = 0;
int lastRps = 0;
int lastRequest = 0;
bool error = false;

void printRps() {
  if (requestPerSecond != lastRps)
  {
    lcd.setCursor(14,0);
    lcd.print(requestPerSecond);
    lcd.print(" ");
    lastRps = requestPerSecond;
  }
};

void printRequestName(int i) {
    lcd.setCursor(0,0);
    if (lastRequest != i)
    {
      lcd.print(requests[i].name + "          ");
      lastRequest = i;
    }
};

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
    i = 4;
    printRequestName(i);
    printRps();
    bool successSend = sendAndRemoveEcho(requests[i].addr);
    if (successSend == false)
    {
      lcd.setCursor(0,1);
      lcd.print("COMM ERROR        ");
      error = true;
    };
    // when characters arrive over the serial port...
    ;
  if (waitForResponse()) {
      int readData = Serial.read();
      int parser = requests[i].parser;
      String result = parseData(readData, parser) + " " +requests[i].unit + "               ";
      lcd.setCursor(0,1); 
      lcd.print(result);
      
      receivedResponses++;
      
  } else if (!error) {
      lcd.setCursor(0,1);
      lcd.print("RESPONSE ERROR                    ");
    }
  if (millis() - start > 1000)
    {
      requestPerSecond = receivedResponses;
      receivedResponses = 0;
      start = millis();
    }
}
}