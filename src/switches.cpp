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
const char FUEL_PUMP_BIT_NAME[] PROGMEM = "FP relay";
const char KEY_START_NAME[] PROGMEM = "Key strt";
const char UNKNOWN_NAME[] PROGMEM = "UNK";

//STATES
const char ON_NAME[] PROGMEM = "ON";
const char OFF_NAME[] PROGMEM = "OFF";

struct sSwitch {
    const uint8_t *address;
    const uint8_t mask;
    const char *name PROGMEM;
};

// Port1 (obdCode $00, RAM $02) and Port3 (obdCode $02, RAM $06) bit meanings
// come from ROM annotations in mh6111_project/source/standard_E932_E931_source.asm.
// Rows with mask 0x00 have been removed because (raw & 0x00) is always 0, which
// made those switches always read as ON.
// Power steering ($08) and Park/N ($20) are community-attributed; not confirmed
// in the disassembly comments -- verify against a known-good vehicle before trusting.
const sSwitch switches[] = {
        {&FIRST,  0x10, FUEL_PUMP_BIT_NAME},     // port1 bit 4 (ROM L7607)
        {&FIRST,  0x20, AC_CLUTCH_NAME},         // port1 bit 5 (ROM L4621/4627)
        {&SECOND, 0x04, TOP_DEAD_CENTER_NAME},   // port3 bit 2 (ROM L948)
        {&SECOND, 0x08, POWER_STEERING_NAME},    // port3 bit 3 (community-attributed)
        {&SECOND, 0x10, AC_SWITCH_NAME},         // port3 bit 4 (ROM L2901)
        {&SECOND, 0x20, PN_SWITCH_NAME},         // port3 bit 5 (community-attributed)
        {&SECOND, 0x40, KEY_START_NAME},         // port3 bit 6 (ROM L7536)
        {&SECOND, 0x80, IDLE_SWITCH_NAME},       // port3 bit 7 (ROM L2716)
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