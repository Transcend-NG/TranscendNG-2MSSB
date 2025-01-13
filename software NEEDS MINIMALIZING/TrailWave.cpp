#include "TrailWave.h"

void TrailRadio::init(U8G2_OLED* display) {
  u8g2 = display;
  mode = 0;
  period = 100;
  locked = false;
  interfreq = IF;
  cal = XT_CAL_F;
  freq = FREQ_MIN;
  vfo_is_A = true;
  interfreqold = 0;
  freqA = FREQ_MIN;
  freqB = FREQ_MIN;
  freqAdjustStep = 2;
  step = 2;
  rit_enabled = false;
  pttButtonState = false;
  setstep(false);
  initPrefs();
  memPresets();
  displaySplashScreen();
  delay(2000);
  initialized = true;
}

void TrailRadio::update() {
  if (freqold != freq || rit_enabled) {
    tunegen();
    freqold = freq;
    displayUpdate();
  }

  if (interfreqold != interfreq) {
    tunegen();
    interfreqold = interfreq;
    displayUpdate();
  }

  if (xo != x) {
    xo = x;
    displayUpdate();
  }

  if (rit_changed) {
    displayUpdate();
    rit_changed = false;
  }

// Check if the SAVE_SETTINGS_PIN is LOW or POWER_DETECTION_PIN is LOW
if (digitalRead(SAVE_SETTINGS_PIN) == LOW || digitalRead(POWER_DETECTION_PIN) == LOW) {
    prefsSave();
    Serial.println("Save button pressed.");
}

// Check if the SAVE_SETTINGS_PIN is LOW or POWER_DETECTION_PIN is LOW
if (digitalRead(FACTORY_RESET_PIN) == LOW) {
    prefsWipe();
    Serial.println("Factory Wipe button pressed.");
}

  //prefsSave();
  handlePTT();
  sgnalread();
}

void TrailRadio::changeMode() {
    mode = (mode + 1) % 4;
    displayUpdate();
}

void TrailRadio::sgnalread() {
  smval = analogRead(ADC_PIN);
  x = map(smval, 0, S_GAIN, 1, 14);
  if (x > 14) x = 14;
}

void TrailRadio::tunegen() {
  long tone_offset = 0; 

  if (mode == 0) tone_offset = 1500;  // USB mode: +1.5 kHz.
  if (mode == 1) tone_offset = -1500; // LSB mode: -1.5 kHz.
  if (mode == 2) tone_offset = 700;   // CW mode: +700 Hz for CW tone.
  if (mode == 3) tone_offset = 1500;  // DIG mode: +1.5 kHz.

  if (!pttButtonState) {
    if (rit_enabled && !split_mode) {
      //si5351.set_freq((freq + rit_offset + tone_offset + (interfreq * 1000ULL)) * 100ULL, SI5351_CLK0);
    } else {
      //si5351.set_freq((freq + tone_offset + (interfreq * 1000ULL)) * 100ULL, SI5351_CLK0);
    }
  } else {  // Transmit mode
    if (split_mode) {
      //si5351.set_freq((freqB + tone_offset + (interfreq * 1000ULL)) * 100ULL, SI5351_CLK0);
    } else {
      //si5351.set_freq((freqA + tone_offset + (interfreq * 1000ULL)) * 100ULL, SI5351_CLK0);
    }
  }
}

  void TrailRadio::handlePTT() {
  // Check if the RX_TX_PIN is LOW on the pcf8575
   if (digitalRead(RX_TX_PIN) == LOW) {
    pttButtonState = 1;
    digitalWrite(RX_TX_LED_PIN, HIGH);
    digitalWrite(TXRX_FILTER_RELAY_PIN, HIGH);
    digitalWrite(TXRX_RELAY_PIN, HIGH);

    // Handle split mode for VFOs
    if (split_mode && vfo_is_A) {
      swapVFOs();
    }

    // If push-to-talk is not active, handle frequency offset adjustments
    if (!pushToTalk) {
      freq += (rit_offset > 0) ? -rit_offset : rit_offset;
      pushToTalk = true;
      displayUpdate();
    }
  } else {
    // RX_TX_PIN is HIGH, handle release of PTT
    pttButtonState = 0;
    digitalWrite(RX_TX_LED_PIN, LOW);
    digitalWrite(TXRX_FILTER_RELAY_PIN, LOW);
    digitalWrite(TXRX_RELAY_PIN, LOW);

    // Swap VFOs back if needed in split mode
    if (split_mode && !vfo_is_A) {
      swapVFOs();
    }

    // Reset push-to-talk and adjust frequency if RIT is enabled
    if (pushToTalk) {
      if (rit_enabled) {
        freq += rit_offset;
      }
      displayUpdate();
      pushToTalk = false;
    }
  }
}
