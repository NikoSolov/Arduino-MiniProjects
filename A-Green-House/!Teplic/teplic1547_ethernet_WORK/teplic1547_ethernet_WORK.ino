#include <Ethernet.h>
#include <SPI.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>

//////////////////////////////////////////////////////

#define POLIV_TIMEOUT 200000 // 3 min
#define DHTTYPE DHT21
#define SLOSH_TIMEOUT 100000 //1,5 min delay
#define GMT 3

#define RX 0
#define TX 1


#define R1 4
#define R2 5
#define R3 6  // slosh
#define R4 7  // poliv


#define DHT_PIN 9
#define HUM_GR A0
#define LIGHT A1
/*#define WATER_LVL A2
  #define xz1 A3*/

///////////////////////////////////////////////////////

LiquidCrystal_I2C lcd(0x27, 20, 4);
DHT dht(DHT_PIN, DHTTYPE);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//////////////////////////////////////////////////////

float temp_dht = 0;
float hum_dht = 0;

int light = 0;
int hum_gr = 0;
int water_lvl = 0;

unsigned long int lastmillis = millis();
unsigned long int lcdmillis = millis();
unsigned long int polivmillis = millis();
unsigned long int sloshmillis = millis();
unsigned long int getmillis = millis();
unsigned long int start_time = millis();

int getDelay = 200;


byte R1_state = 0;
byte R2_state = 0;
byte R3_state = 0;
byte R4_state = 0;

byte fl_poliv = 0;
byte fl_light = 0;
byte fl_slosh = 0;
byte fl_start = 1;

String hours_RTC = "0";
String hours_RTC_NOW = "0";

/////////////////////////////////////////////////////
String month ;
String tmp ;
String date;
String dateLCD ;

int hour = 0;
int minute = 0;
int sec = 0;

int soft_hour = 0;
int soft_minute = 0;
int soft_sec = 0;

int real_hour = 0;
int real_minute = 0;
int real_sec = 0;

unsigned long int real_time_tmp = 0 ;


/////////////////////////////////////////////////////

void setup() {
  lcd.init();                      // initialize the lcd
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Hello, World!!!");
  lcd.setCursor(1, 1);
  lcd.print("Smart GreenHouse");
  lcd.setCursor(2, 2);
  lcd.print("Powered by 1547");
  lcd.setCursor(4, 3);
  lcd.print("IT-Poligon");
  delay (1000);
  lcd.clear();
  lcd.setCursor(3, 1);
  lcd.print("Connecting");
  Serial.begin(115200);
  lcd.print(".");
  Ethernet.begin(mac);
  lcd.print(".");
  delay(1000);
  dht.begin();
  lcd.print(".");
  delay(4000);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Connecting complete");
  delay(500);
  lcd.clear();
  Serial.println("RUN");

  pinMode (R1, OUTPUT);
  pinMode (R2, OUTPUT);
  pinMode (R3, OUTPUT);
  pinMode (R4, OUTPUT);

  pinMode (HUM_GR, INPUT);

  digitalWrite(R1, HIGH);
  digitalWrite(R2, HIGH);
  digitalWrite(R3, HIGH);
  digitalWrite(R4, HIGH);

  Serial.println("START");

}


