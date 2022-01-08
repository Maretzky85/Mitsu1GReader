#define stored_low 0x3B
#define stored_high 0x3C


int currentErrorsPresent = 0;

struct DTC
{
    int code;
    unsigned int mask;
    bool on;
    char name[14];
};

DTC errors[] = {
    {11, 1, false, "Oxygen sns"},
    {12, 2, false, "Air Flow"},
    {13, 4, false, "Air Temp"},
    {14, 8, false, "TPS"},
    {15, 16, false, "ISC mtr"},
    {21, 32, false, "Coolant"},
    {22, 64, false, "Speed"},
    {23, 128, false, "TDC"},
    {24, 256, false, "Speed"},
    {25, 512, false, "Baro"},
    {31, 1024, false, "Knock"},
    {41, 2048, false, "Injector"},
    {42, 4096, false, "FP relay"},
    {43, 8192, false, "EGR"},
    {44, 16384, false, "Ign coil"},
    {36, 32768, false, "Ign circuit"},
};

int errorsCount = 16;

bool parseDtcWithMask(unsigned int rawValue, unsigned int mask)
{
    if ((rawValue & mask) == mask)
    {
        return true;
    }
    else
    {
        return false;
    }
}

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
            printError(RESP_ERR);
        }
    }
    else
    {
        printError(COMM_ERR);
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
            printError(RESP_ERR);
        }
        unsigned int word = high * 256 + low;

        // bool on = parseDtcWithMask(word, errors[0].mask);
        // String result = String(on);
        currentErrorsPresent = 0;
        for (int i = 0; i < errorsCount; i++)
        {
            bool on = parseDtcWithMask(word, errors[i].mask);
            if (on)
            {
                currentErrorsPresent++;
            }
        };
        String result = String(currentErrorsPresent);
        printResult(result, " ");
    }
    else
    {
        printError(COMM_ERR);
        delay(250);
    }
}