#define stored_low 0x3B
#define stored_high 0x3C

void readDtcBytes()
{
    int low = 0;
    int high = 0;
    printHeader("DTC READER");
    bool sendLow = send(stored_low);
    if (sendLow)
    {
        if (waitForResponse())
        {
            low = Serial.read();
            String result = String(low);
            // printResult(result, "DTC");
        }
        else
        {
            printError("RESPONSE ERROR");
        }
    }
    else
    {
        printError("COMM ERROR");
        delay(250);
    }
    bool sendHigh = send(stored_high);
    if (sendHigh)
    {
        if (waitForResponse())
        {
            high = Serial.read();
            String result = String(high);
            // printResult(result, "DTC");
        }
        else
        {
            printError("RESPONSE ERROR");
        }
        String result = String(low) + " " + String(high);
        printResult(result, " ");
    }
    else
    {
        printError("COMM ERROR");
        delay(250);
    }
}