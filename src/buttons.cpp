#include <Arduino.h>
#include <buttons.h>

#define longPressTime 500

buttons buttonState = NONE;

unsigned long prevButtonPressedTime = 0;
bool prevButtonPressed = false;

unsigned long nextButtonPressedTime = 0;
bool nextButtonPressed = false;

buttons checkButtonsInput() {
    int prevButtonState = digitalRead(prevButtonPin);

    if (prevButtonState == HIGH && prevButtonPressed) {
        prevButtonPressed = false;
        if (millis() - prevButtonPressedTime > longPressTime) {
            return LONG_PREVIOUS;
        }
        return PREVIOUS;
    }

    if (prevButtonState == LOW && !prevButtonPressed) {
        prevButtonPressed = true;
        prevButtonPressedTime = millis();
    }

    int nextButtonState = digitalRead(nextButtonPin);

    if (nextButtonState == HIGH && nextButtonPressed) {
        nextButtonPressed = false;
        if (millis() - nextButtonPressedTime > longPressTime) {
            return LONG_NEXT;
        }
        return NEXT;
    }

    if (nextButtonState == LOW && !nextButtonPressed) {
        nextButtonPressed = true;
        nextButtonPressedTime = millis();
    }
    return NONE;
}

void updateButtonsState() {
    buttonState = checkButtonsInput();
}