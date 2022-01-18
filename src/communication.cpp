#include <communication.h>

char COMM_ERR[9] = "Comm Err";
char RESP_ERR[9] = "Resp Err";

const uint8_t MAX_WAIT_TIME = 50;

void _clearBuffer()
{
    while (Serial.available())
    {
        Serial.read();
    }
}

bool _waitForResponse()
{
    long startWaitTime = millis();
    while (Serial.available() == 0)
    {
        if (millis() - startWaitTime > MAX_WAIT_TIME)
        {
            return false;
        }
    }
    return true;
}

bool _send(int &command)
{
    Serial.write(command);
    if (!_waitForResponse())
    {
        return false;
    }
    int echo = Serial.read();
    if (echo != command)
    {
        _clearBuffer();
    }

    //echo should be same as sent command, if not, there communication problem.
    return echo == command;
}

int getResponseFromAddr(int &address)
{
    if (!_send(address))
    {
        _clearBuffer();
        return COMMUNICATION_COMM_ERR;
    }
    if (_waitForResponse())
    {
        int readData = Serial.read();
        return readData;
    }
    else
    {
        _clearBuffer();
        return COMMUNICATION_RESP_ERR;
    }
}