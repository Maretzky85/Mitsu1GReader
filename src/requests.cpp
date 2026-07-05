#include <requests.h>

// NAMES
const char BATTERY_NAME[] PROGMEM = "Battery";
const char ACC_ENRICHMENT_NAME[] PROGMEM = "Accel enrich";
const char COOLANT_NAME[] PROGMEM = "Coolant";
const char ENGINE_SPEED_NAME[] PROGMEM = "Engine Speed";
const char FUEL_TRIM_LOW_NAME[] PROGMEM = "Fuel Trim Low";
const char FUEL_TRIM_MEDIUM_NAME[] PROGMEM = "Fuel Trim Mid";
const char FUEL_TRIM_HIGH_NAME[] PROGMEM = "Fuel Trim High";
const char INJECTOR_PULSE_NAME[] PROGMEM = "Inj Pulse";
const char OXYGEN_FEEDBACK_NAME[] PROGMEM = "Oxygen Fdbck";
const char OXYGEN_SENSOR_NAME[] PROGMEM = "Oxygen Sens";
const char TPS_NAME[] PROGMEM = "TPS";
const char AIR_FLOW_SENSOR_NAME[] PROGMEM = "Air Flow";
const char AIR_TEMPERATURE_SENSOR_NAME[] PROGMEM = "Air Temp";
const char AIR_VOLUME_SENSOR_NAME[] PROGMEM = "Air Volume";
const char BAROMETRIC_SENSOR_NAME[] PROGMEM = "Baro Sensor";
const char ISC_STEPS_NAME[] PROGMEM = "ISC steps";
const char KNOCK_SUM_NAME[] PROGMEM = "Knock Sum";
const char IGNITION_ADVANCE_NAME[] PROGMEM = "Ign Advance";
const char EGR_TEMPERATURE_NAME[] PROGMEM = "Egr Temp";

// UNITS
const char VOLTS_NAME[] PROGMEM = "V";
const char PERCENT_NAME[] PROGMEM = "%";
const char CELSIUS_NAME[] PROGMEM = "C";
const char RPM_NAME[] PROGMEM = "RPM";
const char MILLISECONDS_NAME[] PROGMEM = "ms";
const char HERTZ_NAME[] PROGMEM = "Hz";
const char EMPTY_NAME[] PROGMEM = " ";
const char KPA_NAME[] PROGMEM = "kPa";
const char DEGREES_NAME[] PROGMEM = "C";

// MH6111 ECT/IAT thermistor lookup tables.
// Copied from ROM disassembly (Mitsubishi MMCD service curve), lines 378-444 of
// standard_E932_E931_source.asm. Values above ~127degC are clipped -- the sensor
// never reaches those in a running engine, and clipping keeps the table 8-bit.
const int8_t ectTable[256] PROGMEM = {
     127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  126,  123,  120,  118,  115,  112,
     110,  108,  105,  103,  101,   99,   97,   95,   93,   91,   89,   88,   86,   84,   83,   81,
      80,   79,   77,   76,   75,   74,   73,   72,   71,   70,   69,   68,   67,   66,   66,   65,
      64,   63,   62,   62,   61,   60,   59,   58,   58,   57,   56,   55,   55,   54,   53,   53,
      52,   51,   51,   50,   50,   49,   48,   48,   47,   47,   46,   46,   45,   44,   44,   44,
      43,   42,   42,   41,   41,   40,   40,   39,   39,   38,   38,   37,   37,   36,   36,   35,
      35,   34,   34,   34,   33,   33,   32,   32,   31,   31,   30,   30,   30,   29,   29,   28,
      28,   28,   27,   27,   26,   26,   25,   25,   24,   24,   24,   23,   23,   22,   22,   21,
      21,   21,   20,   20,   19,   19,   19,   18,   18,   18,   17,   17,   16,   16,   16,   15,
      15,   14,   14,   14,   13,   13,   12,   12,   12,   11,   11,   10,   10,    9,    9,    8,
       8,    8,    7,    7,    6,    6,    5,    5,    4,    4,    4,    3,    3,    2,    2,    1,
       1,    0,    0,    0,   -1,   -1,   -2,   -2,   -3,   -3,   -4,   -4,   -5,   -5,   -6,   -6,
      -7,   -8,   -8,   -9,   -9,  -10,  -10,  -11,  -12,  -12,  -13,  -13,  -14,  -14,  -15,  -15,
     -16,  -16,  -17,  -18,  -18,  -19,  -19,  -20,  -20,  -21,  -22,  -23,  -24,  -25,  -26,  -28,
     -29,  -30,  -32,  -34,  -36,  -38,  -40,  -42,  -44,  -46,  -48,  -50,  -52,  -54,  -56,  -57,
     -59,  -59,  -59,  -59,  -59,  -59,  -59,  -59,  -59,  -59,  -59,  -59,  -59,  -59,  -59,  -59,
};

