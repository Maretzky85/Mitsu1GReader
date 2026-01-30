# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Mitsu1GReader is an Arduino-based diagnostic tool for 1988-1994 Mitsubishi ECUs. It's a standalone bench testing device that communicates with the ECU via serial protocol to read sensor data, diagnostic trouble codes (DTCs), and execute commands. The device uses a 20x4 LCD display and two navigation buttons for user interaction.

## Build and Upload Commands

This project uses PlatformIO for Arduino development:

```bash
# Build the project
pio run

# Upload to Arduino
pio run --target upload

# Clean build artifacts
pio run --target clean

# Build and upload in one command
pio run --target upload
```

Target board: Arduino Uno (ATmega328P)
Framework: Arduino
Serial baud rate: 2000 (non-standard baud rate specific to Mitsubishi MUT protocol)

## Architecture

### State Machine Design

The main loop implements a state-based UI with four operating modes (main.cpp:10-14):
- **DATA_READER**: Real-time sensor data display with scrolling through sensor parameters
- **SW_READER**: Switch status monitoring
- **DTC_READER**: Diagnostic trouble code reading and clearing
- **COMMANDS**: Command execution interface

States are switched by long-pressing navigation buttons. Within each state, short button presses navigate through data or options.

### Communication Layer

The ECU communication protocol (communication.cpp) is synchronous request-response:
1. Send command byte to ECU
2. Wait for echo (validates communication)
3. Read response byte
4. Track responses-per-second (RPS) for connection health monitoring

The protocol uses 50ms timeout for responses. Communication status is displayed on LCD:
- "OK": Normal operation
- "!C": Communication error (no echo)
- "!R": Response error (no data)

### Request System

Sensor data requests are defined in a declarative table (requests.cpp:55-75):
- Each request has: address (ECU memory location), parser type, display name, and unit
- 19 different sensor parameters are supported (coolant temp, RPM, TPS, air flow, etc.)
- Raw byte responses are converted to engineering units via specialized parsers

Parser types handle different data formats:
- `P_RAW`: Display raw byte value
- `P_12V`: Convert to 0-12V range (battery voltage)
- `P_PERCENT`: Scale 0-255 to 0-100%
- `P_RPM`: Convert to engine RPM (multiply by 31.25)
- `P_TIMING_ADVANCE`: Ignition timing in degrees (subtract 10)
- `P_COOLING_TEMP`: Coolant temperature (-0.792 * raw + 156)
- `P_INJ_PULSE`: Injector pulse width in milliseconds (multiply by 0.256)
- `P_AIR_FLOW_HZ`: Air flow sensor frequency (multiply by 6.25)
- `P_FEEDBACK_TRIM`: Fuel trim percentage ((raw - 128) / 5)
- And others for air temp, barometric pressure, EGR temp, ISC steps

### DTC System

Diagnostic Trouble Codes are stored as a 16-bit bitmask in ECU addresses 0x3B (low byte) and 0x3C (high byte). Each bit represents a specific fault code (dtcReader.cpp:45-62):
- Bit masks range from 0x0001 to 0x8000
- Codes include: oxygen sensor (11), air flow (12), TPS (14), coolant (21), knock sensor (31), injector (41), etc.

DTC clearing requires sending a specific sequence of commands: 0xCA, 0x7E, 0xFA, 0xFC. Each must respond with 0x00 to confirm successful clearing.

### LCD Display Layout

20x4 character LCD (I2C interface):
- Row 0: Header text, communication status (right corner), RPS counter
- Rows 1-3: Data display (varies by mode)
  - Data reader: Shows 3 sensor values with names and parsed values
  - DTC reader: Shows up to 3 error codes with descriptions

Display uses PROGMEM extensively to store strings in flash memory rather than RAM (critical for ATmega328P's 2KB RAM limit).

### Button Handling

Two buttons with debouncing and long-press detection (buttons.h):
- `nextButtonPin` (pin 2): Navigate forward / cycle through data
- `prevButtonPin` (pin 3): Navigate backward / toggle sub-modes
- Long press (>1 second): Switch between main operating modes
- Short press: Context-dependent navigation within current mode

Pull-up resistors are enabled internally (INPUT_PULLUP mode).

## Critical Implementation Details

### Memory Management
- ATmega328P has only 2KB SRAM - use PROGMEM for all constant strings
- Use `strcpy_P()` to read from program memory
- Sprintf buffer is reused (requests.cpp:79) to minimize heap allocations

### Timing Constraints
- 1ms delay between consecutive ECU requests prevents read errors (dataReader.cpp:47)
- 50ms timeout for ECU responses (communication.cpp:9)
- Serial buffer must be cleared after communication errors to prevent stale data

### Fixed-Point Math
Temperature and voltage conversions use integer math with fixed scaling factors to avoid floating-point overhead. When display precision requires decimals, manual integer/fractional splitting is used (e.g., parseToTwelve in requests.cpp:108-114).

### LCD Update Strategy
The `redraw` flag signals when screen should be completely cleared (mode changes). Otherwise, data overwrites existing positions to minimize flicker and maintain responsiveness.

## Key Files

- `src/main.cpp`: Main state machine and loop
- `src/communication.cpp`: ECU serial protocol implementation
- `src/requests.cpp`: Sensor data definitions and parsing logic
- `src/dataReader.cpp`: Real-time data display mode
- `src/dtcReader.cpp`: DTC reading and clearing logic
- `src/lcd.cpp`: Display abstraction layer
- `src/buttons.cpp`: Button input handling
- `src/switches.cpp`: Switch status reading
- `src/commands.cpp`: Command execution mode
- `platformio.ini`: Build configuration
