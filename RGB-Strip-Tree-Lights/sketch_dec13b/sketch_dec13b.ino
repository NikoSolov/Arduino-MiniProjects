#include <FastLED.h>

#define DATA_PIN     3
#define NUM_LEDS    460

CRGB leds[NUM_LEDS];


void setup() {
  // put your setup code here, to run once:
FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);  // GRB ordering is typical
FastLED.setBrightness(100);
Serial.begin(9600);
}

int ind=0, counter=0, len=460;

void loop() {
  
  for (int i=0; i<NUM_LEDS; i++){ 
    if (i<=ind and i>=ind-len+1){leds[i]=CHSV(counter, 255,  255/len*(i-(ind-len-1)));}
    else{leds[i]=CHSV(counter, 255,  0);}  
  }
  ind++;
  if (ind>NUM_LEDS+len){ind=0;}
  counter++;
 
  FastLED.show();
}
