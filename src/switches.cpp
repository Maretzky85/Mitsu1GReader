#include "switches.h"

//ADDRESS
const uint8_t FIRST = 0x00;
const uint8_t SECOND = 0x02;

//NAMES
const char SWITCHES_HEADER[] PROGMEM = "SWITCHES READER";
const char TOP_DEAD_CENTER_NAME[] PROGMEM = "TDC";
const char POWER_STEERING_NAME[] PROGMEM = "Power St";
const char AC_SWITCH_NAME[] PROGMEM = "AC switch";
const char AC_CLUTCH_NAME[] PROGMEM = "AC clutch";
const char PN_SWITCH_NAME[] PROGMEM = "Park / N";
const char IDLE_SWITCH_NAME[] PROGMEM = "Idle sw";
const char UNKNOWN_NAME[] PROGMEM = "UNK0";
const char UNKNOWN_NAME1[] PROGMEM = "UNK1";
const char UNKNOWN_NAME2[] PROGMEM = "UNK2";
const char UNKNOWN_NAME3[] PROGMEM = "UNK3";
const char UNKNOWN_NAME4[] PROGMEM = "UNK4";
const char UNKNOWN_NAME5[] PROGMEM = "UNK5";
const char UNKNOWN_NAME6[] PROGMEM = "UNK6";
const char UNKNOWN_NAME7[] PROGMEM = "UNK7";
const char UNKNOWN_NAME8[] PROGMEM = "UNK8";
const char UNKNOWN_NAME9[] PROGMEM = "UNK9";

//STATES
const char ON_NAME[] PROGMEM = "ON";
const char OFF_NAME[] PROGMEM = "OFF";

struct sSwitch {
    const uint8_t *address;
    const uint8_t mask;
    const char *name PROGMEM;
};

const sSwitch switches[] = {
        {&SECOND, 0x00, UNKNOWN_NAME},
        {&SECOND, 0x02, UNKNOWN_NAME1},
        {&SECOND, 0x04, TOP_DEAD_CENTER_NAME},
        {&SECOND, 0x08, POWER_STEERING_NAME},
        {&SECOND, 0x10, AC_SWITCH_NAME},
        {&SECOND, 0x20, PN_SWITCH_NAME},
        {&SECOND, 0x40, UNKNOWN_NAME2},
        {&SECOND, 0x80, IDLE_SWITCH_NAME},
        {&FIRST,  0x00, UNKNOWN_NAME3},
        {&FIRST,  0x02, UNKNOWN_NAME4},
        {&FIRST,  0x04, UNKNOWN_NAME5},
        {&FIRST,  0x08, UNKNOWN_NAME6},
        {&FIRST,  0x10, UNKNOWN_NAME7},
        {&FIRST,  0x20, AC_CLUTCH_NAME},
        {&FIRST,  0x40, UNKNOWN_NAME8},
        {&FIRST,  0x80, UNKNOWN_NAME9},
};

const uint8_t switches_size = sizeof(switches) / sizeof(*switches);
uint8_t current = 0;

void checkSwButtons() {
    if (buttonState == NEXT) {
        current++;
        if (current == switches_size) {
            current = 0;
        }
    }
    if (buttonState == PREVIOUS) {
        current--;
        if (current == 0) {
            current = switches_size - 1;
        }
    }
}

int getSwitchesNumberPlus(int addition) {
    int nextR = current + addition;
    if (nextR >= switches_size) {
        nextR = nextR - switches_size;
    }
    return nextR;
}

bool parseWithMask(const uint8_t &rawValue, const int &mask) {
    return (rawValue & mask) == mask;
}

void run_switches_routine() {
    checkSwButtons();
    printHeader(SWITCHES_HEADER);

    for (int i = 0; i < LCD_ROWS - 1; ++i) {
        int switchId = getSwitchesNumberPlus(i);
        printResultName(switches[switchId].name, i);
        int result = getResponseFromAddr(switches[switchId].address);
        if (result == COMM_ERR) {
            return;
        }
        parseWithMask(result, switches[switchId].mask)
        ? printResult(ON_NAME, i)
        : printResult(OFF_NAME, i);
        delay(1);
    }
}