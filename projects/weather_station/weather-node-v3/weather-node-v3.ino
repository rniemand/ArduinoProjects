#include <ESP8266WiFi.h>
#include <DHT.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi and MQTT Config *********************************/

#define WLAN_SSID       "PickMe"
#define WLAN_PASS       "fallout312345"
#define AIO_SERVER      "mqtt.thingspeak.com"
#define AIO_SERVERPORT  1883
#define CHANNEL_ID      ""
#define PUBLISH_KEY     ""

/************************* Pin Configuration *********************************/

#define LED_DISCONNECTED  D0
#define LED_CONNECTED     D1
#define LED_PUBLISHING    D2
#define DHTPIN            D3
#define LDR_PIN           A0

/************************* Misc Configuration *********************************/

#define DHTTYPE           DHT11
#define MAX_CON_RETRY     12
#define CON_RETRY_DELAY   5000
#define DEBUG             true

short TICK_INTERVAL_MS    = 1000;   // time between ticks below (use this as your multiplier)
short PUB_INT_TEMP        = 300;    // publish TEMP readings every x loops
short MQTT_PING_INT       = 15;     // ping the MQTT server every x seconds to keep alive

short tickCountTemp       = PUB_INT_TEMP;
short tickCountPing       = 0;

/************ Global State (you don't need to change this!) ******************/

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT);
DHT dht(DHTPIN, DHTTYPE);

/*************************** Sketch Code ************************************/

bool dhtReadSuccess       = false;
float humidity            = 0;
float temperature         = 0;
float heatIndex           = 0;
int lightReading          = 0;

// void MQTT_connect();

// ========================================================== >>
// Program controls

void setup() {
  Serial.begin(9600);
  delay(10);

  // configure pin modes
  pinMode(LED_CONNECTED, OUTPUT);
  pinMode(LED_DISCONNECTED, OUTPUT);
  pinMode(LED_PUBLISHING, OUTPUT);
  pinMode(LDR_PIN, INPUT);

  // Set initial LED state
  RGB(true, false, false);

  // Boot application
  Serial.println("Home Weather Station 1.0.0");

  dht.begin();
  waitForWiFiConnection();
  waitForDhtReady();

  // Setup MQTT subscription for onoff feed.
  //mqtt.subscribe(&ledSubscription);
}

void loop() {
  runPing(false);
  publishData();
  //runLEDTest();
  
  delay(TICK_INTERVAL_MS);
}

// ========================================================== >>
// Publishing related methods

void runPing(bool forcePing) {
  // We only want to send the ping command every "MQTT_PING_INT" loops
  if( tickCountPing < MQTT_PING_INT && forcePing == false ) {
    tickCountPing++;
    return;
  }

  RGB(false, false, true);
  Serial.print("Running ping ... ");

  if(!mqtt.ping(3)) {
    RGB(true, false, false);
    Serial.println("FAILED - attempting reconnection");
    MQTT_connect();
  } else {
    Serial.println("SUCCESS");
    RGB(false, true, false);
  }

  // Reset the tick counter so we can do this all over again
  tickCountPing = 0;
}

void publishData() {
  // We only want to publish temperature data every "PUB_INT_TEMP" ticks
  if( tickCountTemp < PUB_INT_TEMP ) {
    tickCountTemp++;
    return;
  }

  // Ensure that we are connected
  runPing(true);
  
  // Refresh our readings
  readLdrValue();
  readDhtValues();

  Serial.println("Publishing Temperatue data...");

  String data = String(
    "field1=" + String(temperature, DEC) + 
    "&field2=" + String(heatIndex, DEC) + 
    "&field3=" + String(humidity, DEC) +
    "&field4=" + String(lightReading, DEC)
  );
  
  int length = data.length();
  char msgBuffer[length];
  data.toCharArray(msgBuffer,length+1);
  Serial.print("   >> ");
  Serial.println(msgBuffer);

  RGB(false, false, true);
  bool success = mqtt.publish("channels/" CHANNEL_ID "/publish/" PUBLISH_KEY, msgBuffer);
  RGB(false, true, false);

  // Decide if we need to re-publish the data, or mark this as a success
  Serial.println(success ? "SUCCESS" : "FAILED");
  tickCountTemp = success ? 0 : tickCountTemp;
}

void runLEDTest() {
  Serial.println("Red");
  RGB(true, false, false);
  delay(1000);

  Serial.println("Green");
  RGB(false, true, false);
  delay(1000);

  Serial.println("Blue");
  RGB(false, false, true);
}


// ========================================================== >>
// MQQT Methods

// https://learn.adafruit.com/remote-control-with-the-huzzah-plus-adafruit-io/led-code
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    toggleConnectionLeds(true);
    return;
  }

  toggleConnectionLeds(false);
  Serial.print("Connecting to MQTT... ");

  uint8_t retries = MAX_CON_RETRY;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.print("Retrying MQTT connection in ");
       Serial.print(CON_RETRY_DELAY);
       Serial.println("ms...");
       mqtt.disconnect();
       delay(CON_RETRY_DELAY);
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }

  toggleConnectionLeds(true);
  Serial.println("MQTT Connected!");
}


// ========================================================== >>
// Hardware methods

void waitForWiFiConnection() {
  // Connect to WiFi access point.
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
}

void waitForDhtReady() {
  Serial.print("Waiting for DHT ");
  
  while(!dhtReadSuccess) {
    delay(250);
    Serial.print(".");
    readDhtValues();
  }
  
  Serial.print(" READY (temperature: ");
  Serial.print(temperature);
  Serial.print(" *C) (humidity: ");
  Serial.print(humidity);
  Serial.print(" %) (heatIndex: ");
  Serial.print(heatIndex);
  Serial.print(")");
  Serial.println();
}

void readDhtValues() {
  dhtReadSuccess = false;
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    return;
  }

  dhtReadSuccess = true;
  heatIndex = dht.computeHeatIndex(temperature, humidity, false);
  Serial.println("Updated DHT values");
}

void toggleConnectionLeds(bool connected) {
  digitalWrite(LED_DISCONNECTED, connected ? HIGH : LOW);
  digitalWrite(LED_CONNECTED, connected ? LOW : HIGH);
}

void readLdrValue() {
  lightReading = analogRead(A0);
  Serial.println("Updated LDR value");
}

void RGB(bool redOn, bool greenOn, bool blueOn) {
  // Turn all off
  digitalWrite(LED_DISCONNECTED, HIGH);
  digitalWrite(LED_CONNECTED, HIGH);
  digitalWrite(LED_PUBLISHING, HIGH);

  // Turn requested LED on
  if( redOn ) digitalWrite(LED_DISCONNECTED, LOW);
  if( greenOn ) digitalWrite(LED_CONNECTED, LOW);
  if( blueOn ) digitalWrite(LED_PUBLISHING, LOW);
};






