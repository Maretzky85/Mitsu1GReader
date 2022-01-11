struct request
{
    int addr;
    int parser;
    char name[14];
    char unit[5];
};

enum requests_addr {
SWITCHES = 0x02,
BATT_VOLTAGE = 0x14,
ACC_ENRICH = 0x1D,
COOLANT_TEMP  = 0x07,
ENGINE_SPEED  = 0x21,
FUEL_TRIM_LOW = 0x0C,
FUEL_TRIM_MID = 0x0D,
FUEL_TRIM_HIGH = 0x0E,
INJECTOR_PULSE = 0x29,
OXYGEN_FEEDBACK_TRIM = 0x0F,
OXYGEN_SENSOR= 0x13,
TPS = 0x17,
AIR_FLOW_HZ = 0x1A,
AIR_TEMP = 0x3A,
AIR_VOLUME = 0x2C,
BARO_SENSOR = 0x15,
ISC_STEPS = 0x16,
KNOCK_SUM = 0x26,
TIMING_ADVANCE = 0x06,
EGR_TEMP = 0x12,
MAX_REQUEST
};

#define MASK_TDC 4
#define MASK_PS 8
#define MASK_AC_SW 16
#define MASK_PARK_N 32
#define MASK_IDDLE_SW 128

#define P_RAW 0
#define P_12V 1
#define P_FEEDBACK_TRIM 2
#define P_ZERO_ONE 3
#define P_PERCENT 4
#define P_RPM 5
#define P_TIMING_ADVANCE 6
#define P_COOLING_TEMP 7
#define P_INJ_PULSE 8
#define P_AIR_FLOW_HZ 9
#define P_AIR_TEMP 10
#define P_BARO 11
#define P_EGR_TEMP 12
#define P_TDC 13
#define P_PS 14
#define P_AC_SW 15
#define P_PARK_NEUTRAL 16
#define P_IDDLE_SW 17

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


//TODO do not use string, and move to different file
String parseWithMask(int rawValue, int mask)
{
    if ((rawValue & mask) == mask)
    {
        return "ON";
    }
    else
    {
        return "OFF";
    }
}   

int parseEgrTEmp(int rawValue)
{
    return -2.14 * rawValue + 314;
}

int parseAirTemp(int rawValue)
{
    // return -0.81 * rawValue + 153;
    return -0.94 * rawValue + 181;
}

int parseAirFlowHz(int rawValue)
{
    return 6.29 * rawValue;
}

int parseTimingAdvance(int rawValue)
{
    return rawValue - 10;
}

float parseToTwelve(int rawValue)
{
    return rawValue * 0.0733;
};

int parseToPercent(int rawValue)
{
    return 100 * rawValue / 255;
}

int parseFeedbackTrim(int rawValue)
{
    return rawValue * 0.78;
}

float parseZeroOne(int rawValue)
{
    return rawValue * 0.0195;
}

int parseRPM(int rawValue)
{
    return rawValue * 31.25;
}

int parseCoolant(int rawValue)
{
    return -0.81 * rawValue + 153;
}

int parseInjPulse(int rawValue)
{
    return 0.256 * rawValue;
}

int parseBaro(int rawValue)
{
    return 0.00486 * rawValue;
}

String parseData(int data, int parser)
{
    switch (parser)
    {
    case P_RAW:
        return String(data);
        break;
    case P_12V:
        return String(parseToTwelve(data));
        break;
    case P_PERCENT:
        return String(parseToPercent(data));
        break;
    case P_FEEDBACK_TRIM:
        return String(parseFeedbackTrim(data));
        break;
    case P_ZERO_ONE:
        return String(parseZeroOne(data));
        break;
    case P_RPM:
        return String(parseRPM(data));
        break;
    case P_TIMING_ADVANCE:
        return String(parseTimingAdvance(data));
        break;
    case P_COOLING_TEMP:
        return String(parseCoolant(data));
        break;
    case P_INJ_PULSE:
        return String(parseInjPulse(data));
        break;
    case P_AIR_FLOW_HZ:
        return String(parseAirFlowHz(data));
        break;
    case P_AIR_TEMP:
        return String(parseAirTemp(data));
        break;
    case P_BARO:
        return String(parseBaro(data));
        break;
    case P_EGR_TEMP:
        return String(parseEgrTEmp(data));
        break;
    case P_TDC:
        return parseWithMask(data, MASK_TDC);
        break;
    case P_PS:
        return parseWithMask(data, MASK_PS);
        break;
    case P_AC_SW:
        return parseWithMask(data, MASK_AC_SW);
        break;
    case P_PARK_NEUTRAL:
        return parseWithMask(data, MASK_PARK_N);
        break;
    case P_IDDLE_SW:
        return parseWithMask(data, MASK_IDDLE_SW);
        break;
    default:
        return String(data);
        break;
    }
};
