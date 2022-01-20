#include <dataReader.h>

unsigned long start = millis();
int receivedResponses = 0;
int requestPerSecond = 0;

int currentRequest = 0;

bool error = false;

void updateResponses() {
    if (millis() - start > 1000) {
        requestPerSecond = receivedResponses;
        receivedResponses = 0;
        start = millis();
    }
}

void checkButtons() {
    if (buttonState == NEXT) {
        currentRequest++;
        if (currentRequest == MAX_REQUESTS) {
            currentRequest = 0;
        }
    }
    if (buttonState == PREVIOUS) {
        currentRequest--;
        if (currentRequest < 0) {
            currentRequest = MAX_REQUESTS - 1;
        }
    }
}

void readAndDisplayData() {
    checkButtons();
    printHeader(requests[currentRequest].name);
    printRps(requestPerSecond);
    bool successSend = send(requests[currentRequest].addr);
    if (!successSend) {
        printError(COMM_ERR);
        error = true;
    }
    if (waitForResponse()) {
        int readData = Serial.read();
        receivedResponses++;
        if (!error) {
            printResult(parseData(readData, &requests[currentRequest]));
        } else {
            error = false;
            printResult(parseData(readData, &requests[currentRequest]), true);
        }
    } else if (!error) {
        printError(RESP_ERR);
    }
    updateResponses();
}