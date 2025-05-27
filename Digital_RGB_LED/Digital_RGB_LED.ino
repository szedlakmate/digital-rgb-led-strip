#include "debug.h"
#include "config.h"
#include <FastLED.h>
#include "animation.h"
#include "palette.h"
#include "knob.h"

CRGB leds[NUM_LEDS];
CRGBPalette256 currentPalette;
TBlendType currentBlending;

static long looper = 0;
long stopper = 0;
bool shouldUpdate = true;

int resolution = RESOLUTION;
int brightness = BRIGHTNESS;
float bpm = BPM;

int calculateDelayMillis() {
  int delayMillis = (60000.0) / ((float)NUM_LEDS * bpm * (float)resolution);
  if (delayMillis == 0) {
    dbg::println("Animation configuration reached maximum speed!");
    delayMillis = 1;
  }
  return delayMillis;
}

int delayMillis = calculateDelayMillis();

void setup() {
  dbg::begin();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  dbg::println("\n\nSTART");

  dbg::println("Resolution:  ");
  dbg::println(RESOLUTION);

  dbg::println("delayMillis:  ");
  dbg::println(delayMillis);

  delay(500);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(brightness);

  // OceanColors_p, CloudColors_p, LavaColors_p, HeatColors_p, ForestColors_p, and PartyColors_p., RainbowColors_p, RainbowStripeColors_p
  currentPalette = RainbowStripeColors_p;

  stopper = millis();
  looper = 0;
  shouldUpdate = true;
}

void loop() {
  long now = millis();
  long expectedLooperDiff = (now - stopper) / delayMillis;

  if (expectedLooperDiff > 0) {
    int missedFrames = expectedLooperDiff - 1;
    looper += expectedLooperDiff;
    shouldUpdate = true;

    if (missedFrames > 0) {
      dbg::print("[ANIMATION] Skipped frames: ");
      dbg::println(missedFrames);
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }

  // // Uncomment these lines to allow setting brightness by a knob connected to A0 pin
  // int newBrightness = calculateKnobValueForPin<int>(A0, 0, 250, 0, 1023);
  // if (abs(brightness - newBrightness) > 3) { // add thresshold to avoid flickering
  //   dbg::print("[ANIMATION] Brightness changed from ");
  //   dbg::print(brightness);
  //   dbg::print(" to ");
  //   dbg::println(newBrightness);
  //   brightness = newBrightness;
  //   FastLED.setBrightness(brightness);
  // }

  // // Uncomment these lines to allow setting BPM by a knob connected to A0 pin
  // float newBPM = calculateKnobValueForPin<float>(A0, 0.01, 25.0, 0, 1023);
  // if (abs(bpm - newBPM) > 0.6) {  // add thresshold to avoid flickering
  //   dbg::print("[ANIMATION] BPM changed from ");
  //   dbg::print(bpm);
  //   dbg::print(" to ");
  //   dbg::println(newBPM);
  //   bpm = newBPM;
  //   delayMillis = calculateDelayMillis();
  // }


  if (shouldUpdate) {
    FillLEDsFromPaletteColors(looper, resolution);
    FastLED.show();
    stopper = now;
    shouldUpdate = false;
  }
}
