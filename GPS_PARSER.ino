#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(7, 6, 5, 4, 3);
/*
  D0 - D6
  D1 - D7
  RST - D4
  DC - D5
*/
//you can use any display
#include <SoftwareSerial.h>
SoftwareSerial GPS(2, 1);
// TX - D2; RX - D1(nevermind)

char buffer; // buffer
String str, rmc[11], gga[14], drob, compass;
int acc(0);


void setup() {
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.cp437(true);
  GPS.begin(9600);
}

void loop() {
  //GET
  while (true) {
    while (GPS.available())
    {
      buffer = GPS.read();
      str += buffer;
    }

    if (str[str.length() - 1] == '\n') {
      break;
    }
  }
  //Parsing
  if (str.indexOf("$GPRMC") != -1) {
    acc = str.indexOf("$GPRMC");
    for (int i(0); i < 11; i++) {
      rmc[i] = str.substring(acc, acc + str.substring(acc, str.length()).indexOf(','));
      acc += str.substring(acc, str.length()).indexOf(',');
      acc++;
    }
  }
  else if (str.indexOf("$GPGGA") != -1) {
    acc = str.indexOf("$GPGGA");
    for (int i(0); i < 11; i++) {
      gga[i] = str.substring(acc, acc + str.substring(acc, str.length()).indexOf(','));
      acc += str.substring(acc, str.length()).indexOf(',');
      acc++;
    }
  }
  if (rmc[8].toInt() > 22.5 && rmc[8].toInt() < 67.5) {
    compass = "NE";
  }
  if (rmc[8].toInt() > 67.5 && rmc[8].toInt() < 112.5) {
    compass = "E";
  }
  if (rmc[8].toInt() > 112.5 && rmc[8].toInt() < 157.5) {
    compass = "SE";
  }
  if (rmc[8].toInt() > 157.5 && rmc[8].toInt() < 202.5) {
    compass = "S";
  }
  if (rmc[8].toInt() > 202.5 && rmc[8].toInt() < 247.5) {
    compass = "SW";
  }
  if (rmc[8].toInt() > 247.5 && rmc[8].toInt() < 292.5) {
    compass = "W";
  }
  if (rmc[8].toInt() > 292.5 && rmc[8].toInt() < 337.5) {
    compass = "NW";
  }
  if (rmc[8].toInt() > 337.5 or rmc[8].toInt() < 22.5) {
    compass = "N";
  }
  //Display
  display.clearDisplay();
  //Date
  display.setCursor(0, 0);
  int hour = rmc[1].substring(0, 2).toInt() + 3;
  if (hour > 23) {
    hour = hour - 24;
  }
  display.print(hour);
  display.print(":" + rmc[1].substring(2, 4) + ":" + rmc[1].substring(4, 6) + " " + rmc[2] + " " + rmc[9].substring(0, 2) + "." + rmc[9].substring(2, 4) + ".20" + rmc[9].substring(4, 6));
  //Coords

  display.setCursor(0, 8);
  drob = (rmc[3].substring(2, 4).toInt() * 10000 + rmc[3].substring(5, 9).toInt()) / 3 * 5;
  display.print(rmc[3].substring(0, 2) + "." + drob + char(0xf8) + rmc[4] + " Sats: " + gga[7]);

  display.setCursor(0, 16);
  drob = (rmc[5].substring(3, 5).toInt() * 10000 + rmc[5].substring(6, 10).toInt()) / 3 * 5;
  display.print(rmc[5].substring(0, 3) + "." + drob + char(0xf8) + rmc[6] + " " + compass + " " + round(rmc[8].toInt()) + char(0xf8));

  display.setCursor(0, 24);
  display.print(gga[9] + " " + gga[10] + " Spd: " + round(rmc[7].toInt() * 1.852) + " km/h ");

  display.display();
  str = "";
  acc = 0;
}
