#include <Arduino.h>
#include <buttons.h>
#include <helpers.h>
#include <lcd.h>
#include <communication.h>
#include <dataReader.h>
#include <dtcReader.h>

#define DATA_READER 0
#define DTC_READER 1
#define MAX_STATE 2

int state[3]
{
  DATA_READER,
  DTC_READER,
  MAX_STATE
};

int currentState = DATA_READER;
void setup()
{
  lcdStart();
  pinMode(prevButtonPin, INPUT);
  pinMode(nextButtonPin, INPUT);
  Serial.begin(2000);
}

void updateState()
{
  readButtonsState();
  if (buttonState == LONG_NEXT)
  {
    currentState++;
    if (currentState == MAX_STATE)
    {
      currentState = 0;
    }
  }
  if (buttonState == LONG_PREVIOUS)
  {
    if (currentState == 0)
    {
      currentState = sizeof(state) / sizeof(*state) - 1;
    }
    else
    {
      currentState--;
    }
  }
}

void loop()
{
  updateState();
  switch (currentState)
  {
  case DATA_READER:
    readAndDisplayData();
    break;
  case DTC_READER:
    readDtc();
  default:
    break;
  }
}