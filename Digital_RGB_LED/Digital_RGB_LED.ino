/***************
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 ***************/


#include <FastLED.h>

// Pins
#define LED_PIN 25  // arduino digital pin

#define NUM_LEDS 300    // total num of leds on the full strip
#define BRIGHTNESS 120  // max: 250
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];
CRGB ledsPreset[NUM_LEDS];

// Wave per miniute. 1 means it takes 60 sec to flow through each LEDs
// Max BPM is ~10 for 300 LEDS
#define BPM 2.0

// Scales the wave's length. >1.0 means overlays the stripe. Default: 1.0
#define WAVE_LENGTH_SCALE 1.00

CRGBPalette256 currentPalette;
TBlendType currentBlending;

static long looper = 0;

// Determine delay time based on BPM
static int delayMillis = (60000.0 * 2.0) / ((float)NUM_LEDS * BPM);
long stopper = millis();


void setup() {
  delay(500);  // power-up safety delay
  Serial.begin(1000000); Serial.println("START");

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  currentBlending = LINEARBLEND;

  // OceanColors_p, CloudColors_p, LavaColors_p, HeatColors_p, ForestColors_p, and PartyColors_p., RainbowColors_p, RainbowStripeColors_p
  currentPalette = RainbowColors_p;

  InitStripe();
}


void loop() {
  looper = (looper + 1) % NUM_LEDS;
  Serial.print("looper: "); Serial.println(looper);

  FillLEDsFromPaletteColors(looper);

  // Determine accurate sleep time
  long now = millis();
  long waitMoreMillis = max(delayMillis - now + stopper, 0);
  if (waitMoreMillis == 0) {
    Serial.print("Missed [ms]:   "); Serial.println(-(delayMillis - now + stopper));
  }
  FastLED.delay(waitMoreMillis);
  stopper = now;
}

void InitStripe() {
  for (int i = 0; i < NUM_LEDS; i += 1) {
    float colorIndex = (float)i / (WAVE_LENGTH_SCALE * (float)NUM_LEDS) * 256.0;
    
    ledsPreset[i] = ColorFromPalette(currentPalette, colorIndex, BRIGHTNESS, currentBlending);
  }
}

void FillLEDsFromPaletteColors(int looper) {
  for (int i = 0; i < NUM_LEDS; i++) {
    int index = looper + i;
    leds[i] = ledsPreset[index % NUM_LEDS];
  }
}

/*******************************************************
 * REFFERENCES & EXAMPLES from unknown external source *
 *******************************************************/

// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.

/*
void SetColorPalette(String color)
{

    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, color);
}
*/
// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette() {
  // 'black out' all 16 palette entries...
  fill_solid(currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;
}



// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette() {
  CRGB purple = CHSV(HUE_PURPLE, 255, 255);
  CRGB green = CHSV(HUE_GREEN, 255, 255);
  CRGB black = CRGB::Black;

  currentPalette = CRGBPalette16(
    green, green, black, black,
    purple, purple, black, black,
    green, green, black, black,
    purple, purple, black, black);
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM = {
  CRGB::Red,
  CRGB::Gray,  // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};