const int8_t iatTable[256] PROGMEM = {
     127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  125,  122,  119,
     116,  113,  111,  108,  106,  104,  101,   99,   98,   96,   94,   92,   91,   89,   88,   86,
      85,   84,   82,   81,   80,   79,   78,   77,   76,   74,   74,   72,   72,   71,   70,   69,
      68,   67,   66,   66,   65,   64,   63,   62,   62,   61,   60,   59,   59,   58,   57,   57,
      56,   55,   55,   54,   53,   53,   52,   51,   51,   50,   49,   49,   48,   48,   47,   46,
      46,   45,   45,   44,   44,   43,   43,   42,   42,   41,   41,   40,   40,   39,   39,   38,
      38,   38,   37,   36,   36,   35,   35,   34,   34,   33,   33,   32,   32,   31,   31,   30,
      30,   30,   29,   29,   28,   28,   27,   27,   26,   26,   26,   25,   25,   24,   24,   23,
      23,   22,   22,   22,   21,   21,   20,   20,   20,   19,   19,   18,   18,   17,   17,   16,
      16,   16,   15,   15,   14,   14,   13,   13,   12,   12,   12,   11,   11,   10,   10,    9,
       9,    8,    8,    8,    7,    7,    6,    6,    6,    5,    5,    4,    4,    3,    3,    2,
       2,    2,    1,    0,    0,    0,   -1,   -2,   -2,   -3,   -3,   -4,   -4,   -5,   -6,   -6,
      -7,   -8,   -8,   -9,   -9,  -10,  -11,  -11,  -12,  -13,  -13,  -14,  -14,  -15,  -16,  -16,
     -17,  -18,  -18,  -19,  -19,  -20,  -21,  -22,  -22,  -23,  -24,  -25,  -26,  -27,  -28,  -30,
     -31,  -32,  -34,  -36,  -38,  -39,  -41,  -43,  -45,  -47,  -49,  -51,  -52,  -54,  -56,  -57,
     -59,  -59,  -59,  -59,  -59,  -59,  -59,  -59,  -59,  -59,  -59,  -59,  -59,  -59,  -59,  -59,
};

request requests[] = {

        {BATT_VOLTAGE,         P_12V,            BATTERY_NAME,                VOLTS_NAME},
        {ACC_ENRICH,           P_PERCENT,        ACC_ENRICHMENT_NAME,         PERCENT_NAME},
        {COOLANT_TEMP,         P_COOLANT_TEMP,   COOLANT_NAME,                CELSIUS_NAME},
        {ENGINE_SPEED,         P_RPM,            ENGINE_SPEED_NAME,           RPM_NAME},
        {FUEL_TRIM_LOW,        P_FEEDBACK_TRIM,  FUEL_TRIM_LOW_NAME,          PERCENT_NAME},
        {FUEL_TRIM_MID,        P_FEEDBACK_TRIM,  FUEL_TRIM_MEDIUM_NAME,       PERCENT_NAME},
        {FUEL_TRIM_HIGH,       P_FEEDBACK_TRIM,  FUEL_TRIM_HIGH_NAME,         PERCENT_NAME},
        {INJECTOR_PULSE_HI,    P_INJ_PULSE_16,   INJECTOR_PULSE_NAME,         MILLISECONDS_NAME},
        {OXYGEN_FEEDBACK_TRIM, P_FEEDBACK_TRIM,  OXYGEN_FEEDBACK_NAME,        PERCENT_NAME},
        {OXYGEN_SENSOR,        P_ZERO_ONE,       OXYGEN_SENSOR_NAME,          VOLTS_NAME},
        {TPS,                  P_PERCENT,        TPS_NAME,                    PERCENT_NAME},
        {AIR_FLOW_HZ,          P_AIR_FLOW_HZ,    AIR_FLOW_SENSOR_NAME,        HERTZ_NAME},
        {AIR_TEMP,             P_AIR_TEMP,       AIR_TEMPERATURE_SENSOR_NAME, CELSIUS_NAME},
        {AIR_VOLUME,           P_RAW,            AIR_VOLUME_SENSOR_NAME,      EMPTY_NAME},
        {BARO_SENSOR,          P_BARO,           BAROMETRIC_SENSOR_NAME,      KPA_NAME},
        {ISC_STEPS,            P_ISC,            ISC_STEPS_NAME,              PERCENT_NAME},
        {KNOCK_SUM,            P_RAW,            KNOCK_SUM_NAME,              EMPTY_NAME},
        {TIMING_ADVANCE,       P_TIMING_ADVANCE, IGNITION_ADVANCE_NAME,       DEGREES_NAME},
        {EGR_TEMP,             P_EGR_TEMP,       EGR_TEMPERATURE_NAME,        CELSIUS_NAME}};

const int MAX_REQUESTS = sizeof(requests) / sizeof(*requests);

char buffer[18] = "";
const char parseFormat[] = "%5d%-3s";
const char parseFloatFormat[] = "%2d.%-2d%-3s";

void parseISC(int rawValue, char *unit) {
    int result = 100 * rawValue / 120;
    sprintf(buffer, parseFormat, result, unit);
}

