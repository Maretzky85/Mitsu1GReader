#include <dataReader.h>

const char DATA_READER_HEADER[] PROGMEM = "DATA READER";

int currentRequest = 0;


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

char *readData(request *requestData) {
    int response = getResponseFromAddr(requestData->addr);
    if (response == COMM_ERR) {
        return nullptr;
    }
    // 16-bit reads (e.g. injPw at $29:$2A) fetch the low byte from addr+1
    // and pack the pair into one int as (high<<8)|low.
    if (requestData->parser == P_INJ_PULSE_16) {
        int lowAddr = requestData->addr + 1;
        int lowByte = getResponseFromAddr(lowAddr);
        if (lowByte == COMM_ERR) {
            return nullptr;
        }
        response = ((response & 0xFF) << 8) | (lowByte & 0xFF);
    }
    return parseData(response, requestData);
}

int getRequestNumberPlus(int addition) {
    int nextR = currentRequest + addition;
    if (nextR >= MAX_REQUESTS) {
        nextR = nextR - MAX_REQUESTS;
    }
    return nextR;
}

void dataReader() {
    printHeader(DATA_READER_HEADER);
    checkButtons();
    for (int i = 0; i < LCD_ROWS - 1; ++i) {
        int requestId = getRequestNumberPlus(i);
        printResultName(requests[requestId].name, i);
        printResult(readData(&requests[requestId]), i);
        delay(1); //without delay leads to too fast next request causes read error
    }
}