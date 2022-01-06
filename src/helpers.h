#include <Arduino.h>

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

const int REQUESTS_SIZE PROGMEM = 20;

#define RAW 0
#define P_12V 1
#define MIDDLE100 2
#define ZERO_ONE 3
#define P_5V 4
#define P_RPM 5

request requests[] = {
    {SWITCHES, RAW, "switches", "Unknown"},
    {BATT_VOLTAGE, P_12V, "battery", "V"},
    {ACC_ENRICH, RAW, "acc enrich", "%"},
    {COOLANT_TEMP, RAW, "Coolant temp", "C"},
    {ENGINE_SPEED, P_RPM, "Engine speed", "RPM"},
    {FUEL_TRIM_LOW, MIDDLE100, "Fuel Trim Low", "%"},
    {FUEL_TRIM_MID, MIDDLE100, "Fuel Trim Mid", "%"},
    {FUEL_TRIM_HIGH, MIDDLE100, "Fuel Trim High", "%"},
    {INJECTOR_PULSE, RAW, "Injector Pulse", "ms"},
    {OXYGEN_FEEDBACK_TRIM, MIDDLE100, "Oxygen Feedback Trim", "%"},
    {OXYGEN_SENSOR, ZERO_ONE, "Oxygen Sensor", "V"},
    {TPS, P_5V, "TPS", "%"},
    {AIR_FLOW_HZ, RAW, "Air Flow", "Hz"},
    {AIR_TEMP, RAW, "Air Temperature", "C"},
    {AIR_VOLUME, RAW, "Air Volume", "raw"},
    {BARO_SENSOR, RAW, "Barometric Sensor", "Hpa"},
    {ISC_STEPS, RAW, "ISC steps", "Steps"},
    {KNOCK_SUM, RAW, "Knock Sum", "Knocks"},
    {TIMING_ADVANCE, RAW, "Timing Advance", "deg"},
    {EGR_TEMP, RAW, "Egr Temperature", "C"}
};

float parseToTwelve(int rawValue) {
  return rawValue * 0.075;
};

float parseToFive(int rawValue) {
    return rawValue * 0.05; //TODO
}

int parseMiddleHundred(int rawValue) {
    return rawValue - 128; //TODO
}

float parseZeroOne(int rawValue) {
    return rawValue * 0.01; //TODO
}

int parseRPM(int rawValue) {
    return rawValue * 32;
}

String parseData(int data, int parser) {
    switch (parser)
    {
    case P_12V:
        return String(parseToTwelve(data));
        break;
    case RAW:
        return String(data);
        break;
    case P_5V:
        return String(parseToFive(data));
        break;
    case MIDDLE100:
        return String(parseMiddleHundred(data));
        break;
    case ZERO_ONE:
        return String(parseZeroOne(data));
        break;
    case P_RPM:
        return String(parseRPM(data));
    default:
        return String(data);
        break;
    }
    return "1";
};

bool waitForResponse() {
    long sentTime = millis();
    while (Serial.available() == 0)
    {
        if (millis() - sentTime > 50)
        {
            return false;
        }
    }
    return true;
}

bool sendAndRemoveEcho(int command) {
    Serial.write(command);
    if(!waitForResponse()) {
        return false;
    }
    int echo = Serial.read();
    //echo should be same as sent command, if not, there communication problem.
    return echo == command;
}
