#include <Arduino.h>
#include <buttons.h>
#include <commands.h>
#include <lcd.h>
#include <dataReader.h>
#include <dtcReader.h>

#define DATA_READER 0
#define DTC_READER 1
#define COMMANDS 2
#define MAX_STATE 3

int state[3]
{
  DATA_READER,
  DTC_READER,
  COMMANDS
};

int currentState = DTC_READER;
void setup()
{
  lcdStart();
  pinMode(prevButtonPin, INPUT);
  pinMode(nextButtonPin, INPUT);
  Serial.begin(2000);
}

void updateState()
{
  updateButtonsState();
  if (buttonState == LONG_NEXT)
  {
    currentState++;
    clearScreen();
    if (currentState == MAX_STATE)
    {
      currentState = 0;
    }
  }
  if (buttonState == LONG_PREVIOUS)
  {
    clearScreen();
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
    dtcReader();
    break;
  case COMMANDS:
    commands_proc();
  default:
    break;
  }
}