void parseEgrTemp(int rawValue, char *unit) {
    // ROM annotation ($cf): degF = -2.7*raw + 597.7 -> degC = -1.5*raw + 314.3
    int result = -1.5 * rawValue + 314; // NOLINT(cppcoreguidelines-narrowing-conversions)
    sprintf(buffer, parseFormat, result, unit);
}

void parseAirTemp(int rawValue, char *unit) {
    int result = (int8_t) pgm_read_byte(&iatTable[rawValue & 0xFF]);
    sprintf(buffer, parseFormat, result, unit);
}

void parseAirFlowHz(int rawValue, char *unit) {
    int result = 6.25 * rawValue; // NOLINT(cppcoreguidelines-narrowing-conversions)
    sprintf(buffer, parseFormat, result, unit);
}

void parseTimingAdvance(int rawValue, char *unit) {
    int result = rawValue - 10;
    sprintf(buffer, parseFormat, result, unit);
}

void parseToTwelve(int &rawValue, char *unit) {
    float result = rawValue * 0.0733f;
    char tmpInt1 = result; // NOLINT(cppcoreguidelines-narrowing-conversions)
    float tmpFrac = result - tmpInt1;
    char tmpInt2 = trunc(tmpFrac * 100); // NOLINT(cppcoreguidelines-narrowing-conversions)
    snprintf(buffer, 20 - 8, parseFloatFormat, tmpInt1, tmpInt2, unit);
}

void parseToPercent(int rawValue, char *unit) {
    int result = 100 * rawValue / 255;
    sprintf(buffer, parseFormat, result, unit);
}

void parseFeedbackTrim(int rawValue, char *unit) {
    // ROM annotation: 0.78% per LSB with $80 = 100% (no correction).
    // Displayed as offset from stoich: (raw - 128) * 100 / 128.
    int result = ((rawValue - 128) * 100) / 128;
    sprintf(buffer, parseFormat, result, unit);
}

void parseZeroOne(int rawValue, char *unit) {
    float result = rawValue * 0.01952f;
    char tmpInt1 = result; // NOLINT(cppcoreguidelines-narrowing-conversions)
    float tmpFrac = result - tmpInt1;
    char tmpInt2 = trunc(tmpFrac * 100); // NOLINT(cppcoreguidelines-narrowing-conversions)
    sprintf(buffer, parseFloatFormat, tmpInt1, tmpInt2, unit);
}

void parseRPM(int rawValue, char *unit) {
    int result = rawValue * 31.25; // NOLINT(cppcoreguidelines-narrowing-conversions)
    sprintf(buffer, parseFormat, result, unit);
}

void parseCoolant(int rawValue, char *unit) {
    int result = (int8_t) pgm_read_byte(&ectTable[rawValue & 0xFF]);
    sprintf(buffer, parseFormat, result, unit);
}

void parseInjPulse16(int rawValue, char *unit) {
    // rawValue is the full 16-bit injPw in microseconds (high<<8 | low).
    if (rawValue != 0) {
        int intMs = rawValue / 1000;
        int fracMs = (rawValue % 1000) / 10;   // hundredths of ms
        sprintf(buffer, "%2d.%-2d%-3s", intMs, fracMs, unit);
    } else {
        sprintf(buffer, parseFormat, 0, unit);
    }
}

void parseBaro(int rawValue, char *unit) {
    double result = 0.486 * rawValue;
    char tmpInt1 = result; // NOLINT(cppcoreguidelines-narrowing-conversions)
    double tmpFrac = result - tmpInt1;
    char tmpInt2 = trunc(tmpFrac * 100);
    snprintf(buffer, 20 - 8, parseFloatFormat, tmpInt1, tmpInt2, unit);
}

char *parseData(int &data, request *requestData) {
    char unit[4];
    strcpy_P(unit, requestData->unit);
    int parser = requestData->parser;
    switch (parser) {
        case P_RAW:
            sprintf(buffer, parseFormat, data, unit);
            break;
        case P_12V:
            parseToTwelve(data, unit);
            break;
        case P_PERCENT:
            parseToPercent(data, unit);
            break;
        case P_FEEDBACK_TRIM:
            parseFeedbackTrim(data, unit);
            break;
        case P_ZERO_ONE:
            parseZeroOne(data, unit);
            break;
        case P_RPM:
            parseRPM(data, unit);
            break;
        case P_TIMING_ADVANCE:
            parseTimingAdvance(data, unit);
            break;
        case P_COOLANT_TEMP:
            parseCoolant(data, unit);
            break;
        case P_INJ_PULSE_16:
            parseInjPulse16(data, unit);
            break;
        case P_AIR_FLOW_HZ:
            parseAirFlowHz(data, unit);
            break;
        case P_AIR_TEMP:
            parseAirTemp(data, unit);
            break;
        case P_BARO:
            parseBaro(data, unit);
            break;
        case P_EGR_TEMP:
            parseEgrTemp(data, unit);
            break;
        case P_ISC:
            parseISC(data, unit);
            break;
        default:
            sprintf(buffer, "%11s%3d", "", data);
            break;
    }
    return buffer;
}
