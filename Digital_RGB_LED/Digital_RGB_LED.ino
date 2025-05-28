#include "debug.h"
#include "config.h"
#include <FastLED.h>
#include "animation.h"
#include "palette.h"
#include "knob.h"
#include "ultrasound.h"

CRGB leds[NUM_LEDS];
CRGBPalette256 currentPalette;
TBlendType currentBlending;

// Palette array and index definitions
const TProgmemRGBPalette16* const* gPalettes = PREDEFINED_PALETTES;
const uint8_t gPaletteCount = PREDEFINED_PALETTES_COUNT;
uint8_t gPaletteIndex = PALETTE_INDEX;

static long looper = 0;
long stopper = 0;
bool shouldUpdate = true;

int resolution = RESOLUTION;
int brightness = BRIGHTNESS;
float waveLengthScale = WAVE_LENGTH_SCALE;
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

  ultrasoundSetup();  // If pins are not set, it does not execute anything

  delay(500);

  uint8_t sel = (PALETTE_INDEX < PREDEFINED_PALETTES_COUNT)
                  ? PALETTE_INDEX
                  : 0;
  currentPalette = *(PREDEFINED_PALETTES[sel]);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(brightness);

  // Set currentPalette from palette array and index
  if (gPaletteIndex >= gPaletteCount) gPaletteIndex = 0;
  currentPalette = CRGBPalette16(*gPalettes[gPaletteIndex]);

  stopper = millis();
  looper = 0;
  shouldUpdate = true;
}

void loop() {
  handleUltrasound();  // If pins are not set, this function does nothing

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

  // Allow setting brightness by a knob connected to A0 pin
  // brightnessByKnob();

  // Allow setting BPM by a knob connected to A0 pin
  // bpmByKnob();

  // Allow setting wave length scale by a knob connected to A0 pin
  // waveLengthByKnob();

  if (shouldUpdate) {
    FillLEDsFromPaletteColors(looper, resolution, waveLengthScale);
    FastLED.show();
    stopper = now;
    shouldUpdate = false;
  }
}

void brightnessByKnob() {
  int newBrightness = calculateKnobValueForPin<int>(A0, 1, 255, 0, 1023);
  if (brightness != newBrightness) {
    dbg::print("[ANIMATION] Brightness changed from ");
    dbg::print(brightness);
    dbg::print(" to ");
    dbg::println(newBrightness);
    brightness = newBrightness;
    FastLED.setBrightness(brightness);
  }
}

void bpmByKnob() {
  float newBPM = calculateKnobValueForPin<float>(A0, 0.01, 25.0, 0, 1023);
  if (abs(bpm - newBPM) > 0.6) {  // add threshold to avoid flickering
    dbg::print("[ANIMATION] BPM changed from ");
    dbg::print(bpm);
    dbg::print(" to ");
    dbg::println(newBPM);
    bpm = newBPM;
    delayMillis = calculateDelayMillis();
  }
}

void waveLengthByKnob() {
  float newWaveLengthScale = calculateKnobValueForPin<float>(A0, 0.01, 5.0, 0, 1023);
  if (abs(waveLengthScale - newWaveLengthScale) > 0.075) {  // add threshold to avoid flickering
    dbg::print("[ANIMATION] Wave length scale changed from ");
    dbg::print(waveLengthScale);
    dbg::print(" to ");
    dbg::println(newWaveLengthScale);
    waveLengthScale = newWaveLengthScale;
  }
}

void handleUltrasound() {
  // If pins are not set, this function does nothing
  ultrasoundUpdate();

  if (ultrasoundHasReading()) {  // non-blocking check
    float mm = ultrasoundRead_mm();
    // dbg::print("[US] ");
    // dbg::println(mm);

    /* example: map 30-800 mm to brightness 50-255 */
    int newBright = constrain(
      map((int)mm, 30, 800, 255, 50), 50, 255);

    if (abs(newBright - brightness) > 5 && mm < 1000) {  // ensure that measuers out of the "useful" range are not consumed
      brightness = newBright;
      FastLED.setBrightness(brightness);
    }
  }
}
