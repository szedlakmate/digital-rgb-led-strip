/***************
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 ***************/

#include <SoftwareSerial.h>
SoftwareSerial bluetooth(10, 11);
// Expected message format:
// #250,2.0,0.3;      #(int)BRIGHTNESS,(float)BPM,(float)WAVELENGTHSCALE;

// Original source: https://www.instructables.com/Tutorial-How-to-4-Digit-Display-Interface-With-Ard/
#include <FastLED.h>
#include <TM1637Display.h>

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

// Pins
#define LED_PIN 25    // arduino digital pin
#define NUM_LEDS 300  // total num of leds on the full strip

int BRIGHTNESS = 250;  // max: 250
#define BRIGHTNESS_MAX 250


CRGB leds[NUM_LEDS];
CRGB ledsPreset[NUM_LEDS];

// Wave per miniute. 1 means it takes 60 sec to flow through each LEDs
// Max BPM is ~10 for 300 LEDS
float BPM = 2.0;
#define BPM_MAX 15.0

// Scales the wave's length. >1.0 means overlays the stripe. Default: 1.0
float WAVE_LENGTH_SCALE = 0.300;
#define WAVE_LENGTH_SCALE_MAX 5.0

CRGBPalette256 currentPalette;
TBlendType currentBlending;

static long looper = 0;
static char delimiter = ';';


int determineDelayMillis() {
  return (60000.0 * 2.0) / ((float)NUM_LEDS * max(BPM, 0.00000000001));
}

// Determine delay time based on BPM
static int delayMillis = determineDelayMillis();
long stopper = millis();


void setup() {
  delay(500);  // power-up safety delay
  bluetooth.begin(9600);
  bluetooth.println("Bluetooth On");
  Serial.begin(9600);
  Serial.println("START");

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  currentBlending = LINEARBLEND;

  // OceanColors_p, CloudColors_p, LavaColors_p, HeatColors_p, ForestColors_p, and PartyColors_p., RainbowColors_p, RainbowStripeColors_p
  currentPalette = RainbowColors_p;

  FillLEDsFromPaletteColors(0);
}


void loop() {
  looper += 1;
  FillLEDsFromPaletteColors(looper);

  // Determine accurate sleep time
  long now = millis();
  long waitMoreMillis = max(delayMillis - now + stopper, 0);
  if (waitMoreMillis == 0) {
    Serial.print("Missed [ms]:   ");
    Serial.println(-(delayMillis - now + stopper));
  }

  waitAndMeasure(waitMoreMillis);

  stopper = now;
}

void waitAndMeasure(int waitMillis) {
  int samplingCount = max(round(waitMillis / 300.0), 1);

  if (samplingCount > 10) {
    Serial.print("!! samplingCount:   ");
    Serial.println(samplingCount);
  }

  for (int i = 0; i < samplingCount; i += 1) {
    FastLED.delay(waitMillis / samplingCount);
    readBluetooth();

    if (BPM == 0.0) {
      i = 0;
    }

    if (determineDelayMillis() < waitMillis * i / samplingCount && BPM > 0.0) {
      break;
    }
  }
}

void FillLEDsFromPaletteColors(int looper) {
  for (int i = 0; i < NUM_LEDS; i++) {
    float colorIndex = (float)(i + looper) / (WAVE_LENGTH_SCALE * (float)NUM_LEDS) * 256.0;

    leds[i] = ColorFromPalette(currentPalette, colorIndex, BRIGHTNESS, currentBlending);
  }
}

void readBluetooth() {
  String inputString = "";
  for (int i = 1; i <= 28; i = i + 1) {
    if (bluetooth.available()) {
      // get the new byte:
      char inChar = (char)bluetooth.read();
      // add it to the inputString:
      inputString += inChar;
    }
    // do something else while waiting
  }

  if (inputString != "") {
    bluetooth.println("inputString: ");
    bluetooth.println(inputString);
  }

  if (inputString[0] == '#') {

    // Process the parameters here
    parseParameters(inputString);
    // Send an acknowledgment
    bluetooth.println("ACK");
  }
}

void parseParameters(String params) {
  int commaIndex1 = params.indexOf(',');
  int commaIndex2 = params.indexOf(',', commaIndex1 + 1);

  if (commaIndex1 != -1 && commaIndex2 != -1) {
    BRIGHTNESS = params.substring(1, commaIndex1).toInt();
    BPM = params.substring(commaIndex1 + 1, commaIndex2).toFloat();
    WAVE_LENGTH_SCALE = params.substring(commaIndex2 + 1).toFloat();
    delayMillis = determineDelayMillis();

    // Process the parameters further if needed
    bluetooth.print("BRIGHTNESS: ");
    bluetooth.println(BRIGHTNESS);
    bluetooth.print("BPM: ");
    bluetooth.println(BPM);
    bluetooth.print("WAVE_LENGTH_SCALE: ");
    bluetooth.println(WAVE_LENGTH_SCALE);
  } else {
    // Throw an exception if parsing fails
    bluetooth.println("Parsing error");
  }
}

/*******************************************************
 * REFFERENCES & EXAMPLES from unknown external source *
 *******************************************************/

// This example shows several ways to set up and use 'palettes' of colors
// with FastLED.
//
// These compact palettes provide an easy way to re-colorize your
// animation on the fly, quickly, easily, and with low overhead.
//
// USING palettes is MUCH simpler in practice than in theory, so first just
// run this sketch, and watch the pretty lights as you then read through
// the code.  Although this sketch has eight (or more) different color schemes,
// the entire sketch compiles down to about 6.5K on AVR.
//
// FastLED provides a few pre-configured color palettes, and makes it
// extremely easy to make up your own color schemes with palettes.
//
// Some notes on the more abstract 'theory and practice' of
// FastLED compact palettes are at the bottom of this file.

// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.


// void SetColorPalette(String color) {
//   // change all 16 palette entries...
//   fill_solid(currentPalette, 16, color);
// }

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

// Additionl notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes.
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.
