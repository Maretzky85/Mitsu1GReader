#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

#define LCD_COLS 20
#define LCD_ROWS 4

extern bool redraw;

void lcdStart();

void printRps(int requestPerSecond);

void printHeader(char *header);

void printHeader(const char *header);

void printError(const char *errorName);

void printDTC(int dtcCode, char *dtcName);

void printResult(char *result, boolean force = false);

void printResult_P(const char *result);

void printResult(int result);

void printDtcCount(int count);

void clearScreen();

void printStatus(const char *status);
//LCD 4 rows

void printResult(char *result, int row, int rOffset = 0);

void printResult(const char *result, int row);

void printResultName(const char *name, int row);

void printDTC(int dtcCode, const char *dtcName, int row);
