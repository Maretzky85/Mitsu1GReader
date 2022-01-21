#include <requests.h>

//Strings

//NAMES
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

//UNITS
const char VOLTS_NAME[] PROGMEM = "V";
const char PERCENT_NAME[] PROGMEM = "%";
const char CELSIUS_NAME[] PROGMEM = "C";
const char RPM_NAME[] PROGMEM = "RPM";
const char MILLISECONDS_NAME[] PROGMEM = "ms";
const char HERTZ_NAME[] PROGMEM = "Hz";
const char EMPTY_NAME[] PROGMEM = " ";
const char KPA_NAME[] PROGMEM = "kPa";
const char DEGREES_NAME[] PROGMEM = "C";

enum parsers {
    P_RAW,
    P_12V,
    P_FEEDBACK_TRIM,
    P_ZERO_ONE,
    P_PERCENT,
    P_RPM,
    P_TIMING_ADVANCE,
    P_COOLING_TEMP,
    P_INJ_PULSE,
    P_AIR_FLOW_HZ,
    P_AIR_TEMP,
    P_BARO,
    P_EGR_TEMP,
    P_ISC
};


request requests[] = {

        {BATT_VOLTAGE,         P_12V,            BATTERY_NAME,                VOLTS_NAME},
        {ACC_ENRICH,           P_PERCENT,        ACC_ENRICHMENT_NAME,         PERCENT_NAME},
        {COOLANT_TEMP,         P_COOLING_TEMP,   COOLANT_NAME,                CELSIUS_NAME},
        {ENGINE_SPEED,         P_RPM,            ENGINE_SPEED_NAME,           RPM_NAME},
        {FUEL_TRIM_LOW,        P_FEEDBACK_TRIM,  FUEL_TRIM_LOW_NAME,          PERCENT_NAME},
        {FUEL_TRIM_MID,        P_FEEDBACK_TRIM,  FUEL_TRIM_MEDIUM_NAME,       PERCENT_NAME},
        {FUEL_TRIM_HIGH,       P_FEEDBACK_TRIM,  FUEL_TRIM_HIGH_NAME,         PERCENT_NAME},
        {INJECTOR_PULSE,       P_INJ_PULSE,      INJECTOR_PULSE_NAME,         MILLISECONDS_NAME},
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

int convertToCelsius(double faren) {
    return (faren - 32.0) * 5 / 9; // NOLINT(cppcoreguidelines-narrowing-conversions)
}

void parseISC(int rawValue, char *unit) {
    int result = 100 * rawValue / 120;
    sprintf(buffer, parseFormat, result, unit);
}

void parseEgrTEmp(int rawValue, char *unit) {
    int result = convertToCelsius(-2.7 * rawValue + 597); // NOLINT(cppcoreguidelines-narrowing-conversions)
    sprintf(buffer, parseFormat, result, unit);
}

void parseAirTemp(int rawValue, char *unit) {
    int result = convertToCelsius(1.8 * rawValue + 32); // NOLINT(cppcoreguidelines-narrowing-conversions)
    sprintf(buffer, parseFormat, result, unit);
}

void parseAirFlowHz(int rawValue, char *unit) {
    int result = 6.25 * rawValue; // NOLINT(cppcoreguidelines-narrowing-conversions)
    sprintf(buffer, parseFormat, result, unit);
}

void parseTimingAdvance(int rawValue, char *unit) {
    int result = rawValue - 20;
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
    int result = (rawValue - 128) / 5;
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
    int result = convertToCelsius(1.8 * rawValue + 32);
    sprintf(buffer, parseFormat, result, unit);
}

void parseInjPulse(int rawValue, char *unit) {
    if (rawValue != 0) {
        float result = rawValue / 1000; // NOLINT(bugprone-integer-division)
        char tmpInt1 = result; // NOLINT(cppcoreguidelines-narrowing-conversions)
        float tmpFrac = result - tmpInt1;
        char tmpInt2 = trunc(tmpFrac * 100);
        sprintf(buffer, "%2d.%-2d%-3s", tmpInt1, tmpInt2, unit);
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
        case P_COOLING_TEMP:
            parseCoolant(data, unit);
            break;
        case P_INJ_PULSE:
            parseInjPulse(data, unit);
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
            parseEgrTEmp(data, unit);
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
