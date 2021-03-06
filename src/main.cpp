
#include <Arduino.h>
#include <buttons.h>
#include <commands.h>
#include <lcd.h>
#include <dataReader.h>
#include <dtcReader.h>
#include <switches.h>

#define DATA_READER 0
#define SW_READER 1
#define DTC_READER 2
#define COMMANDS 3
#define MAX_STATE 4

const char WELCOME[] PROGMEM = "MitsuTester";

int state[4]
        {
                DATA_READER,
                SW_READER,
                DTC_READER,
                COMMANDS
        };

int currentState = DATA_READER;

void setup() {
    lcdStart();
    printHeader(WELCOME);
    delay(500);
    pinMode(prevButtonPin, INPUT_PULLUP);
    pinMode(nextButtonPin, INPUT_PULLUP);
    Serial.begin(2000);
}

void updateState() {
    updateButtonsState();
    if (buttonState == LONG_NEXT) {
        redraw = true;
        currentState++;
        clearScreen();
        if (currentState == MAX_STATE) {
            currentState = 0;
        }
    }
    if (buttonState == LONG_PREVIOUS) {
        redraw = true;
        clearScreen();
        if (currentState == 0) {
            currentState = sizeof(state) / sizeof(*state) - 1;
        } else {
            currentState--;
        }
    }

}

void loop() {
    updateState();
    updateStatus();
    switch (currentState) {
        case DATA_READER:
            dataReader();
            break;
        case DTC_READER:
            dtcReader();
            break;
        case COMMANDS:
            commands_proc();
            break;
        case SW_READER:
            run_switches_routine();
            break;
        default:
            break;
    }
    redraw = false;
}