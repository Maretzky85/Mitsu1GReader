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
const char UNKNOWN_ONE_NAME[] PROGMEM = "UNK 1";
const char UNKNOWN_TWO_NAME[] PROGMEM = "UNK 2";
const char UNKNOWN_THREE_NAME[] PROGMEM = "UNK 3";
const char UNKNOWN_FOUR_NAME[] PROGMEM = "UNK 4";

int currentCommand = 0;

struct command
{
    int address;
    const char * name PROGMEM;
};

enum command_address
{
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

command commands[] = {
    {INJ1, INJECTOR_ONE_NAME},
    {INJ2, INJECTOR_TWO_NAME},
    {INJ3, INJECTOR_THREE_NAME},
    {INJ4, INJECTOR_FOUR_NAME},
    {INJ5, INJECTOR_FIVE_NAME},
    {INJ6, INJECTOR_SIX_NAME},
    {FP, FUEL_PUMP_RELAY_NAME},
    {PURGE, PURGE_RELAY_NAME},
    {U1, UNKNOWN_ONE_NAME},
    {U2, UNKNOWN_TWO_NAME},
    {U3, UNKNOWN_THREE_NAME},
    {U4, UNKNOWN_FOUR_NAME},
};

int commands_length = sizeof(commands) / sizeof(*commands);

void executeCommand() {
    int result = getResponseFromAddr(commands[currentCommand].address);
    printResult(result);
    delay(1000);
}

void checkCommandsButtons()
{
    if (buttonState == NEXT)
    {
        currentCommand++;
        if (currentCommand == commands_length)
        {
            currentCommand = 0;
        }
    }
    if (buttonState == PREVIOUS)
    {
        executeCommand();
    }
    
}

void commands_proc()
{
    checkCommandsButtons();
    printHeader(HEADER);
    printResult_P(commands[currentCommand].name);
}