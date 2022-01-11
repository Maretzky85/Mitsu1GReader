#define COMMUNICATION_COMM_ERR -1
#define COMMUNICATION_RESP_ERR -2
char COMM_ERR[9] = "Comm Err";
char RESP_ERR[9] = "Resp Err";
long const maxWaitTime = 50;

void clearBuffer()
{
    while (Serial.available())
    {
        Serial.read();
    }
}

bool waitForResponse()
{
    long startWaitTime = millis();
    while (Serial.available() == 0)
    {
        if (millis() - startWaitTime > maxWaitTime)
        {
            return false;
        }
    }
    return true;
}

bool send(int command)
{
    Serial.write(command);
    if (!waitForResponse())
    {
        return false;
    }
    int echo = Serial.read();
    if (echo != command)
    {
        clearBuffer();
    }

    //echo should be same as sent command, if not, there communication problem.
    return echo == command;
}

int getResponseFromAddr(int address)
{
    bool sendSuccesfull = send(address);
    if (sendSuccesfull)
    {
        bool isResponseAvailable = waitForResponse();
        if (isResponseAvailable)
        {
            int readData = Serial.read();
            return readData;
        }
        else
        {
            clearBuffer();
            return COMMUNICATION_RESP_ERR;
        }
    }
    else
    {
        clearBuffer();
        return COMMUNICATION_COMM_ERR;
    }
}