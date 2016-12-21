#include "Animations.h"

uint32_t colorWheel(Adafruit_NeoPixel strip, byte wheel_pos) {
  wheel_pos = 255 - wheel_pos;
  if(wheel_pos < 85) {
    return strip.Color(255 - wheel_pos * 3, 0, wheel_pos * 3);
  }
  if(wheel_pos < 170) {
    wheel_pos -= 85;
    return strip.Color(0, wheel_pos * 3, 255 - wheel_pos * 3);
  }
  wheel_pos -= 170;
  return strip.Color(wheel_pos * 3, 255 - wheel_pos * 3, 0);
}


void colorWipe(Adafruit_NeoPixel strip, uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}


void rainbowCycle(Adafruit_NeoPixel strip, uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, colorWheel(strip, ((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
