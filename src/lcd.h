#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

void lcdStart();

void printRps(int requestPerSecond);

void printHeader(char * header);

void printError(char * errorName);

void printDTC(int dtcCode, char *dtcName);

void printResult(char * result, boolean force = false);

void printResult(int result);

void printDtcCount(int count);

void clearScreen();