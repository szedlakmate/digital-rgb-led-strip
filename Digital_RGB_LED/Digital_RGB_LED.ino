#include "config.h"
#include <FastLED.h>
#include "animation.h"
#include "palette.h"

CRGB leds[NUM_LEDS];
CRGBPalette256 currentPalette;
TBlendType currentBlending;

static long looper = 0;
long stopper = 0;
bool shouldUpdate = true;

int calculateDelayMillis() {
  return (60000.0) / ((float)NUM_LEDS * BPM * RESOLUTION);
}

int delayMillis = calculateDelayMillis();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("\n\nSTART");

  Serial.println("delayMillis:  ");
  Serial.println(delayMillis);

  if (delayMillis == 0) {
    Serial.println("Animation configuration reached maximum speed!");
  }
  
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  currentPalette = RainbowColors_p;

  stopper = millis();
  looper = 0;
  shouldUpdate = true;
  FastLED.show();
}

void loop() {
  long now = millis();
  long expectedLooper = (now - stopper) / delayMillis;

  if (expectedLooper > looper) {
    int missedFrames = expectedLooper - looper - 1;
    looper = expectedLooper;
    shouldUpdate = true;

    if (missedFrames > 0) {
      Serial.print("[ANIMATION] Skipped frames: ");
      Serial.println(missedFrames);
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }

  if (shouldUpdate) {
    FillLEDsFromPaletteColors(looper);
    FastLED.show();
    shouldUpdate = false;
  }

  delay(1);  // Small sleep to reduce CPU usage
}
