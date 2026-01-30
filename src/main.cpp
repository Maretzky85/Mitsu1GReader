
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

// Memory monitoring - helps detect memory leaks
extern unsigned int __heap_start;
extern void *__brkval;

int freeMemory() {
    int free_memory;
    if ((int)__brkval == 0)
        free_memory = ((int)&free_memory) - ((int)&__heap_start);
    else
        free_memory = ((int)&free_memory) - ((int)__brkval);
    return free_memory;
}

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

    // Memory monitoring (OPTIONAL - for debugging only)
    // Uncomment to track free RAM over time. NOTE: This will interfere with ECU communication!
    // To use: disconnect ECU, change Serial.begin(2000) to Serial.begin(9600), then uncomment below:
    /*
    static unsigned long lastMemCheck = 0;
    if (millis() - lastMemCheck > 5000) {  // Every 5 seconds
        Serial.print("Free RAM: ");
        Serial.print(freeMemory());
        Serial.println(" bytes");
        lastMemCheck = millis();
    }
    */

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