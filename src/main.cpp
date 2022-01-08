#include <Arduino.h>
#include <helpers.h>
#include <lcd.h>
#include <communication.h>
#include <buttons.h>

//request statistics
long start = millis();
int receivedResponses = 0;
int requestPerSecond = 0;

int currentRequest = 0;

bool error = false;

void checkButtons()
{
  buttons buttonState = checkButtonsInput();
  if (buttonState == NEXT)
  {
    currentRequest++;
    if (currentRequest == REQUESTS_SIZE)
    {
      currentRequest = 0;
    }
  }
  if (buttonState == PREVIOUS)
  {
    currentRequest--;
    if (currentRequest < 0)
    {
      currentRequest = REQUESTS_SIZE - 1;
    }
  }
}

void setup()
{
  lcdStart();
  pinMode(prevButtonPin, INPUT);
  pinMode(nextButtonPin, INPUT);
  Serial.begin(2000);
}

void loop()
{
  // error = false;
  checkButtons();
  printRequestName(currentRequest);
  printRps(requestPerSecond);
  bool successSend = send(requests[currentRequest].addr);
  if (successSend == false)
  {
    printError("COMM ERROR");
    error = true;
  };
  if (waitForResponse())
  {
    int readData = Serial.read();
    receivedResponses++;
    if (error)
    {
      error = false;
      printResult(parseData(readData, requests[currentRequest].parser), requests[currentRequest].unit, true);
    }
    else
    {
      printResult(parseData(readData, requests[currentRequest].parser), requests[currentRequest].unit);
    }
  }
  else if (!error)
  {
    printError("RESPONSE ERROR");
  }
  if (millis() - start > 1000)
  {
    requestPerSecond = receivedResponses;
    receivedResponses = 0;
    start = millis();
  }
}