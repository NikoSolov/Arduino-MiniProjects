#include <FastLED.h>
#define NUM_LEDS 460
#define DATA_PIN 3
CRGB leds[NUM_LEDS];
int i(0);
void setup() {
  Serial.begin(9600);
  delay(100);
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(  30 );

     for(int m = 0; m < NUM_LEDS; m++) {
      // Turn our current led on to white, then show the leds
      // Show the leds (only one of which is set to white, from above
      if (m%4==2){leds[m] = CRGB::Blue;}
      if (m%4==1){leds[m] = CRGB::Yellow;}
      if (m%4==0){leds[m] = CRGB::Green;}
      if (m%4==3){leds[m] = CRGB::Red;}
   }
   
}
//Simple strip: синий, желтый,зеленый, красный

void loop() {
  // put your main code here, to run repeatedly:
  if (i>NUM_LEDS){i=0;}
  
    Serial.print(i);
    Serial.print("\t");
    for (int j(0); j<9; j++){
    Serial.print(leds[i][j]);
    Serial.print("\t");}
    Serial.println();
    i++;
    FastLED.show();
}
