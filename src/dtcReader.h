#define stored_low 0x3B
#define stored_high 0x3C

int currentErrorsPresent = 0;

struct DTC
{
    int code;
    unsigned int mask;
    String name;
    bool on;
};

DTC errors[] = {
    {11, 1, "Oxygen sensor", false},
    {12, 2, "Intake Air Flow", false},
    {13, 4, "Intake Air Temp", false},
    {14, 8, "TPS sensor", false},
    {15, 16, "ISC motor", false},
    {21, 32, "Coolant Temp", false},
    {22, 64, "Speed sensor", false},
    {23, 128, "TDC sensor", false},
    {24, 256, "Speed sensor", false},
    {25, 512, "Baro sensor", false},
    {31, 1024, "Knock sensor", false},
    {41, 2048, "Injector circuit", false},
    {42, 4096, "Fuel pump relay", false},
    {43, 8192, "EGR", false},
    {44, 16384, "Ignition Coil", false},
    {36, 32768, "Ignition Circuit", false},
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
        unsigned int word = high * 256 + low;

        // bool on = parseDtcWithMask(word, errors[0].mask);
        // String result = String(on);
        // currentErrorsPresent = 0;
        // for (int i = 0; i < errorsCount; i++)
        // {
        //     bool on = parseDtcWithMask(word, errors[i].mask);
        //     if (on)
        //     {
        //         currentErrorsPresent++;
        //     }
             
        // };
        String result = String(currentErrorsPresent);
        printResult(result, " ");
    }
    else
    {
        printError("COMM ERROR");
        delay(250);
    }
}