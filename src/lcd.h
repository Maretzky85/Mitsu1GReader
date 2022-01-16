#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

#define LCD_COLS 16
#define LCD_ROWS 2

const int rps_x_position = LCD_COLS - 2;
const int rps_y_position = LCD_ROWS - 1;

hd44780_I2Cexp lcd;

//display optimalisation - dont print if same
String lastHeaderPrinted = "";
int lastRpsPrinted = 0;
String lastResultPrinted = "";
int lastDTCIdPrinted = 0;

void lcdStart()
{
    lcd.init();      // initialize the lcd
    lcd.backlight(); // turn on backlight for lcd
}

void printRps(int requestPerSecond = 0)
{
    if (requestPerSecond != lastRpsPrinted)
    {
        lcd.setCursor(rps_x_position, rps_y_position);
        lcd.print(requestPerSecond);
        lcd.print(" ");
        lastRpsPrinted = requestPerSecond;
    }
};

void printHeader(String header) {
    if (lastHeaderPrinted != header)
    {
    int rest = LCD_COLS - header.length();
        lcd.setCursor(0, 0);
        lcd.print(header);
        lastHeaderPrinted = header;
        for (int i = 0; i < rest; i++)
        {
            lcd.print(" ");
        }
    }
}

void printRequestName(int requestNumber)
{
    printHeader(requests[requestNumber].name);
};

void printError(char errorName[])
{
    // int rest = LCD_COLS - errorName.length() -2; //TODO automatic calculation for rest of row
    int rest = LCD_COLS - 8;
    lcd.setCursor(0, 1);
    lcd.print(errorName);
    for (int i = 0; i < rest; i++)
    {
        lcd.print(" ");
    }
}

void printDTC(int dtcCode, char dtcName[]) {
    if (lastDTCIdPrinted != dtcCode)
    {
        lastDTCIdPrinted = dtcCode;
        lcd.setCursor(0, 1);
        lcd.print(dtcCode);
        lcd.print(" ");
        lcd.print(dtcName);
        lcd.print("   ");
    }
    
}

void printResult(char * result, boolean force = false)
{
    if (lastResultPrinted != result || force)
    {
        lastResultPrinted = result;
        lcd.setCursor(0, 1);
        lcd.print(result);
    } else {
        delay(1); // without drawing next request is sent too fast. 1ms delay is enough.
    }
}

void printDtcCount(int count) {
    lcd.setCursor(14, 1);
    lcd.print(count);
    lcd.print(' ');
}