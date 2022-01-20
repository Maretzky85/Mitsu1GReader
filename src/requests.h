#pragma once

#include <Arduino.h>

struct request {
    int addr;
    int parser;
    const char *name PROGMEM;
    const char *unit PROGMEM;
};

enum requests_addr {
    SWITCHES = 0x02,
    TIMING_ADVANCE = 0x06,
    COOLANT_TEMP = 0x07,
    FUEL_TRIM_LOW = 0x0C,
    FUEL_TRIM_MID = 0x0D,
    FUEL_TRIM_HIGH = 0x0E,
    OXYGEN_FEEDBACK_TRIM = 0x0F,
    EGR_TEMP = 0x12,
    OXYGEN_SENSOR = 0x13,
    BATT_VOLTAGE = 0x14,
    BARO_SENSOR = 0x15,
    ISC_STEPS = 0x16,
    TPS = 0x17,
    AIR_FLOW_HZ = 0x1A,
    ACC_ENRICH = 0x1D,
    ENGINE_SPEED = 0x21,
    KNOCK_SUM = 0x26,
    INJECTOR_PULSE = 0x29,
    AIR_VOLUME = 0x2C,
    AIR_TEMP = 0x3A,
};

extern request requests[];
extern const int MAX_REQUESTS;

char *parseData(int &data, request *requestData);