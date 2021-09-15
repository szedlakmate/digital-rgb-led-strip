#include <FastLED.h>

#define LED_PIN     53    // arduino connection
#define NUM_LEDS    300   // total num of leds on the full strip
#define SECTIONS    4.0   // the num of sections the strip was broken
#define BRIGHTNESS  100   // max: 250
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

// Inject ultrasound distance measurement from https://github.com/szedlakmate/arduino-ultrasound-distance-measurement

// Pins
const int TRIG_PIN = 7;   // Ultrasound device 
const int ECHO_PIN = 8;   // Ultrasound device  

const int numReadings = 3;
const float threshold = 100;     // ?unit

float readings[numReadings];    // the readings from the analog input
int readIndex = 0;              // the index of the current reading
float sum = 0;                  // sum of readings
float average = 0;              // the average
float prevavg = 0;              // previous average
int count = 0;                  // counting readings' elements
int dropcount = 0;              // counting the measurement anomalies

int appliedBrightness = BRIGHTNESS;

// Anything over 50 cm (2900 us(?) pulse) is "out of range"
const unsigned int MAX_DIST = 3300;

// *** End of injection *********************************

// Wave per miniute. 1 means it takes 60 sec to flow through each LEDs
// Max BPM is ~10 for 300 LEDS (RESOLUTION=1) 
// Max BPM*RESOLUTION is ~3 for 300 LEDS (RESOLUTION>1) 
#define BPM 6.0

// Advised max (sub) RESOLUTION is ~3, Min 1, Default 1
// *** Set to 1 to reach FAST animation
#define RESOLUTION 100  // Ultrasound needs: BPM * RESOLUTION >= 240

// Scales the wave's length. >1.0 means overlays the stripe. Default: 1.0
static int WAVE_LENGTH = (NUM_LEDS / SECTIONS);


CRGBPalette16 currentPalette;
CRGBPalette16 newPalette;
TBlendType    currentBlending;

static long looper = 0;

// Determine delay time based on BPM and RESOLUTION
static int delayDelta = 60000.0 / ((float)RESOLUTION * BPM) ;
long stopper = 0;


const TProgmemPalette16 mySunsetBlue PROGMEM =
{
    CRGB(  0,  0, 50),
    CRGB( 11, 11, 61),
    CRGB( 22, 22, 72),
    CRGB( 31, 31, 81),

    CRGB( 39, 39, 89),
    CRGB( 45, 45, 95),
    CRGB( 49, 49, 99),
    CRGB( 50, 50,100),
  
    CRGB( 50, 50,100),
    CRGB( 49, 49, 99),
    CRGB( 45, 45, 95),
    CRGB( 39, 29, 89),
    
    CRGB( 31, 31, 81),
    CRGB( 22, 22, 72),
    CRGB( 11, 11, 61),
    CRGB(  0,  0, 50),
};

const TProgmemPalette16 mySunsetPurple PROGMEM =
{
    CRGB( 30,  0, 50),
    CRGB( 39,  4, 60),
    CRGB( 47,  9, 67),
    CRGB( 55, 12, 70),

    CRGB( 61, 16, 70),
    CRGB( 66, 18,112),
    CRGB( 69, 19,109),
    CRGB( 70, 20, 50),

    CRGB( 70, 20,100),
    CRGB( 69, 19,100),
    CRGB( 66, 18,112),
    CRGB( 61, 16,109),
    
    CRGB( 55, 12,101),
    CRGB( 47,  9, 88),
    CRGB( 39,  4, 71),
    CRGB( 30,  0, 50),
};

const TProgmemPalette16 mySunsetPeach PROGMEM =
{
    0x721c93,
    0x892a87,
    0x9f387c,
    0xb24572,
  
    0xc24f6a,
    0xce5763,
    0xd65c5f,
    0xd85e5e,
    
    0xd85e5e,
    0xd65c5f,
    0xce5763,
    0xc24f6a,

    0xb24572,
    0x9f387c,
    0x892a87,
    0x721c93,
};

const TProgmemPalette16 mySunsetGreen PROGMEM =
{
    CRGB(  0, 40,  0),
    CRGB(  0, 31,  9),
    CRGB(  0, 23, 17),
    CRGB(  0, 15, 25),

    CRGB(  0,  9, 31),
    CRGB(  0,  4, 36),
    CRGB(  0,  1, 39),
    CRGB(  0,  0, 40),
  
    CRGB(  0,  0, 40),
    CRGB(  0,  1, 39),
    CRGB(  0,  4, 36),
    CRGB(  0,  9, 31),
    
    CRGB(  0, 15, 25),
    CRGB(  0, 23, 17),
    CRGB(  0, 31,  9),
    CRGB(  0, 40,  0),
};

const TProgmemPalette16 DebugPalette PROGMEM =
{
   0x0000ff,
   0x000000,
   0x000000,
   0x000000,

   0x00ff00,
   0x000000,
   0x000000,
   0x000000,

   0xff0000,
   0x000000,
   0x000000,
   0x000000,

   0xffffff,
   0x000000,
   0x000000,
   0x000000,
};


