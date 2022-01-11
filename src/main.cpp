#include <Arduino.h>
#include <buttons.h>
#include <helpers.h>
#include <lcd.h>
#include <communication.h>
#include <dataReader.h>
#include <dtcReader.h>

enum state
{
  DATA_READER,
  DTC_READER
};

state currentState = DATA_READER;
void setup()
{
  lcdStart();
  pinMode(prevButtonPin, INPUT);
  pinMode(nextButtonPin, INPUT);
  Serial.begin(2000);
}

void loop()
{
  updateButtonsState();
  if (buttonState == LONG_NEXT)
  {
    currentState = DTC_READER;
  }
  if (buttonState == LONG_PREVIOUS)
  {
    currentState = DATA_READER;
  }
  

  switch (currentState)
  {
  case DATA_READER:
    readAndDisplayData();
    break;
  case DTC_READER:
    readDtcBytes();
  default:
    break;
  }
}