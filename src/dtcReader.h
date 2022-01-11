#define stored_low 0x38
#define stored_high 0x39
#define NO_DTC "NO ERRORS"
int currentErrorsPresent = 0;
int displayingError = 0;

struct DTC
{
    int code;
    unsigned int mask;
    bool on;
    char name[14];
};

DTC errors[] = {
    {11, 0x0001, false, "Oxygen sns"},
    {12, 0x0002, false, "Air Flow"},
    {13, 0x0004, false, "Air Temp"},
    {14, 0x0008, false, "TPS"},
    {15, 0x0010, false, "ISC mtr"},
    {21, 0x0020, false, "Coolant"},
    {22, 0x0040, false, "Speed"},
    {23, 0x0080, false, "TDC"},
    {24, 0x0100, false, "Speed"},
    {25, 0x0200, false, "Baro"},
    {31, 0x0400, false, "Knock"},
    {41, 0x0800, false, "Injector"},
    {42, 0x1000, false, "FP relay"},
    {43, 0x2000, false, "EGR"},
    {44, 0x4000, false, "Ign coil"},
    {36, 0x8000, false, "Ign circuit"},
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
    char dtcResult[14] = {' '};
    int lowByte = getResponseFromAddr(stored_low);
    delay(5);
    int highByte = getResponseFromAddr(stored_high);
    if (lowByte == COMMUNICATION_COMM_ERR || highByte == COMMUNICATION_COMM_ERR)
    {
        printError(COMM_ERR);
    }
    else if (lowByte == COMMUNICATION_RESP_ERR || highByte == COMMUNICATION_RESP_ERR)
    {
        printError(RESP_ERR);
    }
    else
    {
        printHeader("DTC READER");
        unsigned int word = highByte * 256 + lowByte;

        int presentErrors = 0;
        for (int i = 0; i < errorsCount; i++)
        {
            bool isOn = parseDtcWithMask(word, errors[i].mask);
            if (isOn)
            {
                errors[i].on = true;
                presentErrors++;
            }
            else
            {
                errors[i].on = false;
            }
        };
        currentErrorsPresent = presentErrors;
        printDtcCount(currentErrorsPresent);
        for (int i = 0; i < errorsCount; i++)
        {
            if (errors[i].on)
            {
                // sprintf(dtcResult, "%s %d", errors[i].name, errors[i].code);
                printResult(errors[i].name, " ");
                delay(500);
                // dtcResult = &errors[i].name;
            }
        }
        // printResult("No errors", " ");
    }
}