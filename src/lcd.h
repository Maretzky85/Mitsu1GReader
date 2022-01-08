#include <LiquidCrystal_I2C.h>

#define LCD_COLS 16
#define LCD_ROWS 2

const int rps_x_position = LCD_COLS - 2;
const int rps_y_position = LCD_ROWS - 1;

LiquidCrystal_I2C lcd(0x3F, LCD_COLS, LCD_ROWS); // set the LCD address to 0x27 for a 16 chars and 2 line display

//display optimalisation - dont print if same
int lastRequestPrinted = 0;
int lastRpsPrinted = 0;
String lastResultPrinted = "";

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

void printRequestName(int requestNumber)
{
    if (lastRequestPrinted != requestNumber)
    {
        String requestName = requests[requestNumber].name;
        int rest = LCD_COLS - requestName.length();
        lcd.setCursor(0, 0);
        lcd.print(requestName);
        lastRequestPrinted = requestNumber;
        for (int i = 0; i < rest; i++)
        {
            lcd.print(" ");
        }
    }
};

void printError(String errorName)
{
    int rest = LCD_COLS - errorName.length() -2;
    lcd.setCursor(0, 1);
    lcd.print(errorName);
    for (int i = 0; i < rest; i++)
    {
        lcd.print(" ");
    }
}

void printResult(String result, String units, boolean force = false)
{
    if (lastResultPrinted != result || force)
    {
        lastResultPrinted = result;
        lcd.setCursor(0, 1);
        int rest = LCD_COLS - result.length() - units.length() - 3;
        lcd.print(result);
        lcd.print(" ");
        lcd.print(units);
        for (int i = 0; i < rest; i++)
        {
            lcd.print(" ");
        }
    } else {
        delay(1); // without drawing request is sent too fast
    }
}