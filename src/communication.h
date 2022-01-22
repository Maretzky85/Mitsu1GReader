#pragma once

#include <Arduino.h>
#include <lcd.h>

const int COMM_ERR = -1;

extern const char *communicationStatus;

void updateStatus();

int getResponseFromAddr(int &address);

int getResponseFromAddr(const uint8_t *address);