void loop() {
  ////////////ETHERNET GET HERE//////////////
  if ((millis() - getmillis) > getDelay) {
    getmillis = millis();
    EthernetClient client;
    const char* host = "www.google.com";
    Serial.print("connected ");
    Serial.println(client.remoteIP());
    if (client.connect(host, 80))
    {
      yield();
      Serial.println("connected]");
      Serial.println("[Sending a request]");
      client.print(String("GET /") + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Connection: close\r\n" +
                   "\r\n"
                  );
      delay (200);
      unsigned long timeout = millis();
      while (client.available() == 0) {
        if (millis() - timeout > 5000)
        { Serial.println(">>> Client Timeout !");
          yield();
          client.flush();
          client.stop(); return;
        }
      } // Read all the lines of the reply from server and print them to Serial
      Serial.println("");

      while (client.connected() || client.available())
      {
        yield();
        if (client.available())
        {
          String line = client.readStringUntil('\n');
          Serial.println(line);

          if (line.indexOf("Date") == 0) {
            month = line.substring(5, 22);
            Serial.print("Date:");
            Serial.print(month);
            Serial.println();
            tmp = line.substring(23, 25);
            hour = tmp.toInt();
            hour = hour + GMT;
            Serial.print("Hours:");
            Serial.print(hour);
            hours_RTC_NOW = hour;
            Serial.println();
            tmp = line.substring(26, 28);
            minute = tmp.toInt();
            Serial.print("Minutes:");
            Serial.print(minute );
            Serial.println();
            tmp = line.substring(29, 32);
            sec = tmp.toInt();
            Serial.print("Seconds:");
            Serial.print(sec);
            Serial.println();

            date = line.substring(11, 22);
            Serial.print("Date: ");
            Serial.print(date);
            Serial.println();
            client.flush();
            client.stop();
            client.flush();
            if (fl_start) {
              soft_hour = hour;
              soft_minute = minute;
              soft_sec = sec;
              fl_start = 0;
              start_time = millis();
              getDelay = 100000;
            }
            return;
          }
        }
      }
      client.stop();
      Serial.println("\n[Disconnected]");
    }
    else
    {
      Serial.println("connection failed!]");
      client.stop();
    }
  }
  /////// "real" time calculate ////////////
  if (millis() % 500 <= 10) {
    real_time_tmp = (millis() - start_time) / 1000;
    real_sec = real_time_tmp % 60;
    real_sec = (real_sec + soft_sec) % 60;
    Serial.println("################################################################");
    Serial.print("Real sec = ");
    Serial.println(real_sec);
    real_minute = real_time_tmp / 60;
    real_time_tmp = real_time_tmp / 60;
    real_minute = (real_minute + soft_minute) % 60;
    Serial.print("Real mins = ");
    Serial.println(real_minute);
    real_hour = (real_time_tmp + soft_minute) / 60;
    //real_time_tmp = real_time_tmp / 24;
    real_hour = (real_hour + soft_hour) % 24;
    Serial.print("Real hours = ");
    Serial.println(real_hour);
    Serial.println("################################################################");

  }
  /////////////sensors read////////
  if (millis() % 1000 <= 10) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float light = analogRead(LIGHT);
    delay(50);
    if (isnan(t) || isnan(h)) {
      Serial.println("Failed to read from DHT");
      hum_dht = -1;
      temp_dht = -1;
    } else {
      Serial.println("DHT read OK");
      hum_dht = h;
      temp_dht = t;
    }
    hum_gr = analogRead(HUM_GR);
  }
  ////////sensors out///////////////////////////////
  if (millis() % 1000 == 0) {
    Serial.println();
    Serial.println("#####################");
    Serial.println(dateLCD);
    Serial.println("#####################");
    Serial.print("Humidity: ");
    Serial.print(hum_dht);
    Serial.println(" %\t");
    Serial.print("Temperature: ");
    Serial.print(temp_dht);
    Serial.println(" *C");
    Serial.print("Light: ");
    Serial.print(light);
    Serial.println(" lux");
    Serial.print("Ground humidity: ");
    Serial.print(hum_gr);
    Serial.println(" %");
    Serial.print("Water level: ");
    Serial.print(water_lvl);
    Serial.println(" ");
    Serial.println("#####################");
    Serial.print("R1 ");
    Serial.println(R1_state);
    Serial.print("R2 ");
    Serial.println(R2_state);
    Serial.print("R3 ");
    Serial.println(R3_state);
    Serial.print("R4 ");
    Serial.println(R4_state);
    Serial.println("#####################");
    Serial.print("Hours RTC ");
    Serial.println(hours_RTC);
    Serial.print("fl_light ");
    Serial.println(fl_light);
    Serial.print("fl_poliv ");
    Serial.println(fl_poliv);
  }



  //////////LCD OUT////////////////

  if ((millis() - lcdmillis) > 750) {
    dateLCD = "";
    if (real_hour < 10) {
      dateLCD = " " + String(real_hour) + ":";
    }
    else {
      dateLCD = String(real_hour) + ":";
    }
    if (real_minute < 10) {
      dateLCD = dateLCD + "0" + String(real_minute) + ":";
    }
    else {
      dateLCD = dateLCD + String(real_minute) + ":";
    }
    if (real_sec < 10) {
      dateLCD = dateLCD + "0" + String(real_sec) + " " + date;
    }
    else {
      dateLCD = dateLCD + String(real_sec) + " " + date;
    }

    lcdmillis = millis();
    lcd.setCursor(0, 0);
    lcd.print("                 ");
    lcd.setCursor(0, 0);
    lcd.print(dateLCD);
    lcd.setCursor(0, 1);
    lcd.print("                  ");
    lcd.setCursor(0, 1);
    lcd.print("Tem ");
    lcd.print(temp_dht);
    lcd.setCursor(11, 1);
    lcd.print("Hum ");
    lcd.print(hum_dht);
    lcd.setCursor(0, 2);
    lcd.print("                  ");
    lcd.setCursor(0, 2);
    lcd.print("Lig ");
    lcd.print(analogRead(A1));
    lcd.setCursor(11, 2);
    lcd.print("GR_h ");
    lcd.print(hum_gr);
    lcd.setCursor(0, 3);
    lcd.print("                  ");
    lcd.setCursor(0, 3);
    lcd.print("R1:");
    lcd.print(R1_state);
    lcd.print(" R2:");
    lcd.print(R2_state);
    lcd.print("  R3:");
    lcd.print(R3_state);
    lcd.print(" R4:");
    lcd.print(R4_state);
  }

  // **** LOGIC **** //
  // slash 1 sec every 15 min


  ///////////// LIGHT ///////////////
  if (millis() % 700 == 0) {
    if (hours_RTC != hours_RTC_NOW) {
      hours_RTC = hours_RTC_NOW;
      fl_poliv = 1;
      polivmillis = millis();
    }
    if ((real_minute % 15 == 0) and (fl_slosh == 0)) {
      digitalWrite(R3, LOW);
      R3_state = 1;
      delay(1000);
      digitalWrite(R3, HIGH);
      R3_state = 0;
      fl_slosh = 1;
      sloshmillis = millis();
    }
    // light from 5 to 21
    if ((hours_RTC_NOW.toInt() > 5 ) && (hours_RTC_NOW.toInt() < 21)) {
      fl_light = 1;
    }
    else
    {
      fl_light = 0;
    }
  }
  ///// on\ off poliv from rtc//////
  if (temp_dht < 24) {
    digitalWrite(R2, HIGH);
    R2_state = 0;
  }
  if (temp_dht > 26) {
    digitalWrite(R2, LOW);
    R2_state = 1;
  }
  if (fl_poliv) {
    digitalWrite(R4, LOW);
    R4_state = 1;
  }
  else
  {
    digitalWrite(R4, HIGH);
    R4_state = 0;
  }
  if ((millis() - polivmillis) > POLIV_TIMEOUT) {
    fl_poliv = 0;
  }
  if ((millis() - sloshmillis) > SLOSH_TIMEOUT) {
    fl_slosh = 0;
  }
  ///// on\off light //////
  if (fl_light) {
    digitalWrite(R1, LOW);
    R1_state = 1;
  }
  else
  {
    digitalWrite(R1, HIGH);
    R1_state = 0;
  }

  ////////end
  yield();
}
