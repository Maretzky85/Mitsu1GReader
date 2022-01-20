#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

#define LCD_COLS 20
#define LCD_ROWS 4

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