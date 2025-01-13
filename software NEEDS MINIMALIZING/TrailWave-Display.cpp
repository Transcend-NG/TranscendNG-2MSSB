#include "TrailWave.h"

void TrailRadio::displaySplashScreen() {
  u8g2->firstPage();
  do {
    u8g2->setFont(u8g2_font_6x12_tf);
    u8g2->setCursor(13, 18);
    u8g2->print("TrailWave SSB 144");
    u8g2->setCursor(5, 40);
    u8g2->print("Transcend Radio v1.0");
  } while (u8g2->nextPage());
}

void TrailRadio::displayUpdate() {
  if (initialized) {
    unsigned int m = freq / 1000000;
    unsigned int k = (freq % 1000000) / 1000;
    unsigned int h = (freq % 1000) / 1;
    char buffer[15] = "";
    u8g2->firstPage();
    do {
      u8g2->setFont(u8g2_font_10x20_tf);
      unsigned int h = (freq % 1000) / 10;
      sprintf(buffer, "%03u.%03u.%02u", m, k, h);
      u8g2->drawStr(15, 15, buffer);

      if (rit_enabled && !split_mode) {
        u8g2->setFont(u8g2_font_7x14_tf);
        u8g2->setCursor(1, 37);
        u8g2->print("RIT:");
        u8g2->print(rit_offset / 1000.0);
      }

      if (split_mode) {
        u8g2->setFont(u8g2_font_7x14_tf);
        u8g2->setCursor(5, 37);
        u8g2->print("Split");
      }

      layout();
    } while (u8g2->nextPage());
  }
}

void TrailRadio::layout() {
  u8g2->setFont(u8g2_font_6x12_tf);
  u8g2->drawLine(0, 20, 127, 20);
  u8g2->drawLine(0, 43, 127, 43);
  u8g2->drawLine(105, 24, 105, 39);
  u8g2->drawLine(87, 24, 87, 39);
  u8g2->drawLine(87, 48, 87, 63);
  u8g2->drawLine(15, 55, 82, 55);

  if (memMode) {
    u8g2->setFont(u8g2_font_7x14_tf);
    u8g2->setCursor(2, 37);
    u8g2->print("M:");
    u8g2->setFont(u8g2_font_6x12_tf);
    u8g2->setCursor(62, 31);
    u8g2->print("Mode");
    u8g2->setCursor(65, 40);

    switch(mode) {
      case 0: u8g2->print("USB"); break;
      case 1: u8g2->print("LSB"); break;
      case 2: u8g2->print("CW"); break;
      case 3: u8g2->print("DIG"); break;
    }
  }

  if (!memMode) {
    u8g2->setFont(u8g2_font_6x12_tf);
    u8g2->setCursor(62, 31);
    u8g2->print("Mode");
    u8g2->setCursor(65, 40);

    switch(mode) {
      case 0: u8g2->print("USB"); break;
      case 1: u8g2->print("LSB"); break;
      case 2: u8g2->print("CW"); break;
      case 3: u8g2->print("DIG"); break;
    }

    u8g2->setFont(u8g2_font_6x12_tf);
    u8g2->setCursor(92, 54);
    u8g2->print("Step:");
    u8g2->setCursor(92, 64);

    switch(freqAdjustStep) {
      case 1: u8g2->print("10Hz"); break;
      case 2: u8g2->print("100Hz"); break;
      case 3: u8g2->print("1kHz"); break;
      case 4: u8g2->print("5kHz"); break;
      case 5: u8g2->print("10kHz"); break;
      case 6: u8g2->print("100kHz"); break;
    }

    u8g2->setCursor(109, 31);
    u8g2->print("VFO:");
    u8g2->setCursor(114, 40);

    if (vfo_is_A) {
      u8g2->print("A");
    } else {
      u8g2->print("B");
    }
  }

  u8g2->setCursor(91, 35);
  
  if (!pttButtonState) {
    u8g2->print("RX");
    interfreq = IF;
  } else {
    u8g2->print("TX");
    interfreq = 0;
  }

  memlist();
  drawbargraph();
}

void TrailRadio::memlist() {
  if (memMode) {
    u8g2->setFont(u8g2_font_7x14_tf);
    u8g2->setCursor(17, 37);

    switch(memSelection) {
      case 2: u8g2->print("CALL"); break;
      case 3: u8g2->print("CHAT"); break;
      case 4: u8g2->print("FT4"); break;
      case 5: u8g2->print("FT8"); break;
      case 6: u8g2->print("JS8"); break;
      case 7: u8g2->print("JT65"); break;
      case 8: u8g2->print("PSK"); break;
      case 9: u8g2->print("RTTY"); break;
      case 10: u8g2->print("WSPR"); break;
    }
    
    if (memSelection == 1) {
      u8g2->print("2M");
      interfreq = 0;
    } else if (!pttButtonState) {
      interfreq = IF;
    }
  }
}

void TrailRadio::drawbargraph() {
  u8g2->setFont(u8g2_font_5x8_tf);
  u8g2->setCursor(15, 54);
  u8g2->print("1.3.5.7.9.10+");
  u8g2->setFont(u8g2_font_6x12_tf);
  u8g2->setCursor(2, 64);
  u8g2->print("S:");
  for (int i = 0; i < x; i++) {
    u8g2->drawBox(15 + (i * 5), 58, 2, 6);
  }
}
