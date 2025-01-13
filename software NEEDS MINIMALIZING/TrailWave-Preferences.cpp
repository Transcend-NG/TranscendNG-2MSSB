#include "TrailWave.h"

void TrailRadio::initPrefs() {
    prefs.begin("radioPrefs", false);
    if (!prefs.isKey("nvsInit")) {
        prefsSave();
        prefs.putBool("nvsInit", true);
    } else {
        prefsLoad();
    }
}

void TrailRadio::prefsLoad() {
    mode = prefs.getInt("mode");
    step = prefs.getULong64("step");
    freqA = prefs.getULong64("freqA");
    freqB = prefs.getULong64("freqB");
    memSelection = prefs.getInt("memSelection");
    freqAdjustStep = prefs.getInt("freqAdjustStep");
    freq = vfo_is_A ? freqA : freqB;
    Serial.print("Loaded mode: ");
    Serial.println(mode);
    Serial.print("Loaded step: ");
    Serial.println(step);
    Serial.print("Loaded freqA: ");
    Serial.println(freqA);
    Serial.print("Loaded freqB: ");
    Serial.println(freqB);
    displayUpdate();
}

void TrailRadio::prefsSave() {
    freqA = vfo_is_A ? freq : freqA;
    freqB = !vfo_is_A ? freq : freqB;
    prefs.putInt("mode", mode);
    prefs.putULong64("step", step);
    prefs.putULong64("freqA", freqA);
    prefs.putULong64("freqB", freqB);
    prefs.putInt("memSelection", memSelection);
    prefs.putInt("freqAdjustStep", freqAdjustStep);
    Serial.print("Saved mode: ");
    Serial.println(mode);
    Serial.print("Saved step: ");
    Serial.println(step);
    Serial.print("Saved freqA: ");
    Serial.println(freqA);
    Serial.print("Saved freqB: ");
    Serial.println(freqB);
}

void TrailRadio::prefsWipe() {
   prefs.clear(); 
}