#ifndef PALETTE_H
#define PALETTE_H

#include <FastLED.h>

void SetupBlackAndWhiteStripedPalette();
void SetupPurpleAndGreenPalette();
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

extern const TProgmemRGBPalette16* const PREDEFINED_PALETTES[];
constexpr uint8_t PREDEFINED_PALETTES_COUNT = 8;  // keep in sync!

#endif  // PALETTE_H
