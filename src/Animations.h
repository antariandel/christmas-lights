#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// Input a value 0 to 255 to get a color value.
// The colours are a transition R - G - B - R.
uint32_t colorWheel(Adafruit_NeoPixel strip, byte wheel_pos);

// Fill the dots one after the other with a color.
void colorWipe(Adafruit_NeoPixel strip, uint32_t c, uint8_t wait);

// Rainbow equally distributed throughout, cycling 5 times.
void rainbowCycle(Adafruit_NeoPixel strip, uint8_t wait);
