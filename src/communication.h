#pragma once

#include <Arduino.h>

#define COMMUNICATION_COMM_ERR -1
#define COMMUNICATION_RESP_ERR -2
extern char COMM_ERR[9];
extern char RESP_ERR[9];

bool _waitForResponse();

bool _send(int &command);

int getResponseFromAddr(int &address);