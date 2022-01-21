#include <communication.h>

const uint8_t MAX_WAIT_TIME = 50;
uint8_t rps = 0;
uint8_t receivedResponses = 0;
unsigned long start = millis();

void updateResponses() {
    if (millis() - start > 1000) {
        rps = receivedResponses;
        receivedResponses = 0;
        start = millis();
    }
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
        return false;
    }
    int echo = Serial.read();
    if (echo != command) {
        clearBuffer();
    }
    receivedResponses++;
    //echo should be same as sent command, if not, there communication problem.
    return echo == command;
}

//TODO return int8_t
int getResponseFromAddr(int &address) {
    if (!send(address)) {
        clearBuffer();
        return COMMUNICATION_COMM_ERR;
    }
    if (waitForResponse()) {
        int readData = Serial.read();
        return readData;
    } else {
        clearBuffer();
        return COMMUNICATION_RESP_ERR;
    }
}

int getResponseFromAddr(const uint8_t *address) {
    int parsed = *address;
    return getResponseFromAddr(parsed);
}