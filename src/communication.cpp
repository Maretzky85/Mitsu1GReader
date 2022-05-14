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


int getResponseFromAddr(int &address) {
    Serial.write(address);
    unsigned long startWaitTime = millis();
    while (Serial.available() != 2) {
        if (millis() - startWaitTime > MAX_WAIT_TIME) {
            if (Serial.available() > 0) {
                Serial.flush();
            }
            communicationStatus = STATUS_ERR_COMM;
            return COMM_ERR;
        }
    }
    int echo = Serial.read();
    int response = Serial.read();

    if (echo != address) {
        communicationStatus = STATUS_ERR_RESP;
        Serial.flush();
        return COMM_ERR;
    }
    receivedResponses++;
    communicationStatus = STATUS_OK;
    return response;
}

int getResponseFromAddr(const uint8_t *address) {
    int parsed = *address;
    return getResponseFromAddr(parsed);
}