

#include "effects.h"
#include "fonts.c"
#include "Arduino.h"



/* ------ FIRE ------ */

uint8_t fire(SetPixelColor setter, FirePersist *vals, const cycle_t cycle) {
  if ((vals->rate >= 0) != (vals->goal > vals->current)) {
    uint32_t r = deadrand();
    vals->goal = constrain((int8_t)(r & 0x000F) - 2, 0, 10) << 4; //randomish from 0-10
    vals->rate = ((r & 0x02F0 >> 4) + 5); //from 0-31
    if (vals->goal < vals->current) { vals->rate = -(vals->rate); }
  }
  int8_t currentLed = vals->current >> 4;
  for (uint8_t i=0; i < ARRAY_HEIGHT; i++) {
    uint32_t c = (i > currentLed)? 0 : fadeBetween(0xFF6500, 0xFF0000, i, currentLed);
    if (i == currentLed) { dim(c, map(vals->current & 0xF, 0, 0xF, 0, 0xFF)); }
    (*setter)(i, c);
  }
  vals->current += vals->rate;
  return 0;
}



/* ------ text ------ */

uint8_t getTextLine(const char character, const uint8_t verticalLine) {
  return pgm_read_byte(font+((constrain(character, 32, 126) - 32) * 5) + constrain(verticalLine, 0, 5));
}

uint8_t text(SetPixelColor setter, TextPersist *vals, const cycle_t cycle) {
  uint16_t safeCycle = constrain(cycle, 0, vals->len * FONTWIDTH);
  char message_pos = safeCycle / FONTWIDTH;
  uint8_t linepos = safeCycle - (message_pos * FONTWIDTH); //hopefully more efficient than modulo
  char c = vals->text[message_pos];
  uint8_t linepx = getTextLine(c, linepos);
  
  for (uint8_t i=0; i < ARRAY_HEIGHT; i++) {
    int8_t bitindex = FONTHEIGHT - i + 1;
    uint32_t c = ((bitindex >= 0 && bitindex < FONTHEIGHT) && (linepx >> bitindex) & 1)? vals->fgcolor : vals->bgcolor;
    (*setter)(i, c);
  }
  return (cycle >= vals->len * FONTWIDTH);
}





/* ------------------- */
/* ------ color ------ */
/* ------------------- */


uint32_t color(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t wheel(int wheelPos) {
  uint8_t pos = wheelPos & 0xFF;
  if(pos < 85) {
    return color(pos * 3, 255 - pos * 3, 0);
  }
  else if(pos < 170) {
    pos -= 85;
    return color(255 - pos * 3, 0, pos * 3);
  }
  else {
    pos -= 170;
    return color(0, pos * 3, 255 - pos * 3);
  }
}

uint32_t dimrgb(uint32_t incolor, uint8_t bright_r, uint8_t bright_g, uint8_t bright_b) {
  uint32_t r = map((incolor >> 16 & 0xFF), 0, 255, 0, bright_r);
  uint16_t g = map((incolor >> 8  & 0xFF), 0, 255, 0, bright_g);
  uint16_t b = map((incolor & 0xFF), 0, 255, 0, bright_b);
  return color(r, g, b);
}

uint32_t dim(uint32_t incolor, uint8_t b) {
  return dimrgb(incolor, b, b, b);
}

uint32_t fadeBetween(uint32_t c1, uint32_t c2, uint8_t value, uint8_t outOf) {
  return color(
    map(value, 0, outOf, (c1 >> 16) & 0xFF, (c2 >> 16) & 0xFF),
    map(value, 0, outOf, (c1 >>  8) & 0xFF, (c2 >>  8) & 0xFF),
    map(value, 0, outOf,  c1        & 0xFF,  c2        & 0xFF));
}



/* ---------------------------- */
/* ------ random num gen ------ */
/* ---------------------------- */

//based on http://inglorion.net/software/deadbeef_rand/
static uint32_t deadbeef_seed;
static uint32_t deadbeef_beef = 0xdeadbeef;

void deadsrand(uint32_t x) {
  deadbeef_seed = x;
  deadbeef_beef = 0xdeadbeef;
}

uint32_t deadrand() {
  deadbeef_seed = (deadbeef_seed << 7) ^ ((deadbeef_seed >> 25) + deadbeef_beef);
  deadbeef_beef = (deadbeef_beef << 7) ^ ((deadbeef_beef >> 25) + 0xdeadbeef);
  return deadbeef_seed;
}

