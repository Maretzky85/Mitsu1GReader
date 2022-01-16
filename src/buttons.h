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

long _prevButtonPressedTime = 0;
boolean _prevButtonPressed = false;

long _nextButtonPressedTime = 0;
boolean _nextButtonPressed = false;

buttons _checkButtonsInput()
{
    int prevButtonState = digitalRead(prevButtonPin);

    if (prevButtonState == LOW && _prevButtonPressed)
    {
        _prevButtonPressed = false;
        if (millis() - _prevButtonPressedTime > longPressTime)
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
        if (millis() - _nextButtonPressedTime > longPressTime)
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