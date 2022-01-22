#include <communication.h>
//STATUS
const char STATUS_OK[] PROGMEM = "OK";
const char STATUS_ERR_COMM[] PROGMEM = "!C";
const char STATUS_ERR_RESP[] PROGMEM = "!R";

const char *communicationStatus = nullptr;

const uint8_t MAX_WAIT_TIME = 50;
uint8_t rps = 0;
uint8_t receivedResponses = 0;
unsigned long start = millis();

void updateStatus() {
    if (millis() - start > 1000) {
        rps = receivedResponses;
        receivedResponses = 0;
        start = millis();
    }
    printStatus(communicationStatus);
    printRps(rps);
}

void clearBuffer() {
    while (Serial.available()) {
        Serial.read();
    }
}

bool waitForResponse() {
    unsigned long startWaitTime = millis();
    while (Serial.available() == 0) {
        if (millis() - startWaitTime > MAX_WAIT_TIME) {
            return false;
        }
    }
    return true;
}

bool send(int &command) {
    Serial.write(command);
    if (!waitForResponse()) {
        communicationStatus = STATUS_ERR_COMM;
        return false;
    }
    int echo = Serial.read();
    if (echo != command) {
        communicationStatus = STATUS_ERR_COMM;
        clearBuffer();
    }
    communicationStatus = STATUS_OK;
    //echo should be same as sent command, if not, there communication problem.
    return echo == command;
}

int getResponseFromAddr(int &address) {
    if (!send(address)) {
        clearBuffer();
        return COMM_ERR;
    }
    if (waitForResponse()) {
        int readData = Serial.read();
        receivedResponses++;
        return readData;
    } else {
        communicationStatus = STATUS_ERR_RESP;
        clearBuffer();
        return COMM_ERR;
    }
}

int getResponseFromAddr(const uint8_t *address) {
    int parsed = *address;
    return getResponseFromAddr(parsed);
}