#ifndef TRAIL_WAVE
#define TRAIL_WAVE

#include <U8g2lib.h>
#include <Preferences.h>

#define IF 455
//#define MEM_INIT 2
#define XT_CAL_F 33000
#define S_GAIN 303

// Define GPIO and ADC pins
#define ADC_PIN 1                 // ESP32-C3 pin for ADC input, typically connected to the RF signal for strength measurement.
#define POWER_DETECTION_PIN 5     // ESP32-C3 pin for Detecing power off and saving current settings.
#define FACTORY_RESET_PIN 6       // ESP32-C3 pin for Force Rest of Defaults.
#define RX_TX_PIN 4               // ESP32-C3 pin for controlling TX/RX toggle.(PTT)
#define RX_TX_LED_PIN 0           // ESP32-C3 pin for controlling TX/RX Led Relay toggle.(PTT)
#define SAVE_SETTINGS_PIN 10      // ESP32-C# pin for Saving Current Settings.
#define TXRX_FILTER_RELAY_PIN 3   // ESP#@-C3 pin for controlling TX/RX filter relay (switches between transmit and receive).
#define TXRX_RELAY_PIN 2          // ESP32-C# pin for controlling TX/RX relay (switches between transmit and receive).

// PCF8575 pins
#define FREQ_DOWN_PIN 15          // PCF8575 pin for Freq Down pin 
#define FREQ_UP_PIN 14            // PCF8575 pin for Freq Up pin 
#define MEM_DOWN_PIN 3            // PCF8575 pin for decreasing band selection.
#define MEM_UP_PIN 2              // PCF8575 pin for increasing band selection.
#define MODE_PIN 13               // PCF8575 pin for Encoder button (menu/select).
#define SAVE_PIN 0                // PCF8575 pin for prefs save.
#define LOAD_PIN 1                // PCF8575 pin for prefs load.
#define RIT_PIN 5                 // PCF8575 pin for Receiver Incremental Tuning (RIT) Encable/Disable pin (toggles RIT on/off). (RIT)
#define RIT_DOWN_PIN 7            // PCF8575 pin for Receiver Incremental Tuning (RIT) down button pin.(RIT)
#define RIT_UP_PIN 6              // PCF8575 pin for Receiver Incremental Tuning (RIT) up button pin.(RIT)
#define SPLIT_PIN 9               // PCF8575 pin for Split Mode button (toggles split mode).
#define STEP_DOWN_PIN 11          // PCF8575 pin for decreasing the tuning step.
#define STEP_UP_PIN 10            // PCF8575 pin for increasing the tuning step.
#define VFO_AB_PIN 4              // PCF8575 pin for toggling between VFO A and VFO B.
#define VFO_A_TO_B_PIN 8          // PCF8575 pin for COPY VFO A TO VFO B.
#define VFO_MEM_MODE_PIN 12       // PCF8575 pin for Swapping from VFO to MEM or MEM to VFO mode.

// U8G2 ssd1306/sd1109 Displey
#define U8G2_OLED U8G2_SSD1306_128X64_NONAME_F_HW_I2C

const unsigned long FREQ_MIN = 144000000;
const unsigned long FREQ_MAX = 144650000;

class TrailRadio {
  private:
    U8G2_OLED* u8g2;
    Preferences prefs;

    unsigned int smval;
    unsigned int period;

    int lastClk;
    int lastRITClk;

    bool locked;
    bool vfoMode;
    bool vfo_is_A;
    bool pushToTalk;
    bool initialized;
    bool rit_changed;
    bool rit_enabled;
    bool pttButtonState;

    unsigned long step;
    unsigned long freq;
    unsigned long freqA;
    unsigned long freqB;
    unsigned long freqold;
    
    long cal;
    long rit_offset;
    long interfreq;
    long interfreqold;
    long encoderValue;
    
    uint8_t x;
    uint8_t xo;
    uint8_t memSelection;
    uint8_t freqAdjustStep;

    void layout();
    void tunegen();
    void memlist();
    void sgnalread();
    void handlePTT();
    void memPresets();
    void drawbargraph();
    void displayUpdate();
    void displaySplashScreen();

  public:
    int mode;
    bool memMode;
    bool split_mode;

    void update();
    void swapVFOs();
    void toggleRit();
    void decPreset();
    void incPreset();
    void changeMode();
    void prefsWipe();    
    void prefsSave();
    void prefsLoad();
    void initPrefs();
    void enableMemMode();
    void disableMemMode();
    void copyVFOAtoVFOB();
    void setstep(bool dir);
    void toggleSplitMode();
    void adjustRIT(short dir);
    void set_frequency(short dir);
    void init(U8G2_OLED* display);
};

#endif
