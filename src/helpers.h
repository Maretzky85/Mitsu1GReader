#include <Arduino.h>

struct request
{
    int addr;
    int parser;
    String name;
    String unit;
};

#define SWITCHES 2
#define BATT_VOLTAGE 20
#define ACC_ENRICH 29
#define COOLANT_TEMP 7
#define ENGINE_SPEED 33
#define FUEL_TRIM_LOW 12
#define FUEL_TRIM_MID 13
#define FUEL_TRIM_HIGH 14
#define INJECTOR_PULSE 41
#define OXYGEN_FEEDBACK_TRIM 14
#define OXYGEN_SENSOR 19
#define TPS 23
#define AIR_FLOW_HZ 26
#define AIR_TEMP 58
#define AIR_VOLUME 44
#define BARO_SENSOR 21
#define ISC_STEPS 22
#define KNOCK_SUM 38
#define TIMING_ADVANCE 6
#define EGR_TEMP 18

#define REQUESTS_SIZE 20

#define RAW 0
#define P_12V 1
#define MIDDLE100 2
#define ZERO_ONE 3
#define P_5V 4

request requests[] = {
    {SWITCHES, RAW, "SWITCHES", "Unknown"},
    {BATT_VOLTAGE, P_12V, "Battery", "V"},
    {ACC_ENRICH, RAW, "Acceleration enrichment", "%"},
    {COOLANT_TEMP, RAW, "Coolant Temperature", "C"},
    {ENGINE_SPEED, RAW, "Engine Speed", "RPM"},
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
    default:
        return String(data);
        break;
    }
    return "1";
};

bool sendAndRemoveEcho(int command) {
    Serial.write(command);
    delay(10);
    if (Serial.available() == 0)
    {
        return false;
    }
    
    int echo = Serial.read();
    return echo == command;
}