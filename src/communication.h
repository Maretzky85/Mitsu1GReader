#pragma once

#include <Arduino.h>

#define COMMUNICATION_COMM_ERR -1
#define COMMUNICATION_RESP_ERR -2
const char COMM_ERR[] PROGMEM = "Comm Err";
const char RESP_ERR[] PROGMEM = "Resp Err";

bool waitForResponse();

bool send(int &command);

int getResponseFromAddr(int &address);