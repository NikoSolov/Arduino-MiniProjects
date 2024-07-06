/*************************************************************************
 * 
 * VGA output with Arduino (white and black colors).
 * This is a free software with NO WARRANTY.
 * https://simple-circuit.com/
 *
 ************************************************************************/
 
#include <Adafruit_GFX.h>  // include Adafruit graphics library
#include "VGA.h"           // include VGA library
 
// initialize the VGA library
VGA display = VGA();
 
void setup(void) {
  // initialize the VGA display
  display.begin();
 
  display.delay(5000);     // wait 5 seconds
  display.clearDisplay();  // clear the screen buffer
 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Hello, world!");
  display.setTextColor(BLACK, WHITE);
  display.println(3.141592);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("0x");
  display.println(0xDEADBEEF, HEX);
 
  display.setCursor(0, 40);
  display.setTextSize(1);
  display.print("Arduino VGA Example");

  display.drawPixel(0,0,WHITE);
  display.drawPixel(80,0,WHITE);
  display.drawPixel(0,40,WHITE);
  display.drawPixel(80,40,WHITE);

}
 
void loop() {
  ;
}
 
// end of code.
