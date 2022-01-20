#include <lcd.h>

hd44780_I2Cexp lcd;

#define LCD_COLS 20
#define LCD_ROWS 4

const int rps_x_position = LCD_COLS - 2;
const int rps_y_position = 0;
const int printSize = LCD_COLS - 2;

char headerBuffer[printSize + 1];
char resultBuffer[printSize + 1];
char rpsBuffer[3];

char temp[LCD_COLS - 1];
char headerTemplate[6];

//display optimisation - dont print if same
const char *lastPrintedHeader = nullptr;
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
    sprintf(headerTemplate, "%s%d%s", "%-", LCD_COLS - 2, "s");
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
        snprintf(headerBuffer, printSize, headerTemplate, header);
        lcd.setCursor(0, 0);
        lcd.print(headerBuffer);
        lastPrintedHeader = header;
    }
}

void printHeader(const char *header) {
    if (lastPrintedHeader != header) {
        strcpy_P(temp, header);
        snprintf(headerBuffer, printSize,headerTemplate, temp);
        lcd.setCursor(0, 0);
        lcd.print(headerBuffer);
        lastPrintedHeader = header;
    }
}

void printResult(char *result, boolean force) {
    if (lastResultPrinted != result || force) {
        lastResultPrinted = result;
        sprintf(temp, "%-14s", result);
        lcd.setCursor(0, 1);
        lcd.print(temp);
    } else {
        delay(1); // without drawing next request is sent too fast. 1ms delay is enough.
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
        sprintf(resultBuffer, "%2d - %-8s", dtcCode, dtcName);
        lastDTCIdPrinted = dtcCode;
        lcd.setCursor(0, 1);
    }
    printResult(resultBuffer);
}

void printError(const char *errorName) {
    strcpy_P(temp, errorName);
    sprintf(resultBuffer, "%-13s", temp);
    printResult(resultBuffer);
}

void printDtcCount(int count) {
    lcd.setCursor(14, 1);
    lcd.print(count);
    lcd.print(' ');
}

void clearScreen() {
    lcd.clear();
}