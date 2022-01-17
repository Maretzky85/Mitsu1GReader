#include <lcd.h>

hd44780_I2Cexp lcd;

#define LCD_COLS 16
#define LCD_ROWS 2

const int rps_x_position = LCD_COLS - 2;
const int rps_y_position = 0;

char headerBuffer[14];
char resultBuffer[14];

//display optimalisation - dont print if same
void *lastPrintedHeader = nullptr;
int lastRpsPrinted = 0;
String lastResultPrinted = "";
int lastDTCIdPrinted = 0;

void lcdStart()
{
    lcd.init();      // initialize the lcd
    lcd.backlight(); // turn on backlight for lcd
}

void printRps(int requestPerSecond)
{
    if (requestPerSecond != lastRpsPrinted)
    {
        lcd.setCursor(rps_x_position, rps_y_position);
        lcd.print(requestPerSecond);
        lcd.print(" ");
        lastRpsPrinted = requestPerSecond;
    } 
};

void printHeader(char *header)
{
    if (lastPrintedHeader != header)
    {
        sprintf(headerBuffer, "%-13s", header);
        lcd.setCursor(0, 0);
        lcd.print(headerBuffer);
        lastPrintedHeader = header;
    }
}

void printResult(char *result, boolean force)
{
    if (lastResultPrinted != result || force)
    {
        lastResultPrinted = result;
        lcd.setCursor(0, 1);
        lcd.print(result);
    }
    else
    {
        delay(1); // without drawing next request is sent too fast. 1ms delay is enough.
    }
}


void printDTC(int dtcCode, char dtcName[])
{
    if (lastDTCIdPrinted != dtcCode)
    {
        sprintf(resultBuffer, "%2d - %-8s", dtcCode, dtcName);
        lastDTCIdPrinted = dtcCode;
        lcd.setCursor(0, 1);
    }
    printResult(resultBuffer);
}

void printError(char * errorName)
{
    sprintf(resultBuffer, "%-13s", errorName);
    printResult(resultBuffer);
}

void printDtcCount(int count)
{
    lcd.setCursor(14, 1);
    lcd.print(count);
    lcd.print(' ');
}

void clearScreen() {
    lcd.clear();
}