long start = millis();
int receivedResponses = 0;
int requestPerSecond = 0;

int currentRequest = 0;

bool error = false;

buttons _dataReaderButtonState = NONE;

void setButtonState(buttons state) {
    _dataReaderButtonState = state;
}

void updateResponses()
{
    if (millis() - start > 1000)
    {
        requestPerSecond = receivedResponses;
        receivedResponses = 0;
        start = millis();
    }
}

void checkButtons()
{
    if (_dataReaderButtonState == NEXT)
    {
        currentRequest++;
        if (currentRequest == MAX_REQUEST)
        {
            currentRequest = 0;
        }
    }
    if (_dataReaderButtonState == PREVIOUS)
    {
        currentRequest--;
        if (currentRequest < 0)
        {
            currentRequest = MAX_REQUEST - 1;
        }
    }
}

void readAndDisplayData()
{
    checkButtons();
    printRequestName(currentRequest);
    printRps(requestPerSecond);
    bool successSend = send(requests[currentRequest].addr);
    if (successSend == false)
    {
        printError(COMM_ERR);
        delay(250);
        error = true;
    };
    if (waitForResponse())
    {
        int readData = Serial.read();
        receivedResponses++;
        if (!error)
        {
            printResult(parseData(readData, requests[currentRequest].parser), requests[currentRequest].unit);
        }
        else
        {
            error = false;
            printResult(parseData(readData, requests[currentRequest].parser), requests[currentRequest].unit, true);
        }
    }
    else if (!error)
    {
        printError(RESP_ERR);
        delay(250);
    }
    updateResponses();
}