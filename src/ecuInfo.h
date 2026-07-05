#pragma once

#include <Arduino.h>

// Fingerprints the connected MH6111 ECU by querying:
//   $FD -> ROM identifier byte  (unique per ROM family)
//   $FE -> one byte of the strap-selected word from t_strap3
//   $FF -> the other byte of the same word
// Displays the result as a boot splash for ~3 seconds.
void showEcuInfo();
