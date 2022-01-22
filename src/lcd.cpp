#include <lcd.h>

hd44780_I2Cexp lcd;

const int rps_x_position = LCD_COLS - 2;
const int rps_y_position = 0;
const int printSize = LCD_COLS - 2;

char headerBuffer[printSize + 1];
char resultBuffer0[printSize - 2];
char resultBuffer1[printSize - 2];
char resultBuffer2[printSize - 2];
char * resultBuffers[] = {
        resultBuffer0,
        resultBuffer1,
        resultBuffer2
};

char rpsBuffer[3];

char temp[LCD_COLS - 1];
char headerFormat[6];

//display optimisation - dont print if same
const char *lastPrintedStatus = nullptr;
const char *lastPrintedHeader = nullptr;
const char *lastPrintedResultsName[] = {
        nullptr,
        nullptr,
        nullptr
};
String lastResults[] = {
        "",
        "",
        ""
};
int lastRpsPrinted = 0;
int lastDTCIdPrinted = 0;
String lastResultPrinted = "";

//const byte star[] PROGMEM = {
//        B00100,
//        B00100,
//        B10001,
//        B01010,
//        B00100,
//        B01010,
//        B10101,
//        B00100
//};

void lcdStart() {
    sprintf(headerFormat, "%s%d%s", "%-", LCD_COLS - 2, "s");
    lcd.begin(LCD_COLS, LCD_ROWS);
    lcd.backlight();
//    lcd.createChar(0, star);
//    lcd.write(0);
}

void printRps(int requestPerSecond) {
    if (requestPerSecond != lastRpsPrinted) {
        sprintf(rpsBuffer, "%-2d", requestPerSecond);
        lcd.setCursor(rps_x_position, rps_y_position);
        lcd.print(rpsBuffer);
        lastRpsPrinted = requestPerSecond;
    }
}

void printHeader(char *header) {
    if (lastPrintedHeader != header) {
        snprintf(headerBuffer, printSize, headerFormat, header);
        lcd.setCursor(0, 0);
        lcd.print(headerBuffer);
        lastPrintedHeader = header;
    }
}

void printHeader(const char *header) {
    if (lastPrintedHeader != header) {
        strcpy_P(temp, header);
        snprintf(headerBuffer, printSize, headerFormat, temp);
        lcd.setCursor(0, 0);
        lcd.print(headerBuffer);
        lastPrintedHeader = header;
    }
}

void printResult(char *result, boolean force) {
    if (lastResultPrinted != result || force) {
        lastResultPrinted = result;
        sprintf(resultBuffer0, "%-14s", result);
        lcd.setCursor(0, 1);
        lcd.print(resultBuffer0);
    } else {
        delay(1); // without drawing next request is sent too fast. 1ms delay is enough.
    }
}

void printResultName(const char *name, int row) {
    if (lastPrintedResultsName[row] != name) {
        strcpy_P(temp, name);
        sprintf(resultBuffers[row], "%-12s" , temp);
        lcd.setCursor(0, row + 1);
        lcd.print(resultBuffers[row]);
        lastPrintedResultsName[row] = name;
    }
}

void printResultNames(const char *names[]) {
    for (int i = 0; i < 3; ++i) {
        printResultName(names[i], i);
    }
}

void printResult(char *result, int row) {
    if (lastResults[row] != result) {
        lastResults[row] = result;
        lcd.setCursor(LCD_COLS - 8, row + 1);
        lcd.print(result);
    }
}

void printResults(char results[3][10]) {
    for (int i = 0; i < 3; ++i) {
        if (results[i] != nullptr) {
            printResult(results[i], i);
        }
    }
}

void printResult(int result) {
    sprintf(temp, "%d", result);
    printResult(temp);
}

void printResult_P(const char *result) {
    strcpy_P(temp, result);
    printResult(temp);
}

void printDTC(int dtcCode, char dtcName[]) {
    if (lastDTCIdPrinted != dtcCode) {
        sprintf(resultBuffer0, "%2d - %-8s", dtcCode, dtcName);
        lastDTCIdPrinted = dtcCode;
        lcd.setCursor(0, 1);
    }
    printResult(resultBuffer0);
}

void printError(const char *errorName) {
    strcpy_P(temp, errorName);
    sprintf(resultBuffer0, "%-13s", temp);
    printResult(resultBuffer0);
}

void printDtcCount(int count) {
    lcd.setCursor(14, 1);
    lcd.print(count);
    lcd.print(' ');
}

void printStatus(const char *status){
    if (lastPrintedStatus != status) {
        char buffer[3];
        lcd.setCursor(LCD_COLS - 5, 0);
        strcpy_P(buffer, status);
        lcd.print(buffer);
        lastPrintedStatus = status;
    }
}

void clearScreen() {
    lcd.clear();
}