void setup() {
    delay(500); // power-up safety delay
    
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(appliedBrightness);
    
    currentBlending = LINEARBLEND;

    // OceanColors_p, CloudColors_p, LavaColors_p, HeatColors_p, ForestColors_p, and PartyColors_p., RainbowColors_p, RainbowStripeColors_p 

    // mySunsetBlue, mySunsetPurple, mySunsetPeach, mySunsetGreen
     currentPalette = RainbowColors_p;
//      currentPalette = DebugPalette;
     newPalette = RainbowStripeColors_p;
//      newPalette = DebugPalette;
//     fill_solid( newPalette, 16, CRGB::Red);

//      newPalette =  currentPalette;

     // Initialize LED colors
     FillLEDsFromPaletteColors(looper);


    // Ultrasound stuff ***********
   
    // The Trigger pin will tell the sensor to range find
    pinMode(TRIG_PIN, OUTPUT);
    digitalWrite(TRIG_PIN, LOW);
  
    // Initialize built-in LED - Blinking signs active data transfer
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);           // Set built-in LED to default: Off  
  
    // We'll use the serial monitor to view the sensor output
    Serial.begin(9600);
    for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;  
    }
}


void loop()
{
    FillLEDsFromPaletteColors(looper);

    // SetColorPalette(CRGB::Red);

    // Ultrasound measurement
    measure();
//    Serial.println(average);
    if (average > 0) {
      // Smoothen brightness change
      appliedBrightness = (appliedBrightness + min(average, 250))/2;
      FastLED.setBrightness( appliedBrightness );
    }
    
    FastLED.show();
    looper += 1;

    if (looper > RESOLUTION) {
      switchPalettes();
    }
    

    // Determine accurate sleep time
    long timer = millis();
    FastLED.delay(max(delayDelta - timer + stopper, 0));
//    Serial.print("  delayDelta - timer + stopper: ");
//    Serial.println(delayDelta - timer + stopper);
    stopper = timer;
}

void switchPalettes() {
  CRGBPalette16 tempPalette;
  tempPalette = currentPalette;
  currentPalette = newPalette;
  newPalette = tempPalette;
  looper = 0;
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
        for (int sectionIndex = 0; sectionIndex < SECTIONS; sectionIndex++) {
           for( int i = 0; i < WAVE_LENGTH; i++) {

              leds[i+sectionIndex*WAVE_LENGTH] =  blend(
                  ColorFromPalette(currentPalette, (i)*255/WAVE_LENGTH, appliedBrightness, currentBlending),
                  ColorFromPalette(newPalette,     (i)*255/WAVE_LENGTH, appliedBrightness, currentBlending),
                  (float)looper*255.0/(float)RESOLUTION  // 0 - 255
              );
              
//            leds[i+sectionIndex*WAVE_LENGTH] = ColorFromPalette(currentPalette, (i)*255/WAVE_LENGTH, appliedBrightness, currentBlending);
          }
        }
};

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


// Ultrasonic mess *****************************
void reset(){
  // If too many highly different measurement were recorded, 
  // the program drops all saved data and reset the variables.
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0; 
  }
  count = 0;
  readIndex = 0;
  sum = 0;
  average = 0;
  dropcount = 0;
  prevavg = 0;
}


void measure() {
   unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float mm;

  // Hold the trigger pin high for at least 10 us
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Wait for pulse on echo pin
  while ( digitalRead(ECHO_PIN) == 0 );

  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while ( digitalRead(ECHO_PIN) == 1);
  t2 = micros();
  pulse_width = t2 - t1;

                   // Calculate distance in millimeters. The constants
                   // are found in the datasheet, and calculated from the assumed speed 
                   // of sound in air at sea level (~340 m/s).
                   // mm = round(pulse_width / 0.580)/10; // Original calculation method
  mm = round(pulse_width *1.716+6.76)/10; // Calibrated method
  // Print out results
  if ( pulse_width > MAX_DIST ) {
//     Serial.println("Out of range");
  } else {
      if ((count < numReadings) or ((mm > average - threshold) and (mm < average + threshold))) {
        if (count < numReadings) {
          count += 1;
          } 
         // drop old data
        sum = sum - readings[readIndex];
        // add new data
        readings[readIndex] = mm;
        sum = sum + readings[readIndex];
        readIndex = readIndex + 1;
        if (readIndex >= numReadings) {
          // ...wrap around to the beginning:
          readIndex = 0;
          if (dropcount > 0) {
            dropcount -= 1;
          }
        }
        // Calcualting the average
        average = sum / numReadings;
        if ((count == numReadings) and (average < prevavg + threshold)
        and (average > prevavg - threshold)) {
//          Serial.print(average);               
//          Serial.println(",");
        }
        prevavg = average;
      } else {
        if (count == numReadings){
          dropcount += 1;
          if (dropcount == 3){
            reset();
          }
        }
      }
  }
}
