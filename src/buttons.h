#pragma once

#define prevButtonPin 2
#define nextButtonPin 3

enum buttons {
    NEXT,
    LONG_NEXT,
    PREVIOUS,
    LONG_PREVIOUS,
    NONE
};

extern buttons buttonState;

void updateButtonsState();