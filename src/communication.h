long const maxWaitTime = 50;

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
    //echo should be same as sent command, if not, there communication problem.
    return echo == command;
}