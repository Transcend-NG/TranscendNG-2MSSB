//#include <Wire.h>
#include "TrailWave.h"
#include <PCF8575Debounce.h>

TrailRadio trailRadio;

PCF8575Debounce pcf;

//Display setup
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

//si5351
Si5351 si5351;

void getInput() {
  if (pcf.readPin(FREQ_DOWN_PIN)) {
    trailRadio.set_frequency(-1);
  }

  if (pcf.readPin(FREQ_UP_PIN)) {
    trailRadio.set_frequency(1); 
  }

  /*
  if (pcf.readPin(ID_TIMER_PIN)) {
    trailRadio.id_timer(1); 
  }

  if (pcf.readPin(ID_TIMER_RESET_PIN)) {
    trailRadio.id_timer_reset(1); 
  }
  */

  if (pcf.readPin(MEM_DOWN_PIN)) {
    trailRadio.decPreset();
  }

  if (pcf.readPin(MEM_UP_PIN)) {
    trailRadio.incPreset();
  }

  if (pcf.readPin(MODE_PIN)) {
    trailRadio.changeMode();
  }

  if (pcf.readPin(RIT_DOWN_PIN)) {
    trailRadio.adjustRIT(-1);
   }

  if (pcf.readPin(RIT_UP_PIN)) {
    trailRadio.adjustRIT(1);
  }

 if (pcf.readPin(STEP_DOWN_PIN)) {
    trailRadio.setstep(true);
  }

  if (pcf.readPin(STEP_UP_PIN)) {
    trailRadio.setstep(false);
  }

  if (pcf.readPin(RIT_PIN) && !trailRadio.split_mode) {
    trailRadio.toggleRit();
  }

  if (pcf.readPin(SPLIT_PIN)) {
    trailRadio.toggleSplitMode();
  }

  if (pcf.readPin(VFO_AB_PIN)) {
    trailRadio.swapVFOs();
  }

  if (pcf.readPin(VFO_A_TO_B_PIN)) {
    trailRadio.copyVFOAtoVFOB();
  }

  if (pcf.readPin(VFO_MEM_MODE_PIN)) {
    if (trailRadio.memMode) {
      trailRadio.disableMemMode();
    } else {
      trailRadio.enableMemMode();
    }
  }
}

/*
void configSI5351() {
  si5351.init(SI5351_CRYSTAL_LOAD_8PF,  0, 0);  // Initialize the SI5351 (commented out).
  si5351.set_correction(cal, SI5351_PLL_INPUT_XO);  // Set frequency correction for SI5351 (commented out).
  si5351.drive_strength(SI5351_CLK0,  SI5351_DRIVE_8MA);  // Set drive strength for SI5351 clock 0 (commented out).
  si5351.output_enable(SI5351_CLK0, 1);  // Enable SI5351 clock 0 (commented out).
  si5351.output_enable(SI5351_CLK1, 0);  // Disable SI5351 clock 1 (commented out).
  si5351.output_enable(SI5351_CLK2,  0);  // Disable SI5351 clock 2 (commented out).
}
*/

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);

  // Test I2C initialization
  //Serial.println("I2C initialized with custom SDA and SCL pins.");
  
  pinMode(FACTORY_RESET_PIN, INPUT_PULLUP);
  pinMode(POWER_DETECTION_PIN, INPUT_PULLUP);
  pinMode(RX_TX_LED_PIN, OUTPUT);
  pinMode(RX_TX_PIN, INPUT_PULLUP);
  pinMode(SAVE_SETTINGS_PIN, INPUT_PULLUP);
  pinMode(TXRX_RELAY_PIN, OUTPUT);
  pinMode(TXRX_FILTER_RELAY_PIN, OUTPUT);

  pcf.begin(0x20, 7);
  u8g2.begin();
  u8g2.clear();
  u8g2.clearBuffer();
  trailRadio.init(&si5351, &u8g2);
}

void loop() {
  delay(10);
  getInput();
  trailRadio.update();
}
