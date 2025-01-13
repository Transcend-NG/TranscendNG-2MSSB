#include "PCF8575Debounce.h"

volatile bool pcfInt[16];

void pcfISR() {
  for (int i = 0; i < 16; ++i) {
    pcfInt[i] = true;
  }
}

void PCF8575Debounce::begin(uint8_t address, uint8_t intPin) {
  Wire.begin();
  this->address = address;
  for (int i = 0; i < 16; ++i) {
    buttonStates[i] = 1;
  }
  pinMode(intPin, INPUT_PULLUP); // Added this...
  attachInterrupt(digitalPinToInterrupt(intPin), pcfISR, CHANGE);
}

uint16_t PCF8575Debounce::readPCF() {
  Wire.requestFrom(address, static_cast<size_t>(2));
  uint16_t data = Wire.read();
  data |= (Wire.read() << 8);
  return data;
}

uint8_t PCF8575Debounce::isLow(uint8_t pin) {
  return !((readPCF() >> pin) & 1);
}

uint8_t PCF8575Debounce::readPin(uint8_t pin) {
  bool result = false;
  uint64_t now = millis();
  bool debounce = (now - pressTimes[pin]) > 50;

  if (pcfInt[pin] && debounce) {
    int state = ((readPCF() >> pin) & 1);
    
    if (!state && buttonStates[pin]) {
      result = true;
    }

    pcfInt[pin] = false;
    pressTimes[pin] = now;
    buttonStates[pin] = state;
  }

  return result;
}