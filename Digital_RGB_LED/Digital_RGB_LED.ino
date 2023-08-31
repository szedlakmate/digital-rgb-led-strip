/***************
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 ***************/


/* ********************************************************** */
// WIFI CONFIG
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <FS.h>  // Include the SPIFFS library

// Credentials
#if defined __has_include
#if __has_include("credentials.h")
#include "credentials.h"
#else
// if you dont have a credentials.h file you can set them manually here
#define WIFI_SSID "Wifi SSID"
#define WIFI_PASS "password"
#endif
#endif

ESP8266WebServer server(3000);
/* ********************************************************** */


#include <FastLED.h>

// Pins
#define LED_PIN D2  // arduino digital pin

#define NUM_LEDS 300  // total num of leds on the full strip
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

// Wave per miniute. 1 means it takes 60 sec to flow through each LEDs
// Max BPM is ~10 for 300 LEDS
float BPM = 2.0;
int BRIGHTNESS = 180;  // max: 250

// Scales the wave's length. >1.0 means overlays the strip. Default: 1.0
float WAVE_LENGTH_SCALE = 2.0;
// Defines the smoothness of the animation. The higher the smoother, but can slow down the controller.
// If the animation is lagging, the built-in LED turns on
int RESOLUTION = 2;

CRGBPalette256 currentPalette;
TBlendType currentBlending;

bool reversed = false;
static long looper = 0;
bool shouldUpdate = true;
bool connectedToNet = false;

// Determine delay time based on BPM
int calculateDelayMillis() {
  return (60000.0) / ((float)NUM_LEDS * BPM * RESOLUTION);
}

int delayMillis = calculateDelayMillis();
long stopper = millis();


void setup() {
  delay(500);  // power-up safety delay
  Serial.begin(1000000);
  Serial.println("\n\nSTART");

  Serial.println("delayMillis:  ");
  Serial.println(delayMillis);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // WIFI
  setupWifi();

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  currentBlending = LINEARBLEND;

  // OceanColors_p, CloudColors_p, LavaColors_p, HeatColors_p, ForestColors_p, and PartyColors_p., RainbowColors_p, RainbowStripeColors_p
  currentPalette = RainbowColors_p;
  reversed = false;
}


void loop() {
  if (shouldUpdate) {
    FillLEDsFromPaletteColors(looper);
    shouldUpdate = false;
  }

  if (!connectedToNet && WiFi.status() == WL_CONNECTED) {  //Wait for connection
    connectedToNet = true;
    Serial.println("\nConnected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //Print the local IP
    server.begin();                  //Start the server
    Serial.println("Server listening");
    digitalWrite(LED_BUILTIN, HIGH);
  }


  // Web connection
  server.handleClient();  // Handling of incoming requests


  // Determine is refresh is needed
  long now = millis();
  long waitMoreMillis = max(delayMillis - now + stopper, (long int)0);

  if (waitMoreMillis == 0) {
    // Serial.print("looper: ");
    // Serial.println(looper);
    FastLED.show();
    shouldUpdate = true;
    looper = looper + 1;

    int missedMillis = -(delayMillis - now + stopper);
    if (missedMillis > 0) {
      Serial.print("! Missed [ms]:   ");
      Serial.println(missedMillis);
      digitalWrite(LED_BUILTIN, LOW);
    } else {
      digitalWrite(LED_BUILTIN, HIGH);
    }

    if (delayMillis == 0) {
      Serial.println("Animation configuration reached maximum speed!");
      digitalWrite(LED_BUILTIN, LOW);
    }
    stopper = now;
  }
}


void FillLEDsFromPaletteColors(long colorShift) {
  for (int i = 0; i < NUM_LEDS; i++) {
    int index = i;
    if (reversed) {
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

/* ********************************************************** */
// Helper function to get optional parameter from JSON
template<typename T>
T getOptionalParam(JsonObject jsonObject, const char* paramName, T defaultValue) {
  if (jsonObject.containsKey(paramName)) {
    return jsonObject[paramName].as<T>();
  }
  return defaultValue;
}
// WIFI CONFIG
void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);  //Connect to the WiFi network

  server.on("/v1/animation", HTTP_POST, []() {
    server.sendHeader("Connection", "Keep-Alive");  // Enable Keep-Alive header

    // Parse the incoming JSON data
    DynamicJsonDocument jsonDocument(400);  // Adjust size as needed
    DeserializationError error = deserializeJson(jsonDocument, server.arg("plain"));

    if (error) {
      String response = "{\"error\": \"Error parsing JSON data\"}";
      Serial.println(response);
      server.send(400, "application/json", response);
      return;
    }

    JsonObject jsonObject = jsonDocument.as<JsonObject>();
    // serializeJson(jsonObject, Serial);
    // Serial.println();
    handleCommand(jsonObject);

    // Do something with the data, e.g., send a response
    String response = "{\"message\": \"Received JSON data successfully\"}";
    server.send(200, "application/json", response);
  });
}

void handleCommand(JsonObject jsonObject) {
  float bpmNew = getOptionalParam(jsonObject, "BPM", -1.0);
  if (bpmNew != -1 && bpmNew != BPM) {
    // Serial.print("Updated BPM: ");
    // Serial.println(bpmNew);
    BPM = bpmNew;
    delayMillis = calculateDelayMillis();
  }
  int brightnessNew = getOptionalParam(jsonObject, "BRIGHTNESS", -1);
  if (brightnessNew != -1 && brightnessNew > 0 && brightnessNew != BRIGHTNESS) {
    // Serial.print("Updated BRIGHTNESS: ");
    // Serial.println(brightnessNew);
    BRIGHTNESS = brightnessNew;
    FastLED.setBrightness(BRIGHTNESS);
  }

  int reversedNew = getOptionalParam(jsonObject, "REVERSED", -1);
  if (reversedNew != -1 && reversed != (reversedNew == 1)) {
    reversed = reversedNew == 1;
    // Serial.print("Updated reverse: ");
    // Serial.println(reversed);
  }

  int resolutionNew = getOptionalParam(jsonObject, "RESOLUTION", -1);
  if (resolutionNew != -1 && resolutionNew > 0 && resolutionNew != RESOLUTION) {
    // Serial.print("Updated resolutionNew: ");
    // Serial.println(resolutionNew);
    RESOLUTION = resolutionNew;
  }

  float wavelengthNew = getOptionalParam(jsonObject, "WAVE_LENGTH_SCALE", -1.0);
  if (wavelengthNew != -1.0 && wavelengthNew > 0 && wavelengthNew != WAVE_LENGTH_SCALE) {
    // Serial.print("Updated WAVELENGTH: ");
    // Serial.println(wavelengthNew);
    WAVE_LENGTH_SCALE = wavelengthNew;
  }
}
/* ********************************************************** */


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
