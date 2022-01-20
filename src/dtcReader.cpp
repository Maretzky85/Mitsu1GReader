#include <dtcReader.h>

const int DTC_READ = 0;
const int DTC_CLEAR = 1;

int current_state = DTC_READ;

int clearErrorsList[] = {0xCA, 0x7E, 0xFA, 0xFC};

int NO_DTC = -1;
int stored_low = 0x3B;
int stored_high = 0x3C;
const char DTC_HEADER[] PROGMEM = "DTC READER";
const char NO_ERRORS[] PROGMEM = "NO ERRORS";
const char DTC_CLEARED[] PROGMEM = "CLEARED    ";
const char DTC_CLEAR_ERR[] PROGMEM = "ERROR       ";
int currentErrorsPresent = 0;

struct DTC {
    int code;
    unsigned int mask;
    bool on;
    char name[14];
};

DTC errors[] = {
        {11, 0x0001, false, "Oxygen sns"},
        {12, 0x0002, false, "Air Flow"},
        {13, 0x0004, false, "Air Temp"},
        {14, 0x0008, false, "TPS"},
        {15, 0x0010, false, "ISC mtr"},
        {21, 0x0020, false, "Coolant"},
        {22, 0x0040, false, "Speed"},
        {23, 0x0080, false, "TDC"},
        {24, 0x0100, false, "Speed"},
        {25, 0x0200, false, "Baro"},
        {31, 0x0400, false, "Knock"},
        {41, 0x0800, false, "Injector"},
        {42, 0x1000, false, "FP relay"},
        {43, 0x2000, false, "EGR"},
        {44, 0x4000, false, "Ign coil"},
        {36, 0x8000, false, "Ign circuit"},
};

int errorsCount = 16;
int currentErrorShowed = NO_DTC;

bool parseDtcWithMask(unsigned int rawValue, unsigned int mask) {
    if ((rawValue & mask) == mask) {
        return true;
    } else {
        return false;
    }
}

void updateDTCState(unsigned int dtcRawState) {
    int presentErrors = 0;
    for (int i = 0; i < errorsCount; i++) {
        bool isOn = parseDtcWithMask(dtcRawState, errors[i].mask);
        if (isOn) {
            errors[i].on = true;
            presentErrors++;
        } else {
            errors[i].on = false;
        }
    }
    currentErrorsPresent = presentErrors;
}

void printFirstError() {
    for (int i = 0; i < errorsCount; i++) {
        if (errors[i].on) {
            currentErrorShowed = i;
            printDTC(errors[i].code, errors[i].name);
            break;
        }
    }
}

void printErrorIndex(int index) {
    printDTC(errors[index].code, errors[index].name);
}

void setNextDtc() {
    int currentErrorIndex = currentErrorShowed;
    int startPosition = 0;
    if (currentErrorShowed != NO_DTC) {
        startPosition = currentErrorShowed;
    }

    for (int i = startPosition + 1; i < errorsCount; i++) {
        if (errors[i].on) {
            currentErrorShowed = i;
            break;
        }
    }
    bool nextFound = currentErrorIndex != currentErrorShowed;
    if (!nextFound) {
        for (int i = 0; i < currentErrorShowed; i++) {
            if (errors[i].on) {
                currentErrorShowed = i;
                break;
            }
        }
    }
}

void dtc_checkButtons() {
    if (buttonState == NEXT) {
        setNextDtc();
    }
    if (buttonState == PREVIOUS) {
        current_state = !current_state;
    }
}

void readDtc() {
    printHeader(DTC_HEADER);
    int lowByte = getResponseFromAddr(stored_low);
    delay(1);
    int highByte = getResponseFromAddr(stored_high);
    if (lowByte == COMMUNICATION_COMM_ERR || highByte == COMMUNICATION_COMM_ERR) {
        printError(COMM_ERR);
        return;
    }
    if (lowByte == COMMUNICATION_RESP_ERR || highByte == COMMUNICATION_RESP_ERR) {
        printError(RESP_ERR);
        return;
    }

    unsigned int word = highByte * 256 + lowByte;
    updateDTCState(word);

    printDtcCount(currentErrorsPresent);

    dtc_checkButtons();

    if (currentErrorsPresent != 0) {
        if (currentErrorShowed == NO_DTC || currentErrorsPresent == 1) {
            printFirstError();
        } else {
            printDTC(errors[currentErrorShowed].code, errors[currentErrorShowed].name);
        }
    } else {
        printResult_P(NO_ERRORS);
        currentErrorShowed = NO_DTC;
    }
}

void clear_errors() {
    for (int &i: clearErrorsList) {
        int response = getResponseFromAddr(i);
        if (response == 0) {
            printResult_P(DTC_CLEARED);
            delay(250);
            current_state = 0;
            return;
        }
        delay(1);
    }
    printResult_P(DTC_CLEAR_ERR);
    delay(250);
    current_state = 0;
}

void dtcReader() {
    switch (current_state) {
        case DTC_READ:
            readDtc();
            break;
        case DTC_CLEAR:
            clear_errors();
            break;
        default:
            break;
    }
}