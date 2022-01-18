#pragma once

#include <Arduino.h>

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

struct request
{
    int addr;
    int parser;
    char name[14];
    char unit[5];
};

enum requests_addr
{
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
char *parseData(int &data, request * requestData);