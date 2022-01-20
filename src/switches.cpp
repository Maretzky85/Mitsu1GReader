#include "switches.h"

//ADDRESS
const uint8_t FIRST = 0x00;
const uint8_t SECOND = 0x02;

//NAMES
const char TOP_DEAD_CENTER_NAME[] PROGMEM = "TDC";
const char POWER_STEERING_NAME[] PROGMEM = "Power St";
const char AC_SWITCH_NAME[] PROGMEM = "AC switch";
const char AC_CLUTCH_NAME[] PROGMEM = "AC clutch";
const char PN_SWITCH_NAME[] PROGMEM = "Park / N";
const char IDLE_SWITCH_NAME[] PROGMEM = "Idle sw";

//STATES
const char ON_NAME[] PROGMEM = "ON";
const char OFF_NAME[] PROGMEM = "OFF";

enum sw_id {
    TDC,
    PS,
    AC_SW,
    AC_CL,
    PARK_NEUTRAL,
    IDLE_SW
};

struct sSwitch {
    const sw_id id;
    const uint8_t *address;
    const uint8_t mask;
    const char *name PROGMEM;
};

const sSwitch switches[] = {
        {TDC,          &SECOND, 0x04, TOP_DEAD_CENTER_NAME},
        {PS,           &SECOND, 0x08, POWER_STEERING_NAME},
        {AC_SW,        &SECOND, 0x10, AC_SWITCH_NAME},
        {PARK_NEUTRAL, &SECOND, 0x20, PN_SWITCH_NAME},
        {IDLE_SW,      &SECOND, 0x80, IDLE_SWITCH_NAME},
        {AC_CL,        &FIRST,  0x20, AC_CLUTCH_NAME},
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
}

bool parseWithMask(const uint8_t &rawValue, const int &mask) {
    return (rawValue & mask) == mask;
}

void run_switches_routine() {
    checkSwButtons();
    printHeader(switches[current].name);
    //TODO move checks to communication as function returning bool
    int result = getResponseFromAddr(switches[current].address);
    if (result == COMMUNICATION_COMM_ERR) {
        printResult_P(COMM_ERR);
        return;
    }
    //TODO move checks to communication as function returning bool
    if (result == COMMUNICATION_RESP_ERR) {
        printResult_P(RESP_ERR);
        return;
    }
    if (parseWithMask(result, switches[current].mask)) {
        printResult_P(ON_NAME);
    } else {
        printResult_P(OFF_NAME);
    }
}