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
    BATT_VOLTAGE = 0x14,
    ACC_ENRICH = 0x1D,
    COOLANT_TEMP = 0x07,
    ENGINE_SPEED = 0x21,
    FUEL_TRIM_LOW = 0x0C,
    FUEL_TRIM_MID = 0x0D,
    FUEL_TRIM_HIGH = 0x0E,
    INJECTOR_PULSE = 0x29,
    OXYGEN_FEEDBACK_TRIM = 0x0F,
    OXYGEN_SENSOR = 0x13,
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

extern request requests[];

char *parseData(int &data, request * requestData);