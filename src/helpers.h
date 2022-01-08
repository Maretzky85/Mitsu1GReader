struct request
{
    int addr;
    int parser;
    String name;
    String unit;
};

const int SWITCHES PROGMEM = 2;
const int BATT_VOLTAGE PROGMEM = 20;
const int ACC_ENRICH PROGMEM = 29;
const int COOLANT_TEMP PROGMEM = 7;
const int ENGINE_SPEED PROGMEM = 33;
const int FUEL_TRIM_LOW PROGMEM = 12;
const int FUEL_TRIM_MID PROGMEM = 13;
const int FUEL_TRIM_HIGH PROGMEM = 14;
const int INJECTOR_PULSE PROGMEM = 41;
const int OXYGEN_FEEDBACK_TRIM PROGMEM = 14;
const int OXYGEN_SENSOR PROGMEM = 19;
const int TPS PROGMEM = 23;
const int AIR_FLOW_HZ PROGMEM = 26;
const int AIR_TEMP PROGMEM = 58;
const int AIR_VOLUME PROGMEM = 44;
const int BARO_SENSOR PROGMEM = 21;
const int ISC_STEPS PROGMEM = 22;
const int KNOCK_SUM PROGMEM = 38;
const int TIMING_ADVANCE PROGMEM = 6;
const int EGR_TEMP PROGMEM = 18;

const int REQUESTS_SIZE PROGMEM = 19;

#define MASK_TDC 4
#define MASK_PS 8
#define MASK_AC_SW 16
#define MASK_PARK_N 32
#define MASK_IDDLE_SW 128

#define RAW 0
#define P_12V 1
#define P_FEEDBACK_TRIM 2
#define ZERO_ONE 3
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
    // {SWITCHES, P_IDDLE_SW, "Iddle switch", " "},
    {BATT_VOLTAGE, P_12V, "Battery voltage", "V"},
    {ACC_ENRICH, P_PERCENT, "Acceleration enrichment", "%"},
    {COOLANT_TEMP, P_COOLING_TEMP, "Coolant temp", "C"},
    {ENGINE_SPEED, P_RPM, "Engine speed", "RPM"},
    {FUEL_TRIM_LOW, P_FEEDBACK_TRIM, "Fuel Trim Low", "%"},
    {FUEL_TRIM_MID, P_FEEDBACK_TRIM, "Fuel Trim Mid", "%"},
    {FUEL_TRIM_HIGH, P_FEEDBACK_TRIM, "Fuel Trim High", "%"},
    {INJECTOR_PULSE, P_INJ_PULSE, "Injector Pulse", "ms"},
    {OXYGEN_FEEDBACK_TRIM, P_FEEDBACK_TRIM, "Oxygen Feedback Trim", "%"},
    {OXYGEN_SENSOR, ZERO_ONE, "Oxygen Sensor", "V"},
    {TPS, P_PERCENT, "Throttle Position", "%"},
    {AIR_FLOW_HZ, P_AIR_FLOW_HZ, "Air Flow", "Hz"},
    {AIR_TEMP, P_AIR_TEMP, "Air Temperature", "C"},
    {AIR_VOLUME, RAW, "Air Volume", " "},
    {BARO_SENSOR, P_BARO, "Barometric Sensor", "Hpa"},
    {ISC_STEPS, RAW, "ISC steps", "Steps"},
    {KNOCK_SUM, RAW, "Knock Sum", "Knocks"},
    {TIMING_ADVANCE, P_TIMING_ADVANCE, "Timing Advance", "deg"},
    {EGR_TEMP, P_EGR_TEMP, "Egr Temperature", "C"}};

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
    return -0.81 * rawValue + 153;
    // return 0.94 * rawValue + 181;
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
    case RAW:
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
    case ZERO_ONE:
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
