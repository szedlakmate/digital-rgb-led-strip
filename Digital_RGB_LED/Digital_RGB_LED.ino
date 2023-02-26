#include <FastLED.h>

// Pins
#define LED_PIN 53  // arduino connection
#define BPM_INPUT_PIN 1

#define NUM_LEDS 300    // total num of leds on the full strip
#define SECTIONS 4.0    // the num of sections the strip was broken
#define BRIGHTNESS 250  // max: 250
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

// Ultrasound distance measurement from https://github.com/szedlakmate/arduino-ultrasound-distance-measurement


// Wave per miniute. 1 means it takes 60 sec to flow through each LEDs
// Max BPM is ~10 for 300 LEDS (RESOLUTION=1)
// Max BPM*RESOLUTION is ~3 for 300 LEDS (RESOLUTION>1)
float bpm = 0.1;
#define BPM_MAX 15.0
int bpmPotmeterValue = 10;  // Initial value

#define TRANSITION_SMOOTHNESS_STEPS 3  // Defines how much measurements needed to be considered to apply change

// Advised max (sub) RESOLUTION is ~3, Min 1, Default 1
// *** Set to 1 to reach FAST animation
#define RESOLUTION 200  // Ultrasound needs: BPM * RESOLUTION >= 240

// Scales the wave's length. >1.0 means overlays the stripe. Default: 1.0
static int WAVE_LENGTH = (NUM_LEDS / SECTIONS);

int cycle = -1;


CRGBPalette16 currentPalette;
CRGBPalette16 newPalette;
TBlendType currentBlending;

static long looper = 0;

int determineDelayMillis() {
  return (60000.0 * 2.0) / ((float)NUM_LEDS * max(bpm, 0.00000000001));
}

// Determine delay time based on BPM
static int delayMillis = determineDelayMillis();
long stopper = millis();


// Palettes in order
const TProgmemPalette16 mySunsetPeach PROGMEM = {
  0x960096,
  0xAD1674,
  0xC32B54,
  0xD73E38,
  0xE84E20,
  0xF45A0E,
  0xFC6103,
  0xFF6400,
  0xFF6400,
  0xFC6103,
  0xF45A0E,
  0xE84E20,
  0xD73E38,
  0xC32B54,
  0xAD1674,
  0x960096,
};

const TProgmemPalette16 mySunsetPurple PROGMEM = {
  0x6400FF,
  0x7A06E0,
  0x8F0DC4,
  0xA212AA,
  0xB21795,
  0xBE1B85,
  0xC51D7B,
  0xC81E78,
  0xC81E78,
  0xC51D7B,
  0xBE1B85,
  0xB21795,
  0xA212AA,
  0x8F0DC4,
  0x7A06E0,
  0x6400FF,
};

const TProgmemPalette16 mySunsetBlue PROGMEM = {
  0x0000FF,
  0x1A04E7,
  0x3408D1,
  0x4A0CBD,
  0x5D0FAC,
  0x6C12A0,
  0x741398,
  0x781496,
  0x781496,
  0x741398,
  0x6C12A0,
  0x5D0FAC,
  0x4A0CBD,
  0x3408D1,
  0x1A04E7,
  0x0000FF,
};

const TProgmemPalette16 mySunsetGreen PROGMEM = {
  0x005000,
  0x003E0A,
  0x002D14,
  0x001E1D,
  0x001125,
  0x00072B,
  0x00022E,
  0x000030,
  0x000030,
  0x00022E,
  0x00072B,
  0x001125,
  0x001E1D,
  0x002D14,
  0x003E0A,
  0x005000,
};

CRGBPalette16 temp;

void setup() {
  delay(500);  // power-up safety delay

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  currentBlending = LINEARBLEND;

  // OceanColors_p, CloudColors_p, LavaColors_p, HeatColors_p, ForestColors_p, and PartyColors_p., RainbowColors_p, RainbowStripeColors_p

  // mySunsetBlue, mySunsetPurple, mySunsetPeach, mySunsetGreen
  newPalette = mySunsetPeach;

  switchPalettes();

  // Initialize LED colors
  FillLEDsFromPaletteColors(looper);

  // Initialize built-in LED - Blinking signs active data transfer
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);  // Set built-in LED to default: Off

  // We'll use the serial monitor to view the sensor output
  Serial.begin(9600);
}


void loop() {
  looper += 1;
  if (looper > RESOLUTION) {
    switchPalettes();
    looper = 0;
  }

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

void switchPalettes() {
  currentPalette = newPalette;
  cycle = (cycle + 1) % 4;

  // Determines the order of the animation
  // mySunsetBlue, mySunsetPurple, mySunsetPeach, mySunsetGreen
  if (cycle == 0) {
    newPalette = mySunsetPurple;
  } else if (cycle == 1) {
    newPalette = mySunsetBlue;
  } else if (cycle == 2) {
    newPalette = mySunsetGreen;
  } else if (cycle == 3) {
    newPalette = mySunsetPeach;
  }

  looper = 0;
}

void waitAndMeasure(int waitMillis) {
  int samplingCount = max(round(waitMillis / 300.0), 1);

  if (samplingCount > 10) {
    Serial.print("!! samplingCount:   ");
    Serial.println(samplingCount);
  }

  for (int i = 0; i < samplingCount; i += 1) {
    FastLED.delay(waitMillis / samplingCount);
    measureAndApplyState();

    if (bpm == 0.0) {
      i = 0;
    }

    if (determineDelayMillis() < waitMillis * i / samplingCount && bpm > 0.0) {
      break;
    }
  }
}

void measureAndApplyState() {
  setBpmByPotmeter(analogRead(BPM_INPUT_PIN));
}

float determineBPM(long potMeterValue) {
  return min(max((potMeterValue / 650.0) * BPM_MAX, 0), BPM_MAX);
}

bool shouldApplyMeasurement(int originalMeasurement, int newMeasurement) {
  // diff is larger than 2 OR (diff is lessOrEqual 2 AND newMeasurement is 0)
  int tolerance = 4;
  return (abs(originalMeasurement - newMeasurement) > tolerance || (abs(originalMeasurement - newMeasurement) <= tolerance && newMeasurement == 0));
}

void setBpmByPotmeter(float potMeterValue) {
  if (shouldApplyMeasurement(bpmPotmeterValue, potMeterValue)) {
    bpm += (determineBPM(potMeterValue) - bpm) / TRANSITION_SMOOTHNESS_STEPS;
    delayMillis = determineDelayMillis();
    bpmPotmeterValue = potMeterValue;
  }

  Serial.print("  BPM: ");
  Serial.print(bpm);
  Serial.print("  potMeterValue: ");
  Serial.println(potMeterValue);
}


void FillLEDsFromPaletteColors(uint8_t colorIndex) {
  for (int sectionIndex = 0; sectionIndex < SECTIONS; sectionIndex++) {
    for (int i = 0; i < WAVE_LENGTH; i++) {

      leds[i + sectionIndex * WAVE_LENGTH] = blend(
        ColorFromPalette(currentPalette, (i)*255 / WAVE_LENGTH, BRIGHTNESS, currentBlending),
        ColorFromPalette(newPalette, (i)*255 / WAVE_LENGTH, BRIGHTNESS, currentBlending),
        (float)looper * 255.0 / (float)RESOLUTION  // 0 - 255
      );

      //            leds[i+sectionIndex*WAVE_LENGTH] = ColorFromPalette(currentPalette, (i)*255/WAVE_LENGTH, BRIGHTNESS, currentBlending);
    }
  }
};
