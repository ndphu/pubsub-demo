#ifdef __ESP_V12__
#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "SH1106.h"
//#include "OLEDDisplayUi.h"
#include "Display.h"

SH1106  display(0x3c, D6, D5);
//OLEDDisplayUi ui(&display);

String text = "<Empty>";

int drawX = 0;

void setDisplayText(String t) {
  text = t;
}

void setFontSize(const int size) {
  // 0 = small (ArialMT_Plain_10)
  // 1 = medium (ArialMT_Plain_16)
  // 2 = large (ArialMT_Plain_24)
  switch (size) {
    case 0:
      display.setFont(ArialMT_Plain_10);
      break;
    case 1:
      display.setFont(ArialMT_Plain_16);
      break;
    case 2:
      display.setFont(ArialMT_Plain_24);
      break;
    default:
      display.setFont(ArialMT_Plain_10);
  }
}

void setTextAlignment(const int align) {
  // 0 = left (TEXT_ALIGN_LEFT)
  // 1 = left (TEXT_ALIGN_CENTER)
  // 2 = left (TEXT_ALIGN_RIGHT)
  switch (align) {
    case 0:
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      drawX = 0;
      break;
    case 1:
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      drawX = 63;
      break;
    case 2:
      display.setTextAlignment(TEXT_ALIGN_RIGHT);
      drawX = 127;
      break;
    default:
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      drawX = 0;
  }
}




void displaySetup() {
  display.init();
  display.flipScreenVertically();
}

void displayLoop() {
  display.clear();
  display.drawStringMaxWidth(drawX, 0, 128, text);
  display.display();
}

#endif
#endif
