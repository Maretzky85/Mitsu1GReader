const char COMM_ERR[] = "Comm Err";
const char RESP_ERR[] = "Resp Err";
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
