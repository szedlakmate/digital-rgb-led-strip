#include <FastLED.h>

#define LED_PIN     5
#define NUM_LEDS    51
#define BRIGHTNESS  150
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 80
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

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void setup() {
    // ESP2886 related:
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    
    delay( 500 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
   
    // TODO: Add serial communication
    
    currentBlending = LINEARBLEND;

    // OceanColors_p, CloudColors_p, LavaColors_p, HeatColors_p, ForestColors_p, and PartyColors_p., RainbowColors_p, RainbowStripeColors_p 
    // currentPalette = RainbowStripeColors_p ;
    // 0 x RR-GG-BB
    
    // AntiqueWhite:FAEBD7,Aqua:00FFFF,Aquamarine:7FFFD4,Azure:F0FFFF,Beige:F5F5DC,Bisque:FFE4C4,
    // Black:000000, BlanchedAlmond:FFEBCD, Blue:0000FF,BlueViolet:8A2BE2,Brown:A52A2A,BurlyWood:DEB887
    // CadetBlue:5F9EA0,Chartreuse:7FFF00,Chocolate:D2691E,Coral:FF7F50,CornflowerBlue:6495ED
    // Cornsilk:FFF8DC,Crimson:DC143C,Cyan:00FFFF,DarkBlue:00008B,DarkCyan:008B8B,DarkGoldenRod:B8860B
    // DarkGray:A9A9A9,DarkGreen:006400,DarkKhaki:BDB76B,DarkMagenta:8B008B,DarkOliveGreen:556B2F
    // DarkOrange:FF8C00,DarkOrchid:9932CC,DarkRed:8B0000,DarkSalmon:E9967A,DarkSeaGreen:8FBC8F
    // DarkSlateBlue:483D8B,DarkSlateGray:2F4F4F,DarkTurquoise:00CED1,DarkViolet:9400D3,DeepPink:FF1493
    // DeepSkyBlue:00BFFF,DimGray:696969,DodgerBlue:1E90FF,FireBrick:B22222,FloralWhite:FFFAF0
    // ForestGreen:228B22,Fuchsia:FF00FF,Gainsboro:DCDCDC,GhostWhite:F8F8FF,Gold:FFD700
    // GoldenRod:DAA520,Gray:808080,Green:008000,GreenYellow:ADFF2F,HoneyDew:F0FFF0,HotPink:FF69B4
    // IndianRed:CD5C5C,Indigo:4B0082,Ivory:FFFFF0,Khaki:F0E68C,Lavender:E6E6FA,LavenderBlush:FFF0F5
    // LawnGreen:7CFC00,LemonChiffon:FFFACD,LightBlue:ADD8E6,LightCoral:F08080,LightCyan:E0FFFF
    // LightGoldenRodYellow:FAFAD2,LightGray:D3D3D3,LightGreen:90EE90,LightPink:FFB6C1,LightSalmon:FFA07A
    // LightSeaGreen:20B2AA,LightSkyBlue:87CEFA,LightSlateGray:778899,LightSteelBlue:B0C4DE,
    // LightYellow:FFFFE0,Lime:00FF00,LimeGreen:32CD32,Linen:FAF0E6,Magenta:FF00FF,Maroon:800000
    // MediumAquaMarine:66CDAA,MediumBlue:0000CD,MediumOrchid:BA55D3,MediumPurple:9370DB
    // MediumSeaGreen:3CB371,MediumSlateBlue:7B68EE,MediumSpringGreen:00FA9A,MediumTurquoise:48D1CC
    // MediumVioletRed:C71585,MidnightBlue:191970,MintCream:F5FFFA,Navy:000080,Olive:808000
    // OliveDrab:6B8E23,Orange:FFA500,OrangeRed:FF4500,Orchid:DA70D6,PaleGreen:98FB98,Peru:CD853F
    // Yellow:FFFF00,White:FFFFFF,Cyan:00FFFF
    fill_solid( currentPalette, 16, 0x000033);
    currentPalette[0] = 0x000000;
    
}


void loop()
{
    // ESP2886 related:
    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
  
    // ChangePalettePeriodically();
    
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */

    
    
    FillLEDsFromPaletteColors(startIndex); //startIndex
    
    //color = Blue
    // SetColorPalette(CRGB::Red);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}



void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
        if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}
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

