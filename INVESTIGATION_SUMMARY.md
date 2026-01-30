# Investigation Summary: LCD Garbage and Freezing Issue

**Date:** 2026-01-30
**Issue:** Device works fine on bench, but after a few minutes in car shows LCD garbage, repeating patterns, and eventually freezes.

---

## Root Cause Analysis

### Primary Issue: Arduino String Memory Leak (CRITICAL)

**Location:** `src/lcd.cpp` lines 34-39

**Problem:**
```cpp
// BAD CODE (causing heap fragmentation):
String lastResultPrinted = "";
String lastResults[] = {"", "", ""};
```

**Why it fails:**
- Arduino `String` class uses dynamic memory allocation
- Every comparison/assignment allocates/frees heap memory
- Main loop runs thousands of times per minute
- ATmega328P has only 2KB RAM - heap fragments over time
- After ~5-10 minutes: heap/stack collision → corruption → freeze

**Evidence:**
- ✅ Time-dependent failure (works initially, degrades over minutes)
- ✅ "Garbage" and "repeating patterns" (classic heap corruption symptoms)
- ✅ Works on bench (shorter test duration)
- ✅ Worse in car (voltage fluctuations trigger more redraws → faster fragmentation)

**Fix Applied:**
- Replaced `String` objects with fixed-size `char[]` arrays
- Used `strcmp()` for comparisons (no allocation)
- Used `strncpy()` for copying (no allocation)
- Result: Zero dynamic allocation in display code

---

### Secondary Issue: Inadequate Power Supply Filtering (CONTRIBUTING FACTOR)

**Location:** Hardware schematic - Power supply section (U3: L7805ABV)

**Problems Found:**

1. **Input capacitor C1 = 10µF (TOO SMALL)**
   - Automotive environment needs 470µF - 1000µF
   - 10µF cannot filter:
     - Alternator ripple (100-400Hz)
     - Relay/solenoid transients (microsecond spikes)
     - Starter motor voltage sag (8-10V for 1-2 seconds)

2. **No large bulk capacitor**
   - Missing 1000µF electrolytic on 12V input
   - Critical for automotive applications

3. **Linear regulator heating**
   - 7805 dissipates (Vin - 5V) × current as heat
   - At 14.4V input, 200mA load: 1.88W heat
   - May cause voltage sag under thermal stress

4. **No TVS diode**
   - Car electrical system has 50V+ transients
   - No protection for voltage spikes

**Impact:**
- Voltage fluctuations → unstable 5V rail
- I2C communication errors to LCD
- More frequent display redraws → accelerates String memory leak
- Possible brown-out without reset (voltage dips to 3.5-4.5V)

---

## What is "Brown-out Without Reset"?

**Brown-out** = voltage drops below stable operation, but above reset threshold

**ATmega328P behavior:**
```
5.0V   → Normal operation
4.5V   → Still running, but glitchy
4.0V   → Random bit flips, I2C errors, corruption
3.5V   → Zombie state (running but unreliable)
2.7V   → Brown-out detector triggers RESET (clean restart)
```

**Problem:** If voltage drops to 3.5-4.5V, CPU keeps running but:
- RAM corruption
- Incorrect calculations
- Peripheral malfunction (I2C sends garbage to LCD)
- No reset triggered → corrupted state persists

**Car scenarios:**
- Headlights ON: 13.2V → 12.8V (minor dip)
- AC compressor kicks in: 13V → 11.5V (regulator sags momentarily)
- Starter cranking: 13V → 8V (massive dip, but short duration)

---

## Changes Made

### Code Changes

#### 1. `src/lcd.cpp` - Fixed String Memory Leak

**Lines 34-39:** Replaced String objects with char arrays
```cpp
// Before:
String lastResultPrinted = "";
String lastResults[] = {"", "", ""};

// After:
char lastResultPrinted[LCD_COLS] = "";
char lastResults[3][LCD_COLS] = {"", "", ""};
```

**Lines 88-90:** Fixed comparison and assignment
```cpp
// Before:
if (lastResultPrinted != result || force || redraw) {
    lastResultPrinted = result;

// After:
if (strcmp(lastResultPrinted, result) != 0 || force || redraw) {
    strncpy(lastResultPrinted, result, sizeof(lastResultPrinted) - 1);
    lastResultPrinted[sizeof(lastResultPrinted) - 1] = '\0';
```

**Lines 108-110:** Fixed row-based results
```cpp
// Before:
if (lastResults[row] != result || redraw) {
    lastResults[row] = result;

// After:
if (strcmp(lastResults[row], result) != 0 || redraw) {
    strncpy(lastResults[row], result, sizeof(lastResults[row]) - 1);
    lastResults[row][sizeof(lastResults[row]) - 1] = '\0';
```

#### 2. `src/main.cpp` - Added Memory Monitoring

