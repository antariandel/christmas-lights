#include <Arduino.h>
#include <FastLED.h>

// Strip config
#define DATA_PIN 10
#define NUM_LEDS 42

// HSV Value and Saturation for animations
#define NOMINAL_VAL 255
#define NOMINAL_SAT 255

// All LEDs will not exceed this amount of power draw
//#define MAX_MILLIWATTS 2000

// Timing
#define FRAMES_PER_SECOND 60

// Number of animation functions in gAnimations array
#define NUM_ANIMATIONS 6

CRGB leds[NUM_LEDS];
uint8_t gHue = 0;
uint8_t led_pos[NUM_LEDS] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 14, 15, 16, 11, 12, 13, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 35, 36, 37, 31, 32, 33, 34, 38, 39, 40, 41, 42};
uint8_t gGlitter = 0;
uint8_t gCurrentAnim = 0;
void (*gAnimations[NUM_ANIMATIONS]) (void);

// -------------------------------------------------------------------
// ANIMATION FUNCTIONS
// -------------------------------------------------------------------

void Glittery(uint8_t frequency) {
  if (random8() < frequency) { // chance of glitter
    leds[random8(NUM_LEDS)] = CHSV(gHue, 255, 255);
  }
}

void Sinelon() {
  uint8_t bpm = 20;

  int pos = beatsin16(bpm,0,NUM_LEDS);
  leds[led_pos[pos]-1] += CHSV( gHue, NOMINAL_SAT, NOMINAL_VAL);
}

void Juggle() {
  uint8_t num = 3;

  byte dothue = 0;
  for( int i = 0; i < num; i++) {
    leds[led_pos[beatsin16(i+7,0,NUM_LEDS)]-1] |= CHSV(dothue, NOMINAL_SAT, NOMINAL_VAL);
    dothue += 256/num;
  }
  FastLED.show();
}

void Rainbow() {
  for(uint8_t i = 0; i<NUM_LEDS; i++) {
    leds[led_pos[i]-1] = CHSV(gHue+i*3, NOMINAL_SAT, NOMINAL_VAL);
  }
}

void Confetti() {
  leds[random8(NUM_LEDS)] = CHSV(random8(255), NOMINAL_SAT, NOMINAL_VAL);
}

void Vibro() {
  fill_solid(leds, 54, CHSV(gHue, NOMINAL_SAT, NOMINAL_VAL));
}

void BPM() {
  uint8_t bpm = 75;

  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(bpm, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) {
    uint8_t norm_sat = scale8(gHue+(i*2), NOMINAL_SAT);
    uint8_t norm_val = scale8(beat-gHue+(i*10), NOMINAL_VAL);
    leds[led_pos[i]-1] = ColorFromPalette(palette, norm_sat, norm_val);
  }
}

// -------------------------------------------------------------------
// MAIN
// -------------------------------------------------------------------

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  #ifdef MAX_MILLIWATTS
    FastLED.setMaxPowerInMilliWatts(MAX_MILLIWATTS);
  #endif
  LEDS.clear(1); // clear the LEDS

  pinMode(5, OUTPUT);
  pinMode(4, INPUT_PULLUP);
  digitalWrite(5, LOW);
  while(digitalRead(4) == LOW); // wait here if prog jumper is on

  digitalWrite(13, HIGH);

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
  gAnimations[5] = Vibro;
}

void loop() {
  fadeToBlackBy(leds, NUM_LEDS, 20);

  gAnimations[gCurrentAnim % NUM_ANIMATIONS]();
  //Glittery(gGlitter);

  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND);

  EVERY_N_MILLISECONDS(500) {
    digitalWrite(13, !digitalRead(13));
  }


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
