#include <dtcReader.h>

//NAMES
const char DTC_OXYGEN_SENSOR[] PROGMEM = "Oxygen sns";
const char DTC_AIR_FLOW[] PROGMEM = "Air Flow";
const char DTC_AIR_TEMP[] PROGMEM = "Air Temp";
const char DTC_TPS[] PROGMEM = "TPS";
const char DTC_ISC[] PROGMEM = "ISC mtr";
const char DTC_COOLANT[] PROGMEM = "Coolant";
const char DTC_ENGINE_SPEED_SENSOR[] PROGMEM = "Eng Speed";
const char DTC_TDC[] PROGMEM = "TDC";
const char DTC_VEHICLE_SPEED_SENSOR[] PROGMEM = "VSS sns";
const char DTC_BAROMETER[] PROGMEM = "Barometer";
const char DTC_KNOCK_SENSOR[] PROGMEM = "Knock sns";
const char DTC_INJECTOR[] PROGMEM = "Injector";
const char DTC_FUEL_PUMP_RELAY[] PROGMEM = "FP relay";
const char DTC_EGR[] PROGMEM = "Egr";
const char DTC_IGNITION_COIL[] PROGMEM = "Ign coil";
const char DTC_IGNITION_CIRCUIT[] PROGMEM = "Ign circuit";
const char DTC_EMPTY[] PROGMEM = " ";

const int DTC_READ = 0;
const int DTC_CLEAR = 1;

int current_state = DTC_READ;

int clearErrorsList[] = {0xCA, 0x7E, 0xFA, 0xFC};

int stored_low = 0x3B;
int stored_high = 0x3C;
const char DTC_HEADER[] PROGMEM = "DTC READER";
const char NO_ERRORS[] PROGMEM = "NO ERRORS";
const char DTC_CLEARED[] PROGMEM = "CLEARED";
const char DTC_CLEAR_ERR[] PROGMEM = "ERROR";
int currentErrorsPresent = 0;

struct DTC {
    int code;
    unsigned int mask;
    const char *name PROGMEM;
};

DTC * activeErrors[16];

DTC errors[] = {
        {11, 0x0001, DTC_OXYGEN_SENSOR},
        {12, 0x0002, DTC_AIR_FLOW},
        {13, 0x0004,  DTC_AIR_TEMP},
        {14, 0x0008,  DTC_TPS},
        {15, 0x0010,  DTC_ISC},
        {21, 0x0020,  DTC_COOLANT},
        {22, 0x0040,  DTC_ENGINE_SPEED_SENSOR},
        {23, 0x0080,  DTC_TDC},
        {24, 0x0100,  DTC_VEHICLE_SPEED_SENSOR},
        {25, 0x0200,  DTC_BAROMETER},
        {31, 0x0400,  DTC_KNOCK_SENSOR},
        {41, 0x0800,  DTC_INJECTOR},
        {42, 0x1000,  DTC_FUEL_PUMP_RELAY},
        {43, 0x2000,  DTC_EGR},
        {44, 0x4000,  DTC_IGNITION_COIL},
        {36, 0x8000,  DTC_IGNITION_CIRCUIT},
};

int errorsCount = 16;
int printIndex = 0;

bool parseDtcWithMask(unsigned int rawValue, unsigned int mask) {
    return ((rawValue & mask) == mask);
}

void updateDTCState(unsigned int dtcRawState) {
    int activeIndex = 0;
    int presentErrors = 0;
    for (int i = 0; i < errorsCount; i++) {
        bool isOn = parseDtcWithMask(dtcRawState, errors[i].mask);
        if (isOn) {
            activeErrors[activeIndex] = &errors[i];
            activeIndex++;
            presentErrors++;
        } else {
        }
    }
    for (int i = activeIndex; i < errorsCount; ++i) {
        activeErrors[i] = nullptr;
    }
    if (presentErrors < LCD_ROWS - 1) {
        printIndex = 0;
    }
    currentErrorsPresent = presentErrors;
}

void printFirstError() {
    DTC firstError = *activeErrors[0];
    printDTC(firstError.code, firstError.name, 0);
    printResultName(DTC_EMPTY, 1);
    printResultName(DTC_EMPTY, 2);
    printResultName(DTC_EMPTY, 3);
}

void printErrorIndex(int index) {
    printDTC(errors[index].code, errors[index].name, 0);
}

void dtc_checkButtons() {
    if (buttonState == NEXT) {
        printIndex < currentErrorsPresent - (LCD_ROWS - 1)
        ? printIndex++
        : printIndex = 0;
        }
    if (buttonState == PREVIOUS) {
        current_state = !current_state;
    }
}

void readAndUpdateState() {
    printHeader(DTC_HEADER);
    int lowByte = getResponseFromAddr(stored_low);
    delay(1);
    int highByte = getResponseFromAddr(stored_high);
    if (lowByte == COMM_ERR || highByte == COMM_ERR) {
        return;
    }
    updateDTCState(highByte * 256 + lowByte);
}

void displayDtcResults() {
    if (currentErrorsPresent == 0) {
        printResult_P(NO_ERRORS);
        printResultName(DTC_EMPTY, 1);
        printResultName(DTC_EMPTY, 2);
        printResultName(DTC_EMPTY, 3);
        return;
    }

    if (currentErrorsPresent == 1) {
        printFirstError();
    } else {
        for (int i = 0; i < LCD_ROWS - 1; ++i) {
            DTC currentErrorToPrint = *activeErrors[i + printIndex];
            activeErrors[i] != nullptr
            ? printDTC(currentErrorToPrint.code, currentErrorToPrint.name, i)
            : printResultName(DTC_EMPTY, i);
        }
    }
}

void readDtc() {
    readAndUpdateState();
    printDtcCount(currentErrorsPresent);
    dtc_checkButtons();
    displayDtcResults();
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