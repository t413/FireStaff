#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include "effects.h"

Adafruit_NeoPixel s1 = Adafruit_NeoPixel(ARRAY_HEIGHT, 5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel s2 = Adafruit_NeoPixel(ARRAY_HEIGHT, 6, NEO_GRB + NEO_KHZ800);
SoftwareSerial btSerial(2, 3); // RX, TX
uint8_t rxbuf[128] = {0};
uint8_t rxpos = 0;


void setup() {
  //Serial.begin(115200);
  btSerial.begin(57600);
//  s1.setBrightness(100);
//  s2.setBrightness(100);
  s1.begin();
  s1.show();
  s2.begin();
  s2.show();
}

void setS1(uint16_t i, uint32_t c) { s1.setPixelColor(i,c); }
void setS2(uint16_t i, uint32_t c) { s2.setPixelColor(i,c); }
void setBoth(uint16_t i, uint32_t c) {
  s1.setPixelColor(i,c);
  s2.setPixelColor(i,c);
}

TextPersist text1 = { 0x07CFF, 0x4E5E00,"YO !  ", 9 };
FirePersist fire1 = {0}, fire2 = {0};
//uint8_t persistBuffer[128] = {0};

void loop() {
  uint8_t mode = 0;
  uint16_t cycle = 0;
  uint32_t lastChangeMode = 0;

  while(1) {
    uint32_t current = millis();

    uint8_t ret = 0;
    if (mode == 0) { // fire
      ret = fire(setS1, &fire1, cycle);
      ret = fire(setS2, &fire2, cycle);
    } else if (mode == 1) {
      ret = text(setBoth, &text1, cycle);
    } else {
    }
    if (ret) { cycle = 0; }
    s1.show();
    s2.show();

    if (current - lastChangeMode >= 10000){
      if (mode == 1) { mode = 0; }
      else {
        mode = 1;
        deadsrand(text1.fgcolor);
        uint16_t wpos = deadrand() & 0xFF;
        text1.fgcolor = wheel(wpos);
        text1.bgcolor = dim(wheel((wpos + 128) % 256), 50);
     }
      lastChangeMode = current;
    }

    if (btSerial.available() > 1) {
      while (btSerial.available()) {
        char in = btSerial.read();
        if (in == '\n' || in == 0 || in == '\r') {
          processCommands();
        } else {
          rxbuf[rxpos++] = in;
        }
      }

    }

    delay(1);
    cycle++;
  }
}

void processCommands() {
  //memset(persistBuffer, 0, 128);
  deadsrand(text1.fgcolor);
  uint16_t wpos = deadrand() & 0xFF;
  text1.fgcolor = wheel(wpos);
  text1.bgcolor = dim(wheel((wpos + 128) % 256), 50);
  strncpy(text1.text, (const char*)rxbuf, TEXTMAXLEN);
  text1.len = min(strlen(text1.text), 40);
}