**Added `freeMemory()` function** for debugging:
```cpp
extern unsigned int __heap_start;
extern void *__brkval;

int freeMemory() {
    int free_memory;
    if ((int)__brkval == 0)
        free_memory = ((int)&free_memory) - ((int)&__heap_start);
    else
        free_memory = ((int)&free_memory) - ((int)__brkval);
    return free_memory;
}
```

**Added commented-out memory monitoring in loop():**
- Prints free RAM every 5 seconds
- Requires disconnecting ECU and changing to 9600 baud
- Useful for validating the fix

### Documentation Created

1. **CLAUDE.md** - Repository guidance for Claude Code
2. **MEMORY_EXPLANATION.md** - Detailed explanation of String vs char[] for learning
3. **INVESTIGATION_SUMMARY.md** - This document

---

## Hardware Recommendations (NOT YET IMPLEMENTED)

### Critical: Add Input Bulk Capacitor

**Action:** Solder 1000µF 25V electrolytic capacitor parallel to C1
- Positive lead to VCC1 (12V input)
- Negative lead to GND
- **Mind polarity!**

**Expected improvement:**
- Smooth voltage dips during electrical load changes
- Filter alternator noise
- Prevent brown-out conditions
- Stabilize 7805 regulator operation

### Recommended: Add Output Capacitor

**Action:** Solder 470µF 10V electrolytic capacitor on 5VVCC rail
- Between 5VVCC and GND near Arduino
- Holds voltage stable if regulator sags briefly

### Optional: Add Ceramic Bypass

**Action:** Solder 100nF ceramic capacitor parallel to C1
- Handles high-frequency spikes (electrolytics are slow)
- Improves EMI rejection

### Optional: Add TVS Diode

**Action:** Add P6KE18A TVS diode across 12V input
- Clamps voltage spikes above 18V
- Protects against automotive transients (load dump, alternator spikes)

---

## Testing Plan

### Phase 1: Verify Code Fix (NOW)

1. Upload fixed code to Arduino
2. Test in car for 10+ minutes
3. **Expected:** No garbage, no freezing

**Success criteria:** Device runs indefinitely without memory issues

### Phase 2: Hardware Validation (IF STILL ISSUES)

1. Solder 1000µF capacitor to C1 input
2. Test in car during electrical load changes:
   - Turn headlights ON/OFF
   - AC compressor cycling
   - Blower fan speed changes
3. Monitor LCD for stability

**Success criteria:** No display corruption during voltage transients

### Phase 3: Memory Monitoring (OPTIONAL - FOR LEARNING)

1. Disconnect ECU
2. Change `Serial.begin(2000)` to `Serial.begin(9600)`
3. Uncomment memory monitoring in `loop()`
4. Upload and open Serial Monitor
5. Observe free RAM over time

**Expected result:** RAM usage remains stable (no decrease over time)

---

## Lessons Learned

### For Embedded C++ Development

1. **Never use Arduino `String` on AVR (ATmega328P)**
   - Use `char[]` arrays instead
   - Use `strcmp()` and `strncpy()` for string operations
   - Store constants in PROGMEM

2. **Monitor free memory during development**
   - ATmega328P has only 2KB RAM
   - Heap fragmentation is silent and deadly
   - Use `freeMemory()` function to track usage

3. **Fixed allocation is always better than dynamic**
   - Allocate at compile time, not runtime
   - Predictable memory usage
   - No fragmentation

### For Automotive Electronics

1. **Power supply filtering is critical**
   - Large bulk capacitors (470µF - 1000µF) on input
   - Smaller capacitors (100nF) for high-frequency noise
   - Output reservoir capacitors to hold voltage during sags

2. **Automotive electrical is hostile**
   - Voltage range: 8V - 16V (nominal 12-14V)
   - Transients up to 50V+
   - Brown-out conditions common during load changes

3. **Test under real-world conditions**
   - Bench testing with clean power supply insufficient
   - Must test with electrical loads (headlights, AC, starter)
   - Long-duration tests (10+ minutes minimum)

4. **Brown-out detection**
   - Set BOD fuses appropriately (4.3V recommended)
   - Better to reset than continue in corrupted state
   - Capacitors prevent brown-out, BOD is safety net

---

## Status: READY FOR TESTING

**Code fixes:** ✅ Complete
**Documentation:** ✅ Complete
**Hardware recommendations:** 📝 Documented (not yet implemented)

**Next action:** Upload fixed firmware and test in car for 10+ minutes.

If issue persists, proceed with hardware modifications (add 1000µF capacitor to C1).

---

## Files Modified

- `src/lcd.cpp` - Fixed String memory leak
- `src/main.cpp` - Added memory monitoring function
- `CLAUDE.md` - Added repository guidance
- `MEMORY_EXPLANATION.md` - Educational documentation
- `INVESTIGATION_SUMMARY.md` - This file

**Branch:** `fix/memory-leak-and-power-analysis`
