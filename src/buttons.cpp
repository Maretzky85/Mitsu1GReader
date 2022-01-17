#include <Arduino.h>
#include <buttons.h>

#define _longPressTime 500

buttons buttonState = NONE;

long _prevButtonPressedTime = 0;
bool _prevButtonPressed = false;

long _nextButtonPressedTime = 0;
bool _nextButtonPressed = false;

buttons _checkButtonsInput()
{
    int prevButtonState = digitalRead(prevButtonPin);

    if (prevButtonState == LOW && _prevButtonPressed)
    {
        _prevButtonPressed = false;
        if (millis() - _prevButtonPressedTime > _longPressTime)
        {
            return LONG_PREVIOUS;
        }
        return PREVIOUS;
    }

    if (prevButtonState == HIGH && !_prevButtonPressed)
    {
        _prevButtonPressed = true;
        _prevButtonPressedTime = millis();
    }

    int nextButtonState = digitalRead(nextButtonPin);

    if (nextButtonState == LOW && _nextButtonPressed)
    {
        _nextButtonPressed = false;
        if (millis() - _nextButtonPressedTime > _longPressTime)
        {
            return LONG_NEXT;
        }
        return NEXT;
    }

    if (nextButtonState == HIGH && !_nextButtonPressed)
    {
        _nextButtonPressed = true;
        _nextButtonPressedTime = millis();
    }
    return NONE;
}

void updateButtonsState() {
    buttonState = _checkButtonsInput();
}