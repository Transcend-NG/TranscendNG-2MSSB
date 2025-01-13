#include <Wire.h>
#include <Arduino.h>

class PCF8575Debounce {
  private:
    uint8_t address;
    uint16_t readPCF();
    uint64_t pressTimes[16];
    uint8_t buttonStates[16];
  public:
    uint8_t isLow(uint8_t pin);
    uint8_t readPin(uint8_t pin);
    void begin(uint8_t address, uint8_t intPin);
};