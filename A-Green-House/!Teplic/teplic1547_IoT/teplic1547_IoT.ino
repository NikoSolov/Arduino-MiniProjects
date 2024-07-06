#include <Ethernet.h>
#include <SPI.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>
#include <DS3231.h>
//////////////////////////////////////////////////////

#define POLIV_TIMEOUT 180000 // 3 min
#define DHTTYPE DHT21
#define SLOSH_TIMEOUT 1000
#define GMT 3

#define RX 0
#define TX 1

#define R1 4
#define R2 5
#define R3 6
#define R4 7

#define DHT_PIN 9

#define HUM_GR A0
#define LIGHT A1
///////////////////////////////////////////////////////

LiquidCrystal_I2C lcd(0x27, 20, 4);
DHT dht(DHT_PIN, DHTTYPE);
DS3231  rtc(11, 10); //RTC init

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//////////////////////////////////////////////////////

float temp_dht = 0;
float hum_dht = 0;

int light = 0;
int hum_gr = 0;

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

/////////////////////////////////////////////////////
String tmp ;

int hour = 0;
int minute = 0;
int second = 0;

int hour_net=0;
int minute_net = 0;
int second_net = 0;

bool online=false;
bool connected=false;
int min_slosh=0;
int hour_poliv=0;
String Time;

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
  lcd.setCursor(0, 0);
  lcd.print("Initialization start");
  Serial.begin(115200);
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Ethernet... ");
  if (Ethernet.begin(mac)){  lcd.print("started"); connected=true;}
  else{
  lcd.print("failed"); connected=false;
    }
  delay(1000);
  lcd.setCursor(0, 2);
  lcd.print("DHT...");
  dht.begin();
  lcd.setCursor(11, 2);
  lcd.print("started");
  delay(1000);
  lcd.setCursor(0, 3);
  lcd.print("RTC...");
  rtc.begin();
  lcd.setCursor(11, 3);
  lcd.print("started");
  delay(4000);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Init complete");
  delay(1500);
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
  Time=rtc.getTimeStr();
  hour=Time.substring(0,2).toInt();
  minute=Time.substring(3,5).toInt();
  second=Time.substring(6,8).toInt();
  hour_poliv=hour;
  min_slosh=minute;
}


void loop() {
  ////////////ETHERNET GET HERE//////////////
  if (connected && (millis() - getmillis) > getDelay) {
    Serial.println("Ethernet");
    getmillis = millis();
    EthernetClient client;
//    const char* host = "www.google.com";
    const char* host = "127.0.0.1:500/teplic/";
    Serial.print("connected to ");
    Serial.println(client.remoteIP());
    if (client.connect(host, 80))
    {
      yield();
      Serial.println("connected]");
      Serial.println("[Sending a request]");
      client.print(String("GET /") + " HTTP/1.1\r\n" +
                   "Host: " + host + temp_dht + "\r\n" +
                   "Connection: close\r\n" +
                   "\r\n"
                  );
      delay (200);
      online=true;
      unsigned long timeout = millis();
      while (client.available() == 0) {
        if (millis() - timeout > 5000)
        { Serial.println(">>> Client Timeout !");
          yield();
          client.flush();
          client.stop(); 
          return;
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
            tmp = line.substring(23, 25);
            hour_net = tmp.toInt();
            hour_net = hour_net + GMT;
            tmp = line.substring(26, 28);
            minute_net = tmp.toInt();
            tmp = line.substring(29, 32);
            second_net = tmp.toInt();
//            rtc.setTime(hour_net, minute_net, second_net);     // Set the time to 12:00:00 (24hr format)
            
            client.flush();
            client.stop();
            client.flush();
            if (fl_start) {
              fl_start = 0;
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
      online=false;
      client.stop();
    }
  }
  /////// "real" time calculate ////////////
Time=rtc.getTimeStr();
 hour=Time.substring(0,2).toInt();
minute=Time.substring(3,5).toInt();
second=Time.substring(6,8).toInt();
  /////////////sensors read////////
  if (millis() % 1000 <= 10) {
    Serial.println("Sensor");
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float light = analogRead(LIGHT);
    delay(50);
    if (isnan(t) || isnan(h)) {
      hum_dht = -1;
      temp_dht = -1;
    } else {
      hum_dht = h;
      temp_dht = t;
    }
    hum_gr = analogRead(HUM_GR);
  }
/*  ////////sensors out///////////////////////////////
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
*/


  //////////LCD OUT////////////////

  if ((millis() - lcdmillis) > 750) {
    Serial.println("LCD");
    lcdmillis = millis();
    lcd.setCursor(0, 0);
    lcd.print(rtc.getTimeStr());
    lcd.setCursor(10, 0);
    if (online){lcd.print("Online");}
    else if (!connected){lcd.print("Not Wired");}
    else {lcd.print("Offline");}
    
    lcd.print(" ");
    lcd.setCursor(0, 1);
    lcd.print("Tem ");
    lcd.print(temp_dht);
    lcd.setCursor(11, 1);
    lcd.print("Hum ");
    lcd.print(hum_dht);
    lcd.setCursor(0, 2);
    lcd.print("Lig ");
    lcd.print(analogRead(LIGHT));
    lcd.print(" ");
    lcd.setCursor(11, 2);
    lcd.print("GR_h ");
    lcd.print(hum_gr);
    lcd.print(" ");
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

  ///////////// LIGHT ///////////////
  if ((hour > 5 ) && (hour < 21)) {
    fl_light = 1;
  }
  else {
    fl_light = 0;
  }
/////////////////////////////////////
  if (fl_light) {
    digitalWrite(R1, LOW);
    R1_state = 1;
  }
  else {
    digitalWrite(R1, HIGH);
    R1_state = 0;
  }
  /////// dry //////////////
  if (temp_dht < 24.6) {
    digitalWrite(R2, HIGH);
    R2_state = 0;
  }
  else if (temp_dht > 25) {
    digitalWrite(R2, LOW);
    R2_state = 1;
  }
  ////////////////  SLOSH  /////////////////////
  if (minute % 15 == 0 && second==0 && minute!=min_slosh) {
    min_slosh=minute;
    fl_slosh = 1;
    sloshmillis = millis();
  }
  //////////////////////////////////////////////
  if ((millis() - sloshmillis) > SLOSH_TIMEOUT) {
    fl_slosh = 0;
  }
  ///////////////////////////////
  if (fl_slosh) {
    digitalWrite(R3, LOW);
    R3_state = 1;
  }
  else
  {
    digitalWrite(R3, HIGH);
    R3_state = 0;

  }
  //////////// POLIV ////////////////////
  if (hour!=hour_poliv && second==0) {
    hour_poliv=hour;
    fl_poliv = 1;
    polivmillis = millis();
  }
  else if ((millis() - polivmillis) > POLIV_TIMEOUT) {
    fl_poliv = 0;
  }
  ///////////////////////////////
  if (fl_poliv) {
    digitalWrite(R4, LOW);
    R4_state = 1;
  }
  else
  {
    digitalWrite(R4, HIGH);
    R4_state = 0;
  }
  ////////end

  yield();

}
