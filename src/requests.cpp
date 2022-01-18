#include <requests.h>

const char *EMPTY[] = {""};

#define MASK_TDC 4
#define MASK_PS 8
#define MASK_AC_SW 16
#define MASK_PARK_N 32
#define MASK_IDDLE_SW 128

request requests[] = {
    // {SWITCHES, P_TDC, "TDC", " "},
    // {SWITCHES, P_PS, "Power Steering", " "},
    // {SWITCHES, P_AC_SW, "A/C Switch", " "},
    // {SWITCHES, P_PARK_NEUTRAL, "PARK/NEUTRAL", " "},
    // {SWITCHES, P_IDDLE_SW, "Iddle switch", " "}, //TODO move to different routine
    {BATT_VOLTAGE, P_12V, "Batt", "V"},
    {ACC_ENRICH, P_PERCENT, "Acc enrich", "%"},
    {COOLANT_TEMP, P_COOLING_TEMP, "Coolant", "C"},
    {ENGINE_SPEED, P_RPM, "Engine Speed", "RPM"},
    {FUEL_TRIM_LOW, P_FEEDBACK_TRIM, "Fuel Trim Low", "%"},
    {FUEL_TRIM_MID, P_FEEDBACK_TRIM, "Fuel Trim Mid", "%"},
    {FUEL_TRIM_HIGH, P_FEEDBACK_TRIM, "Fuel Trim Hgh", "%"},
    {INJECTOR_PULSE, P_INJ_PULSE, "Inj Pulse", "ms"},
    {OXYGEN_FEEDBACK_TRIM, P_FEEDBACK_TRIM, "Oxygen Fdbck", "%"},
    {OXYGEN_SENSOR, P_ZERO_ONE, "Oxygen Sens", "V"},
    {TPS, P_PERCENT, "TPS", "%"},
    {AIR_FLOW_HZ, P_AIR_FLOW_HZ, "Air Flow", "Hz"},
    {AIR_TEMP, P_AIR_TEMP, "Air Temp", "C"},
    {AIR_VOLUME, P_RAW, "Air Volume", " "},
    {BARO_SENSOR, P_BARO, "Baro Sensor", "Hpa"},
    {ISC_STEPS, P_RAW, "ISC steps", "Step"},
    {KNOCK_SUM, P_RAW, "Knock Sum", " "},
    {TIMING_ADVANCE, P_TIMING_ADVANCE, "Ign Advance", "deg"},
    {EGR_TEMP, P_EGR_TEMP, "Egr Temp", "C"}};

const int MAX_REQUESTS = sizeof(requests) / sizeof(* requests);

char buffer[15] = "";

//TODO do not use string, and move to different file
// bool parseWithMask(int rawValue, int mask)
// {
//     return (rawValue & mask) == mask;
// }

void parseEgrTEmp(int rawValue, char* unit)
{
    int result = -2.14 * rawValue + 314;
    sprintf(buffer, "%4s%3d %-6s", *EMPTY, result, unit);
}

void parseAirTemp(int rawValue, char* unit)
{
    // return -0.81 * rawValue + 153;
    int result = -0.94 * rawValue + 181;
    sprintf(buffer, "%4s%3d %-6s", *EMPTY, result, unit);
}

void parseAirFlowHz(int rawValue, char* unit)
{
    int result = 6.29 * rawValue;
    sprintf(buffer, "%4s%4d %-5s", *EMPTY, result, unit);
}

void parseTimingAdvance(int rawValue, char* unit)
{
    int result = rawValue - 20;
    sprintf(buffer, "%6d %-7s", result, unit);
}

void parseToTwelve(int &rawValue, char* unit)
{
    float result = rawValue * 0.0733f;
    char tmpInt1 = result;
    float tmpFrac = result - tmpInt1;
    char tmpInt2 = trunc(tmpFrac * 100);
    sprintf(buffer, "%6d.%-2d %-4s", tmpInt1, tmpInt2, unit);
};

void parseToPercent(int rawValue, char* unit)
{
    int result = 100 * rawValue / 255;
    sprintf(buffer, "%7d %-6s", result, unit);
}

void parseFeedbackTrim(int rawValue, char* unit)
{
    int result = rawValue * 0.78;
    sprintf(buffer, "%5s%3d %-5s", *EMPTY, result, unit);
}

void parseZeroOne(int rawValue, char* unit)
{
    float result = rawValue * 0.0195f;
    char tmpInt1 = result;
    float tmpFrac = result - tmpInt1;
    char tmpInt2 = trunc(tmpFrac * 100);
    sprintf(buffer, "%6d.%-2d %-4s", tmpInt1, tmpInt2, unit);
}

void parseRPM(int rawValue, char* unit)
{
    int result = rawValue * 31.25;
    sprintf(buffer, "%4s%4d %-5s", *EMPTY, result, unit);
}

void parseCoolant(int rawValue, char* unit)
{
    int result = -0.81 * rawValue + 153;
    sprintf(buffer, "%5s%3d %-5s", *EMPTY, result, unit);
}

void parseInjPulse(int rawValue, char* unit)
{
    int result = 0.256 * rawValue;
    sprintf(buffer, "%7d %-6s", result, unit);
}

void parseBaro(int rawValue, char* unit)
{
    int result = 0.00486 * rawValue * 1000;
    sprintf(buffer, "%4s%4d %-5s", *EMPTY, result, unit);
}

char *parseData(int &data, request * requestData)
{
    int parser = requestData->parser;
    switch (parser)
    {
    case P_RAW:
        sprintf(buffer, "%4s%3d %-6s", "", data, requestData->unit);
        break;
    case P_12V:
        parseToTwelve(data, requestData->unit);
        break;
    case P_PERCENT:
        parseToPercent(data, requestData->unit);
        break;
    case P_FEEDBACK_TRIM:
        parseFeedbackTrim(data, requestData->unit);
        break;
    case P_ZERO_ONE:
       parseZeroOne(data, requestData->unit);
        break;
    case P_RPM:
        parseRPM(data, requestData->unit);
        break;
    case P_TIMING_ADVANCE:
        parseTimingAdvance(data, requestData->unit);
        break;
    case P_COOLING_TEMP:
        parseCoolant(data, requestData->unit);
        break;
    case P_INJ_PULSE:
        parseInjPulse(data, requestData->unit);
        break;
    case P_AIR_FLOW_HZ:
        parseAirFlowHz(data, requestData->unit);
        break;
    case P_AIR_TEMP:
        parseAirTemp(data, requestData->unit);
        break;
    case P_BARO:
        parseBaro(data, requestData->unit);
        break;
    case P_EGR_TEMP:
        parseEgrTEmp(data, requestData->unit);
        break;

    // case P_TDC:
    //     return String(parseWithMask(data, MASK_TDC));
    //     break;
    // case P_PS:
    //     return String(parseWithMask(data, MASK_PS));
    //     break;
    // case P_AC_SW:
    //     return String(parseWithMask(data, MASK_AC_SW));
    //     break;
    // case P_PARK_NEUTRAL:
    //     return String(parseWithMask(data, MASK_PARK_N));
    //     break;
    // case P_IDDLE_SW:
    //     return String(parseWithMask(data, MASK_IDDLE_SW));
    //     break;
    default:
        sprintf(buffer, "%11s%3d", "", data);
        break;
    }
    return buffer;
};
