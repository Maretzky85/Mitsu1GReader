#include <Arduino.h>
#define prevButtonPin 2
#define nextButtonPin 3
#define longPressTime 500

enum buttons
{
    NEXT,
    LONG_NEXT,
    PREVIOUS,
    LONG_PREVIOUS,
    NONE
};

buttons buttonState = NONE;

long prevButtonPressedTime = 0;
boolean prevButtonPressed = false;

long nextButtonPressedTime = 0;
boolean nextButtonPressed = false;

buttons checkButtonsInput()
{
    int prevButtonState = digitalRead(prevButtonPin);
    if (prevButtonState == LOW && prevButtonPressed)
    {
        prevButtonPressed = false;
        if (millis() - prevButtonPressedTime > longPressTime)
        {
            return LONG_PREVIOUS;
        }
        return PREVIOUS;
    }

    if (prevButtonState == HIGH && !prevButtonPressed)
    {
        prevButtonPressed = true;
        prevButtonPressedTime = millis();
    }

    int nextButtonState = digitalRead(nextButtonPin);
    if (nextButtonState == LOW && nextButtonPressed)
    {
        nextButtonPressed = false;
        if (millis() - nextButtonPressedTime > longPressTime)
        {
            return LONG_NEXT;
        }
        return NEXT;
    }

    if (nextButtonState == HIGH && !nextButtonPressed)
    {
        nextButtonPressed = true;
        nextButtonPressedTime = millis();
    }
    return NONE;
}


void readButtonsState() {
    buttonState = checkButtonsInput();
}