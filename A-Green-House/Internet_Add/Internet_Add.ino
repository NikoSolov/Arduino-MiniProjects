#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

char server[] = "teplic1547.000webhostapp.com";    // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 0, 1);

EthernetClient client;

// Variables to measure the speed
unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = true;  // set to false for better speed measurement

void setup() {
  // start the Ethernet connection:
  Serial.begin(9600);
  Ethernet.begin(mac);
  Serial.println(Ethernet.localIP());    
  // give the Ethernet shield a second to initialize:
  delay(1000);

  Serial.println(server);
  
  if (client.connect(server, 80)) {
    Serial.println(client.remoteIP());
    client.println("GET getout_test.php/?TIME=16&MYMAC=sdfasdfasd");
    client.println("Host: teplic1547.000webhostapp.com");
    client.println("Connection: close");
    client.println();

    } 
  else {
    Serial.println("connection failed");
  }
  beginMicros = micros();
}

void loop() {
  if (client.available()) {
    int len = client.available();
    byte buffer[80];
    client.read(buffer, len);
    Serial.write(buffer, len); // show in the serial monitor (slows some boards)
  }
  }
