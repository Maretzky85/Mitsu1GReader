#pragma once

#include <Arduino.h>

// MH6111 (Galant VR4 / DSM 4G63) diagnostic port protocol:
// Byte-in / byte-out request-response on the SCI at 1953 baud, 8N1, echoed.
//
// obdCode range | Meaning
//   $00..$3D    | Sensor lookup via obdTable at ROM $D000 -> returns value at RAM[obdTable[code]]
//   $3E..$3F    | Clamped to $3D
//   $40..$F0    | Direct RAM peek: returns value at RAM[code]  (except $CA = clear fault codes)
//   $F1..$F6    | Actuator on commands (only when engine not rotating)
//   $F7..$FC    | Injector kill commands (only when speed near 0)
//   $FD         | Serial link test: returns $B5 for E931, $B7 for E932  (ROM ID probe)
//   $FE..$FF    | Configuration strap data (t_strap3)
//
// obdTable lives at CPU $D000 (E931 file offset $5000), 62 bytes, one RAM-pointer per code.
// Source: mh6111_project/source/standard_E932_E931_source.asm line 1575.

enum parsers {
    P_RAW,
    P_12V,
    P_FEEDBACK_TRIM,
    P_ZERO_ONE,
    P_PERCENT,
    P_RPM,
    P_TIMING_ADVANCE,
    P_COOLANT_TEMP,
    P_INJ_PULSE_16,
    P_AIR_FLOW_HZ,
    P_AIR_TEMP,
    P_BARO,
    P_EGR_TEMP,
    P_ISC,
    P_HEX,
    P_CL_FLAGS
};

struct request {
    int addr;         // primary obdCode
    int parser;
    const char *name PROGMEM;
    const char *unit PROGMEM;
};

// obdCodes -- verified against MH6111 obdTable at ROM $D000.
// Codes $40..$F0 (except $CA) are direct RAM peek: response = RAM[code].
enum requests_addr {
    // Table lookups ($00..$3D) -- indexed through obdTable
    SWITCHES             = 0x02,  // port3 flags
    TIMING_ADVANCE       = 0x06,  // timingAdv:  (raw - 10) deg BTDC
    COOLANT_TEMP_RAW     = 0x07,  // ectRaw:     ADC, thermistor lookup
    FUEL_TRIM_LOW        = 0x0C,  // ftrim_low:  0.78% per LSB, $80=100%
    FUEL_TRIM_MID        = 0x0D,  // ftrim_mid:  0.78% per LSB, $80=100%
    FUEL_TRIM_HIGH       = 0x0E,  // ftrim_hi:   0.78% per LSB, $80=100%
    OXYGEN_FEEDBACK_TRIM = 0x0F,  // o2Fbk high: 0.78% per LSB, $80=100%
    COOLANT_TEMP         = 0x10,  // ectFiltered: same curve as ectRaw, validated + rate-limited
    AIR_TEMP             = 0x11,  // iatChecked:  validated iatRaw
    EGR_TEMP             = 0x12,  // egrtRaw:    -2.7*x + 597.7 degF
    OXYGEN_SENSOR        = 0x13,  // o2Raw:      0.0195 V/LSB
    BATT_VOLTAGE         = 0x14,  // battRaw:    0.0733 V/LSB
    BARO_SENSOR          = 0x15,  // baroRaw:    0.486 kPa/LSB
    ISC_STEPS            = 0x16,  // iscStepCurr: 0-120 raw steps
    TPS                  = 0x17,  // tpsRaw:     100*x/255 %
    AIR_FLOW_HZ          = 0x1A,  // mafRaw:     6.25 Hz/LSB
    ACC_ENRICH           = 0x1D,  // accEnr:     100*x/255 %  (max $48)
    ENGINE_SPEED         = 0x21,  // rpm31:      31.25 rpm/LSB
    KNOCK_SUM            = 0x26,  // knockSum:   0..255 count
    INJECTOR_PULSE_HI    = 0x29,  // injPw high byte  (paired with $2A for 16-bit us)
    INJECTOR_PULSE_LO    = 0x2A,  // injPw low  byte
    AIR_VOLUME           = 0x2C,  // airCnt0 high byte
    AIR_TEMP_RAW         = 0x3A,  // iatRaw:     ADC, thermistor lookup (unfiltered)

    // Direct RAM peek ($40..$F0) -- returns RAM[code]. No obdTable indirection.
    OCTANE               = 0x52,  // octane:     ignition-timing learning value, decrements on knock, increments when clean
    KNOCK_DECAY_TIMER    = 0x8C,  // T200s_knock: knock attenuation timer (non-zero = ECU pulling timing right now)
    CLOSED_LOOP_FLAGS    = 0xE8,  // closedLpFlags: bit 7 = rich/lean, other bits = loop state
};

extern request requests[];
extern const int MAX_REQUESTS;

char *parseData(int &data, request *requestData);
