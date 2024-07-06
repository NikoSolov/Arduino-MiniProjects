#include <FastLED.h>
//460
#define NUM_LEDS 460
#define LED_PIN 3
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define len 300
#define BRIGHTNESS  64
#define UPDATES_PER_SECOND 100

CRGB leds[NUM_LEDS];

unsigned char led_val[ len ] = {0};
//const byte PROGMEM led_val[460]={0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 21, 21, 22, 22, 23, 23, 24, 24, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30, 31, 31, 32, 32, 33, 33, 34, 34, 35, 36, 36, 37, 37, 38, 38, 39, 39, 40, 41, 41, 42, 42, 43, 43, 44, 44, 45, 46, 46, 47, 47, 48, 48, 49, 49, 50, 51, 51, 52, 52, 53, 53, 54, 54, 55, 55, 56, 57, 57, 58, 58, 59, 59, 60, 60, 61, 62, 62, 63, 63, 64, 64, 65, 65, 66, 67, 67, 68, 68, 69, 69, 70, 70, 71, 72, 72, 73, 73, 74, 74, 75, 75, 76, 77, 77, 78, 78, 79, 79, 80, 80, 81, 82, 82, 83, 83, 84, 84, 85, 85, 86, 87, 87, 88, 88, 89, 89, 90, 90, 91, 92, 92, 93, 93, 94, 94, 95, 95, 96, 97, 97, 98, 98, 99, 99, 100, 100, 101, 102, 102, 103, 103, 104, 104, 105, 105, 106, 106, 107, 108, 108, 109, 109, 110, 110, 111, 111, 112, 113, 113, 114, 114, 115, 115, 116, 116, 117, 118, 118, 119, 119, 120, 120, 121, 121, 122, 123, 123, 124, 124, 125, 125, 126, 126, 127, 128, 128, 129, 129, 130, 130, 131, 131, 132, 133, 133, 134, 134, 135, 135, 136, 136, 137, 138, 138, 139, 139, 140, 140, 141, 141, 142, 143, 143, 144, 144, 145, 145, 146, 146, 147, 148, 148, 149, 149, 150, 150, 151, 151, 152, 153, 153, 154, 154, 155, 155, 156, 156, 157, 157, 158, 159, 159, 160, 160, 161, 161, 162, 162, 163, 164, 164, 165, 165, 166, 166, 167, 167, 168, 169, 169, 170, 170, 171, 171, 172, 172, 173, 174, 174, 175, 175, 176, 176, 177, 177, 178, 179, 179, 180, 180, 181, 181, 182, 182, 183, 184, 184, 185, 185, 186, 186, 187, 187, 188, 189, 189, 190, 190, 191, 191, 192, 192, 193, 194, 194, 195, 195, 196, 196, 197, 197, 198, 199, 199, 200, 200, 201, 201, 202, 202, 203, 204, 204, 205, 205, 206, 206, 207, 207, 208, 208, 209, 210, 210, 211, 211, 212, 212, 213, 213, 214, 215, 215, 216, 216, 217, 217, 218, 218, 219, 220, 220, 221, 221, 222, 222, 223, 223, 224, 225, 225, 226, 226, 227, 227, 228, 228, 229, 230, 230, 231, 231, 232, 232, 233, 233, 234, 235, 235, 236, 236, 237, 237, 238, 238, 239, 240, 240, 241, 241, 242, 242, 243, 243, 244, 245, 245, 246, 246, 247, 247, 248, 248, 249, 250, 250, 251, 251, 252, 252, 253, 253, 254, 255};
unsigned char acc = 0, color = 0;
int ps = 0, step = 5, time = 0;


CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;


void setup() {
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
//  Serial.begin(9600);
//---------------------------------------------
  for (int i = 0; i < len; i++){
    led_val[i]= (unsigned char)(255 * ((float)i / (float)(len-1)));
  }
  color = random(0, 255);
//----------------------------------------------
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
}
//static uint8_t startIndex = 0;
int startIndex = 0;

void loop(){
//  Comet();
  if (time < 3){
    Comet();
    startIndex = 0;
//    Serial.println("Comet");   
  }
  else{

    ChangePalettePeriodically();
    startIndex = startIndex - 1; // motion speed 
    FillLEDsFromPaletteColors( startIndex);

  }

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND); 

}

void Comet(){
 for ( int i = NUM_LEDS-1; i > -1; i-- ){
  if ( i <= ps && i > ps-len){
   acc = led_val[i - (ps - len)-1];  
   leds[i].setHSV(i*10, random(0,255), acc);
  }
  else {
   leds[i].setHSV(0, 0, 0);
  }
 }
 ps += step;
 if (ps > NUM_LEDS + len) {ps = 0;  color = random(0, 255); time+=1;}
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 61;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
//        Serial.println("Change");
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
        if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND;}
        if( secondHand == 60)  { time = 0; }
  }
}


// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

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
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Yellow,
    CRGB::Yellow,
    CRGB::Green,
    CRGB::Green,
    CRGB::Red,
    CRGB::Red,
    CRGB::Black,
    CRGB::Black,
};

/*
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Yellow,
    CRGB::Yellow,
    CRGB::Green,
    CRGB::Green,    
    CRGB::Red,
    CRGB::Red,
    CRGB::Black,
    CRGB::Black
};
*/