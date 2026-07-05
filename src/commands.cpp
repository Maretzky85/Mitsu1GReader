#include <commands.h>

//NAMES
const char HEADER[] PROGMEM = "COMMANDS";
const char INJECTOR_ONE_NAME[] PROGMEM = "Injector 1";
const char INJECTOR_TWO_NAME[] PROGMEM = "Injector 2";
const char INJECTOR_THREE_NAME[] PROGMEM = "Injector 3";
const char INJECTOR_FOUR_NAME[] PROGMEM = "Injector 4";
const char INJECTOR_FIVE_NAME[] PROGMEM = "Injector 5";
const char INJECTOR_SIX_NAME[] PROGMEM = "Injector 6";
const char FUEL_PUMP_RELAY_NAME[] PROGMEM = "Fuel Pump R";
const char PURGE_RELAY_NAME[] PROGMEM = "Purge";
const char FPS_SOLENOID_NAME[] PROGMEM = "Fuel Pres";
const char EGR_SOLENOID_NAME[] PROGMEM = "EGR Sol";
const char UNUSED_BIT_NAME[] PROGMEM = "Unused$10";
const char BOOST_SOLENOID_NAME[] PROGMEM = "Boost Sol";

int currentCommand = 0;

struct command {
    int address;
    const char *name PROGMEM;
};

enum command_address {
    U4 = 0xF1,
    U3 = 0xF2,
    U2 = 0xF3,
    U1 = 0xF4,
    PURGE = 0xF5,
    FP = 0xF6,
    INJ6 = 0xF7,
    INJ5 = 0xF8,
    INJ4 = 0xF9,
    INJ3 = 0xFA,
    INJ2 = 0xFB,
    INJ1 = 0xFC
};

// MH6111 actuator masks (from t_obdActMask in ROM: 20 10 08 04 01 02):
//   $F1 mask $20 -> boost/wastegate solenoid      (ROM L4860)
//   $F2 mask $10 -> unused in E931 (no brset ref) -- may be live in other family ROMs
//   $F3 mask $08 -> EGR solenoid                  (ROM L4782)
//   $F4 mask $04 -> fuel pressure solenoid (FPS)  (ROM L4943 header, L4954)
//   $F5 mask $01 -> purge solenoid                (ROM L4647)
//   $F6 mask $02 -> fuel pump relay               (ROM L4542)
// Injector kill ($F7-$FC): $F7/$F8 are silently ignored by the ECU (4-cyl -> no inj 5/6);
// $F9-$FC kill injectors 4/3/2/1 in that order (ROM L7199-7207).
command commands[] = {
        {INJ1,  INJECTOR_ONE_NAME},
        {INJ2,  INJECTOR_TWO_NAME},
        {INJ3,  INJECTOR_THREE_NAME},
        {INJ4,  INJECTOR_FOUR_NAME},
        {INJ5,  INJECTOR_FIVE_NAME},   // no-op on 4-cyl (ECU silently discards)
        {INJ6,  INJECTOR_SIX_NAME},    // no-op on 4-cyl
        {FP,    FUEL_PUMP_RELAY_NAME},
        {PURGE, PURGE_RELAY_NAME},
        {U1,    FPS_SOLENOID_NAME},    // U1 = $F4 -> mask $04 -> FPS
        {U2,    EGR_SOLENOID_NAME},    // U2 = $F3 -> mask $08 -> EGR
        {U3,    UNUSED_BIT_NAME},      // U3 = $F2 -> mask $10 -> dead in E931
        {U4,    BOOST_SOLENOID_NAME},  // U4 = $F1 -> mask $20 -> boost/wastegate
};

int commands_length = sizeof(commands) / sizeof(*commands);

void executeCommand() {
    int result = getResponseFromAddr(commands[currentCommand].address);
    printResult(result);
    delay(1000);
}

void checkCommandsButtons() {
    if (buttonState == NEXT) {
        currentCommand++;
        if (currentCommand == commands_length) {
            currentCommand = 0;
        }
    }
    if (buttonState == PREVIOUS) {
        executeCommand();
    }

}

void commands_proc() {
    checkCommandsButtons();
    printHeader(HEADER);
    printResult_P(commands[currentCommand].name);
}