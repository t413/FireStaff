
#ifndef _EFFECTS_H
#define _EFFECTS_H

#ifdef __cplusplus
extern "C"{
#endif

#include <inttypes.h>
#define ARRAY_HEIGHT 11

typedef uint16_t cycle_t;
typedef void (*SetPixelColor)(uint16_t, uint32_t);

typedef struct {
  int16_t goal;
  int16_t rate;
  int16_t current;
} FirePersist;

#define TEXTMAXLEN 40
typedef struct {
  int32_t fgcolor;
  int32_t bgcolor;
  char text[TEXTMAXLEN];
  uint8_t len;
} TextPersist;

uint8_t fire(SetPixelColor setter, FirePersist *persist, const cycle_t cycle);
uint8_t text(SetPixelColor setter, TextPersist *persist, const cycle_t cycle);

//color utility functions
uint32_t color(uint8_t r, uint8_t g, uint8_t b);
uint32_t wheel(int wheelPos);
uint32_t dim(uint32_t color, uint8_t b);
uint32_t dimrgb(uint32_t color, uint8_t bright_r, uint8_t bright_g, uint8_t bright_b);
uint32_t fadeBetween(uint32_t c1, uint32_t c2, uint8_t value, uint8_t outOf);


//random number generation
uint32_t deadrand();
void deadsrand(uint32_t x);


#ifdef __cplusplus
} // extern "C"
#endif
#endif //_EFFECTS_H
