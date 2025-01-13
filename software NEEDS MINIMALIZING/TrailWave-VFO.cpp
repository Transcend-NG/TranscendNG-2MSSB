#include "TrailWave.h"

void TrailRadio::set_frequency(short dir) {
  if (!pttButtonState && !memMode) {
    if (rit_offset > 0) {
      freq = freq - rit_offset;
    } else {
      freq = freq + rit_offset;
    }
    freq = (dir == 1) ? freq + step : freq - step;
    if (rit_enabled) {
      freq = freq + rit_offset;
    }
    freq = constrain(freq, FREQ_MIN, FREQ_MAX);
    displayUpdate();
  }
}

void TrailRadio::adjustRIT(short dir) {
  if (rit_enabled && !pttButtonState && !memMode) {
    if (rit_offset > 0) {
      freq = freq - rit_offset;
    } else {
      freq = freq + rit_offset;
    }
    rit_offset += dir * 100;
    rit_offset = constrain(rit_offset, -9900, 9900);
    freq = freq + rit_offset;
    freq = constrain(freq, FREQ_MIN, FREQ_MAX);
    rit_changed = true;
    displayUpdate();
  }
}

void TrailRadio::toggleRit() {
  if (!memMode && !memMode) {
    rit_enabled = !rit_enabled;
    if (!rit_enabled) {
      if (rit_offset > 0) {
        freq = freq - rit_offset;
      } else {
        freq = freq + rit_offset;
      }
      rit_offset = 0;
    }
    displayUpdate();
  }
}

void TrailRadio::toggleSplitMode() {
  if (!split_mode && !memMode) {
    rit_enabled = false;
    if (rit_offset > 0) {
      freq = freq - rit_offset;
    } else {
      freq = freq + rit_offset;
    }
    rit_offset = 0;
    split_mode = true;
  } else {
    split_mode = false;
  }
  displayUpdate();
}

void TrailRadio::swapVFOs() {
  if (!memMode) {
    if (rit_offset > 0) {
      freq = freq - rit_offset;  
    } else {
      freq = freq + rit_offset;  
    }
    if (vfo_is_A) {
      freqA = freq;
      freq = freqB;
      vfo_is_A = false;
    } else {
      freqB = freq;
      freq = freqA;
      vfo_is_A = true;
    }
    if (rit_enabled && !pttButtonState) {
      freq = freq + rit_offset;
    }
    displayUpdate();
  }
}

void TrailRadio::copyVFOAtoVFOB() {
  if (!memMode) {
    if (vfo_is_A) {
      freqA = freq;
      freqB = freqA;
    } else {
      freqB = freq;
      freqA = freqB;
    }
    displayUpdate();
  }
}

void TrailRadio::setstep(bool down) {
  if (!memMode) {
    if (down) {
      freqAdjustStep = freqAdjustStep > 1 ? freqAdjustStep - 1 : 6;
    } else {
      freqAdjustStep = freqAdjustStep < 6 ? freqAdjustStep + 1 : 1;
    }

    switch (freqAdjustStep) {
      case 1: step = 10; break;
      case 2: step = 100; break;
      case 3: step = 1000; break;
      case 4: step = 5000; break;
      case 5: step = 10000; break;
      case 6: step = 100000; break;
    }
    displayUpdate();
  }
}