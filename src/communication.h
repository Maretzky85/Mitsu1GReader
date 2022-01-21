#pragma once

#include <Arduino.h>
#include <lcd.h>

#define COMMUNICATION_COMM_ERR -1
#define COMMUNICATION_RESP_ERR -2
const char COMM_ERR[] PROGMEM = "Comm Err";
const char RESP_ERR[] PROGMEM = "Resp Err";

void updateResponses();

int getResponseFromAddr(int &address);

int getResponseFromAddr(const uint8_t *address);