#include "TrailWave.h"

void TrailRadio::incPreset()  {
  if (memMode) {
    memSelection++;
    if (memSelection > 10) memSelection = 1;
    memPresets();
    displayUpdate();
  }
}

void TrailRadio::decPreset()  {
  if (memMode) {
    memSelection--;
    if (memSelection < 1) memSelection = 10;
    memPresets();
    displayUpdate();
  }
}

void TrailRadio::memPresets() {
  if (memMode) {
    switch (memSelection)  {
      case 1: freq = 144000000; tunegen();  break;  // 2M Band.
      case 2: freq = 144200000; break;  // CALL frequency.
      case 3: freq = 144250000; break;  // CHAT frequency.
      case 4: freq = 144150000; break;  // FT4 frequency.
      case 5: freq = 144174000; break;  // FT8 frequency.
      case 6: freq = 144178000; break;  // JS8 frequency.
      case 7: freq = 144120000; break;  // JT65 frequency.
      case 8: freq = 144138000; break;  // PSK frequency.
      case 9: freq = 144600000; break;  // RTTY frequency.
      case 10: freq = 144489000; break; // WSPR frequency.
    }

  }
}

void TrailRadio::enableMemMode() {
  memMode = true;
  memPresets();
  displayUpdate();
}

void TrailRadio::disableMemMode() {
  memMode = false;
  freq = vfo_is_A ? freqA : freqB;
  displayUpdate();
}
