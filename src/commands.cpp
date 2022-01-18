#include <commands.h>

char HEADER[] = "COMMANDS";
int currentCommand = 0;

struct command
{
    int addres;
    char name[14];
};

enum command_addr
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
    {INJ1, "Injector 1"},
    {INJ2, "Injector 2"},
    {INJ3, "Injector 3"},
    {INJ4, "Injector 4"},
    {INJ5, "Injector 5"},
    {INJ6, "Injector 6"},
    {FP, "Fuel Pump R"},
    {PURGE, "Purge"},
    {U1, "Unk 1"},
    {U2, "Unk 2"},
    {U3, "Unk 3"},
    {U4, "Unk 4"},
};

int commands_length = sizeof(commands) / sizeof(*commands);

void executeCommand() {
    int result = getResponseFromAddr(commands[currentCommand].addres);
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
    printResult(commands[currentCommand].name);
}