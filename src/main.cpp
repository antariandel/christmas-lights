#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 28
#define DATA_PIN 6
#define FRAMES_PER_SECOND 20

CRGB leds[NUM_LEDS];

uint8_t gHue = 0;
uint8_t gCurrentAnim = 0;
void (*gAnimations[6]) (void);

// -------------------------------------------------------------------
// ANIMATION FUNCTIONS
// -------------------------------------------------------------------

void Juggle() {
  uint8_t num = 4;

  fadeToBlackBy(leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < num; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 256/num;
  }
  FastLED.show();
}

void Confetti() {
  uint8_t frequency = 255;

  fadeToBlackBy(leds, NUM_LEDS, 20);
  if (random8() < frequency) {
    int pos = random16(NUM_LEDS);
    leds[pos] += CHSV(gHue + random8(64), 200, 255);
  }
}

void Sinelon() {
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void Glittery(uint8_t frequency) {
  if( random8() < frequency) { //chance of glitter
    leds[ random16(NUM_LEDS) ] += CRGB::Yellow;
  }
  fadeToBlackBy(leds, NUM_LEDS, 20);
}

void Rainbow() {
  fill_rainbow(leds, NUM_LEDS, gHue, 5);
}

void RainbowDarker(uint8_t value) {
  for(uint8_t i = 0; i<NUM_LEDS; i++) {
    leds[i] = CHSV(gHue+i*3, 255, value);
  }
}

void GlitteryRainbow() {
  RainbowDarker(128);
  Glittery(64);
}

void BPM() {
  uint8_t BeatsPerMinute = 40;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

// -------------------------------------------------------------------
// MAIN
// -------------------------------------------------------------------

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
	LEDS.setBrightness(255);
  LEDS.clear(1);

  pinMode(5, OUTPUT);
  pinMode(4, INPUT_PULLUP);
  digitalWrite(5, LOW);
  while(digitalRead(4) == LOW); //wait here if jumper is on

  gAnimations[0] = Sinelon;
  gAnimations[1] = Juggle;
  gAnimations[2] = GlitteryRainbow;
  gAnimations[3] = Rainbow;
  gAnimations[4] = Confetti;
}

void loop() {
  gAnimations[gCurrentAnim % 6]();

  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND);
  EVERY_N_MILLISECONDS(10) { gHue++; }
  EVERY_N_MILLISECONDS(30000) { gCurrentAnim++; }
}
