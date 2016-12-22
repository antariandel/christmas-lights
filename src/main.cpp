#include <Arduino.h>
#include <FastLED.h>

// Strip config
#define DATA_PIN 6
#define NUM_LEDS 28

// HSV Value and Saturation for animations
#define NOMINAL_VAL 192
#define NOMINAL_SAT 200

// All LEDs will not exceed this amount of power draw
#define MAX_MILLIWATTS 2000

// Timing
#define FRAMES_PER_SECOND 25

// Number of animation functions in gAnimations array
#define NUM_ANIMATIONS 5

CRGB leds[NUM_LEDS];
uint8_t gHue = 0;
uint8_t gGlitter = 0;
uint8_t gCurrentAnim = 0;
void (*gAnimations[NUM_ANIMATIONS]) (void);

// -------------------------------------------------------------------
// ANIMATION FUNCTIONS
// -------------------------------------------------------------------

void Glittery(uint8_t frequency) {
  if (random8() < frequency) { // chance of glitter
    uint8_t led_pos = random8(NUM_LEDS);
    if (leds[random8(NUM_LEDS)]) {
      leds[led_pos].maximizeBrightness();
    } else {
      leds[led_pos] = CHSV(gHue, 255, 255);
    }
  }
  fadeToBlackBy(leds, NUM_LEDS, 20);
}

void Sinelon() {
  uint8_t bpm = 20;

  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(bpm,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, NOMINAL_SAT, NOMINAL_VAL);
}

void Juggle() {
  uint8_t num = 4;

  fadeToBlackBy(leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < num; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, NOMINAL_SAT, NOMINAL_VAL);
    dothue += 256/num;
  }
  FastLED.show();
}

void Rainbow() {
  for(uint8_t i = 0; i<NUM_LEDS; i++) {
    leds[i] = CHSV(gHue+i*3, NOMINAL_SAT, NOMINAL_VAL);
  }
}

void Confetti() {
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(gHue + random8(64), NOMINAL_SAT, NOMINAL_VAL);
}

void BPM() {
  uint8_t bpm = 40;

  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(bpm, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) {
    uint8_t norm_sat = scale8(gHue+(i*2), NOMINAL_SAT);
    uint8_t norm_val = scale8(beat-gHue+(i*10), NOMINAL_VAL);
    leds[i] = ColorFromPalette(palette, norm_sat, norm_val);
  }
}

// -------------------------------------------------------------------
// MAIN
// -------------------------------------------------------------------

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInMilliWatts(MAX_MILLIWATTS);
  LEDS.clear(1); // clear the LEDS

  pinMode(5, OUTPUT);
  pinMode(4, INPUT_PULLUP);
  digitalWrite(5, LOW);
  while(digitalRead(4) == LOW); // wait here if prog jumper is on

  pinMode(A3, INPUT);
  random16_set_seed(analogRead(A3));
  gCurrentAnim = random8();
  gGlitter = random8();
  gHue = random8(); // randomize!

  gAnimations[0] = Sinelon;
  gAnimations[1] = Juggle;
  gAnimations[2] = Rainbow;
  gAnimations[3] = Confetti;
  gAnimations[4] = BPM;
}

void loop() {
  gAnimations[gCurrentAnim % NUM_ANIMATIONS]();
  Glittery(gGlitter);

  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND);

  EVERY_N_MILLISECONDS(10) { gHue++; }
  EVERY_N_MILLISECONDS(10000) {
    if (random8() < 25) { // 10% chance to change anim
      gCurrentAnim = random8();
    }
    if (random8() < 128) { // 50% chance to randomize glitter
      gGlitter = random8()/2;
    } else {
      gGlitter = 0; // no glitter
    }
  }
}
