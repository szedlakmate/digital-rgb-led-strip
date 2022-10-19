#include <FastLED.h>
#include <Arduino.h>
// Download from here: https://www.instructables.com/Tutorial-How-to-4-Digit-Display-Interface-With-Ard/
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

#define LED_PIN     12

#define NUM_LEDS    300
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

TM1637Display display(CLK, DIO);

#define BRIGHTNESS_INPUT_PIN  1
#define WAVE_LENGTH_INPUT_PIN  3
#define BPM_INPUT_PIN  2

#define FILTER_FACTOR 3

// Advised max (sub) RESOLUTION is ~3, Min 1, Default 1
// *** Set to 1 to reach FAST animation
#define RESOLUTION 1

// Max BPM*RESOLUTION is ~15 for 300 LEDS
#define BPM_MAX 50.0

// Scales the wave's length. >1.0 means overlays the stripe. Default: 1.0
#define WAVE_LENGTH_SCALE_MAX  2.0

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


CRGBPalette16 currentPalette;
TBlendType    currentBlending;

// These will be overridden in the setup block
float WAVE_LENGTH_SCALE = 0.0;
float BPM = 0.0;
int BRIGHTNESS = 0;  // max: 250;

static long startIndex = 0;
static bool reversed = true;

// Determine delay time based on BPM and RESOLUTION
static int delayDelta = 60000.0 / ((float)NUM_LEDS * (float)RESOLUTION * BPM) ;
long stopper = 0;

void setup() {
    delay( 500 ); // power-up safety delay
    
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip ); // TypicalLEDStrip
    FastLED.setBrightness(  BRIGHTNESS );
    
    currentBlending = LINEARBLEND;

    // OceanColors_p, CloudColors_p, LavaColors_p, HeatColors_p, ForestColors_p, and PartyColors_p., RainbowColors_p, RainbowStripeColors_p 
     currentPalette = RainbowColors_p ;

     // Initialize LED colors
     FillLEDsFromPaletteColors(startIndex, false);

     // Turning ON Serial communication may slow down the animation
     Serial.begin(9600); 

     applyConfiguration();

     // Correcting initial zero values 
     WAVE_LENGTH_SCALE *= FILTER_FACTOR;
     BPM *= FILTER_FACTOR;
     BRIGHTNESS *= FILTER_FACTOR;

     display.setBrightness(0x08);
}


void loop()
{
    applyConfiguration();
    // Buttons require constant refreshing
    FillLEDsFromPaletteColors(startIndex, false);
    
    // SetColorPalette(CRGB::Red);
    
    FastLED.show();
    startIndex = startIndex + 1;

    // Determine accurate sleep time
    long timer = millis();
    int sleepTime = max(delayDelta - timer + stopper, 0);
    FastLED.delay(sleepTime);
    Serial.print("  delayDelta - timer + stopper: ");
    Serial.println(sleepTime);
    stopper = timer;

    showBPM(sleepTime>0);
}

float showBPM(bool bpmIsValid) {
  // Set segment display
  uint8_t data[] = { 0x00, 0x80, 0x00, 0x00 };
  
  // Selectively set different digits
  data[3] += display.encodeDigit((int) (BPM * 100) % 10);
  
  if (BPM > 0.1) {
    data[2] += display.encodeDigit((int) (BPM * 10) % 10);
  }
  if (BPM > 1) {
    data[1] += display.encodeDigit((int) BPM  % 10);
  }
  if (BPM > 10) {
    data[0] += display.encodeDigit((int)( BPM / 10) % 10);
  }


    
  if (!bpmIsValid) {
    data[0] = 0x49;
  }
 
  display.setSegments(data);
}

void applyConfiguration() {
    setBrightnessByPotmeter(analogRead(BRIGHTNESS_INPUT_PIN));
    setBPM(analogRead(BPM_INPUT_PIN));
    setWaveLengthByPotmeter(analogRead(WAVE_LENGTH_INPUT_PIN));
}

int determineBrightness(long potMeterValue) {
  return potMeterValue * 250L / 1023L;
}

float determineBPM(long potMeterValue) {
  return max(potMeterValue * BPM_MAX / 1023.0, 0.01);
}

float determineWaveLength(long potMeterValue) {
  float rawWaveLength = max(potMeterValue * WAVE_LENGTH_SCALE_MAX / 1023.0, 0.05);
  // Filter noise by rounding scaled value. Helps at very short wave lengths
  return  ((float) round(rawWaveLength * 50)) / 50.0;
}

void setBrightnessByPotmeter(long potMeterValue) {
    //BRIGHTNESS += (determineBrightness(potMeterValue) - BRIGHTNESS) / FILTER_FACTOR;
     BRIGHTNESS = 110;
//    Serial.print("  BRIGHTNESS: ");
//    Serial.print(BRIGHTNESS);
    FastLED.setBrightness(  BRIGHTNESS );
}

void setBPM(float potMeterValue) {
    // BPM += (determineBPM(potMeterValue) - BPM) / FILTER_FACTOR;
    BPM = 10;
    Serial.print("  BPM: ");
    Serial.print(BPM);
    delayDelta = 60000.0 / ((float)NUM_LEDS * (float)RESOLUTION * BPM); // correction of calculation time loss
}

void setWaveLengthByPotmeter(float potMeterValue) {
    WAVE_LENGTH_SCALE += (determineWaveLength(potMeterValue) - WAVE_LENGTH_SCALE) / FILTER_FACTOR;
    Serial.print("  potMeterValue: ");
    Serial.println(potMeterValue);

      Serial.print("  WAVE_LENGTH_SCALE: ");
    Serial.println(WAVE_LENGTH_SCALE);
}

void FillLEDsFromPaletteColors(long colorShift, bool shiftOnly)
{
    for( int i = 0; i < NUM_LEDS; i++) {
      int index = i;
      if (reversed) {
        index = NUM_LEDS - i -1;
      }

      // Speed up calculation when possible by shifting the leds
      if (shiftOnly && RESOLUTION == 1) {
        if (reversed) {
          if (index != 0) {
            leds[index] = leds[index - 1];
            continue;
          }
        } else {
          if (index != NUM_LEDS-1) {
             leds[index] = leds[index + 1];
             continue;
          }
        }
      }
      
      long colorIndex = ((long) i * (long) 256)/ (float) WAVE_LENGTH_SCALE / ((long)NUM_LEDS) + colorShift / (long) RESOLUTION;
     
      if (currentBlending == NOBLEND || RESOLUTION == 1)
      {
         leds[index] = ColorFromPalette( currentPalette, colorIndex + colorShift / (long) RESOLUTION, BRIGHTNESS, currentBlending);
      } 
      else {
        // Apply smoothing
         leds[index] = blend(
            ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending),
            ColorFromPalette( currentPalette, colorIndex + 1, BRIGHTNESS, currentBlending),
            (float)(colorShift % RESOLUTION)*255.0/(float)RESOLUTION);
      }
    }
//     Serial.println(colorShift / (long) RESOLUTION);
}


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
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
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
