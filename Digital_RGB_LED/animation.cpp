#include "animation.h"
#include "config.h"
#include <FastLED.h>
#include "palette.h"

void FillLEDsFromPaletteColors(long colorShift) {
  for (int i = 0; i < NUM_LEDS; i++) {
    int index = i;
    if (REVERSED) {
      index = NUM_LEDS - i - 1;
    }

    long colorIndex = ((long)i * (long)256) / (float)WAVE_LENGTH_SCALE / ((long)NUM_LEDS) + colorShift / (long)RESOLUTION;

    if (RESOLUTION == 1) {
      leds[index] = ColorFromPalette(currentPalette, colorIndex + colorShift, BRIGHTNESS, currentBlending);
    } else {
      // Apply smoothing
      leds[index] = blend(
        ColorFromPalette(currentPalette, colorIndex, BRIGHTNESS, currentBlending),
        ColorFromPalette(currentPalette, colorIndex + 1, BRIGHTNESS, currentBlending),
        (float)(colorShift % RESOLUTION) * 255.0 / (float)RESOLUTION);
    }
  }
}

