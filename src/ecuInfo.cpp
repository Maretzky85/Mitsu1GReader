#include <ecuInfo.h>
#include <communication.h>
#include <lcd.h>

// Universal MH6111 ECU fingerprint splash.
//
// Every MH6111 ROM responds to:
//   $FD -> a hardcoded ROM-identity byte
//   $FE -> one byte of the strap-selected word from t_strap3 (4-slot table)
//   $FF -> the other byte of that word
//
// Known ROMs are listed in romTable below with their $FD byte and the 4
// possible strap words. Slot semantics vary by market family (DSM US uses
// Fwd/Awd x Fed/Cal; EDM and JDM ROMs may map the same 4 slots to different
// markets, so we deliberately do NOT hardcode slot-name meanings). The
// splash shows:
//   1) ROM name (or "$XX unknown" if not in the table)
//   2) Raw strap bytes as a 16-bit word
//   3) Which of the 4 slots the strap word matches for the identified ROM
//
// To add a new ROM: dump it, disassemble the SCI dispatch (see
// mh6111_project/reference/ROM_fingerprint_table.md for method), and append
// a row with the $FD byte, ROM code, and 4 strap words from t_strap3.

const char ROM_NAME_E742[] PROGMEM     = "E742";
const char ROM_NAME_E766[] PROGMEM     = "E766";
const char ROM_NAME_E799[] PROGMEM     = "E799";
const char ROM_NAME_E931[] PROGMEM     = "E931";
const char ROM_NAME_E932[] PROGMEM     = "E932";
const char ROM_NAME_E935[] PROGMEM     = "E935";
const char ROM_NAME_E44228B5[] PROGMEM = "E44228B5";

const char PROBING[] PROGMEM = "Probing ECU...";
const char NO_RESPONSE[] PROGMEM = "ECU not responding";
const char ECU_INFO_HEADER[] PROGMEM = "ECU Info";

struct rom_entry {
    uint8_t  id_byte;
    const char *name PROGMEM;
    uint16_t slot[4];
};

// One row per known ROM. Strap slot values are the 16-bit words at t_strap3
// as stored in the ROM (source .word directive = big-endian bytes).
const rom_entry romTable[] PROGMEM = {
    {0x14, ROM_NAME_E935,     {0xE025, 0xE025, 0x1412, 0x201E}},
    {0x36, ROM_NAME_E742,     {0xE022, 0xE023, 0x272F, 0x0348}},
    {0x46, ROM_NAME_E44228B5, {0xE086, 0xE087, 0xE086, 0xE087}},
    {0x4F, ROM_NAME_E766,     {0xE025, 0xE025, 0x0303, 0x1412}},
    {0x61, ROM_NAME_E799,     {0xE020, 0xE021, 0x030B, 0x4001}},
    {0xB5, ROM_NAME_E931,     {0xE022, 0xE023, 0xE022, 0xE023}},
    {0xB7, ROM_NAME_E932,     {0xE022, 0xE023, 0xE022, 0xE023}},
};

const uint8_t ROM_TABLE_SIZE = sizeof(romTable) / sizeof(*romTable);

// Query one obdCode. Returns COMM_ERR on timeout/echo mismatch.
static int probe(int code) {
    return getResponseFromAddr(code);
}

static int findRomByIdByte(uint8_t id) {
    for (uint8_t i = 0; i < ROM_TABLE_SIZE; i++) {
        uint8_t stored = pgm_read_byte(&romTable[i].id_byte);
        if (stored == id) return i;
    }
    return -1;
}

// Given ROM index and both possible byte orderings of the strap word,
// return matching slot 0-3, or -1 if none match.
static int findStrapSlot(int romIdx, uint16_t strap_be, uint16_t strap_le) {
    for (uint8_t s = 0; s < 4; s++) {
        uint16_t val = pgm_read_word(&romTable[romIdx].slot[s]);
        if (val == strap_be || val == strap_le) return s;
    }
    return -1;
}

void showEcuInfo() {
    clearScreen();
    printHeader(ECU_INFO_HEADER);

    char msg[LCD_COLS + 1];
    strcpy_P(msg, PROBING);
    printResult(msg, 0);

    while (Serial.available()) Serial.read();
    delay(50);

    int romId  = probe(0xFD);
    int strapA = probe(0xFE);
    int strapB = probe(0xFF);

    clearScreen();
    printHeader(ECU_INFO_HEADER);

    if (romId == COMM_ERR || strapA == COMM_ERR || strapB == COMM_ERR) {
        strcpy_P(msg, NO_RESPONSE);
        printResult(msg, 0);
        delay(3000);
        return;
    }

    // Line 1: "ROM: <name>" or "ROM: $XX unknown"
    int idx = findRomByIdByte((uint8_t) romId);
    char line[LCD_COLS + 1];
    if (idx >= 0) {
        char name[12];
        strncpy_P(name, (const char *) pgm_read_word(&romTable[idx].name), sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
        snprintf(line, sizeof(line), "ROM: %s ($%02X)", name, romId);
    } else {
        snprintf(line, sizeof(line), "ROM: $%02X unknown", romId);
    }
    printResult(line, 0);

    // Line 2: raw strap word display
    uint16_t strap_be = ((uint16_t) strapA << 8) | (uint16_t) strapB;
    uint16_t strap_le = ((uint16_t) strapB << 8) | (uint16_t) strapA;
    snprintf(line, sizeof(line), "Straps: $%02X%02X", strapA, strapB);
    printResult(line, 1);

    // Line 3: slot match against the identified ROM's table
    if (idx >= 0) {
        int slot = findStrapSlot(idx, strap_be, strap_le);
        if (slot >= 0) {
            snprintf(line, sizeof(line), "Slot: %d of 4", slot);
        } else {
            snprintf(line, sizeof(line), "Slot: no match");
        }
    } else {
        snprintf(line, sizeof(line), "Slot: -- (ROM new)");
    }
    printResult(line, 2);

    delay(3000);
}
