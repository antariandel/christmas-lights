#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "Animations.h"

#define PIN 7 // was 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); //reset strip
}

void loop() {
  rainbowCycle(strip, 20);
}
