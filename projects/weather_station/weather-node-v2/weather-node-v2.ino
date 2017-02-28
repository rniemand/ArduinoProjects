#include <ESP8266WiFi.h>
#include <DHT.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "PickMe"
#define WLAN_PASS       "fallout312345"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883  1883
#define AIO_USERNAME    "niemandr"
#define AIO_KEY         "2448a303eb354ad18a64623e6544916a"

/************************* Pin Configuration *********************************/

#define LED_DISCONNECTED  D0
#define LED_CONNECTED     D1
#define LED_PUBLISHING    D2
#define DHTPIN            D3
#define LDR_PIN           A0
#define DHTTYPE           DHT11

/************ Global State (you don't need to change this!) ******************/

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
DHT dht(DHTPIN, DHTTYPE);

/****************************** Feeds ***************************************/

Adafruit_MQTT_Publish temperatureFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish humidityFeed    = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");
Adafruit_MQTT_Publish heatIndexFeed   = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/heatIndex");
Adafruit_MQTT_Publish ldrFeed         = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ldr");

// Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");

/*************************** Sketch Code ************************************/

bool dhtReadSuccess       = false;
float humidity            = 0;
float temperature         = 0;
float heatIndex           = 0;
int lightReading          = 0;

void MQTT_connect();

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
  digitalWrite(LED_CONNECTED, HIGH);
  digitalWrite(LED_PUBLISHING, HIGH);
  digitalWrite(LED_DISCONNECTED, LOW);

  // Boot application
  Serial.println("Home Weather Station 1.0.0");

  dht.begin();
  waitForWiFiConnection();
  waitForDhtReady();


  // Setup MQTT subscription for onoff feed.
  // mqtt.subscribe(&onoffbutton);
}

void loop() {
  MQTT_connect();

  /*
  Adafruit_MQTT_Subscribe *subscription;
  
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);
    }
  }
  */

  
  refreshData();
  publishData();

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */

  delay(10000);
}

// ========================================================== >>
// WiFi methods

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

// ========================================================== >>
// DHT methods

void waitForDhtReady() {
  Serial.println("Waiting for DHT");
  
  while(!dhtReadSuccess) {
    delay(250);
    Serial.print(".");
    readDhtValues();
  }

  Serial.println();
  Serial.print("DHT Ready: (temperature: ");
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
  
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    return;
  }

  dhtReadSuccess = true;
  heatIndex = dht.computeHeatIndex(temperature, humidity, false);
}

// ========================================================== >>
// LDR methods

void readLdrValue() {
  lightReading = analogRead(A0);
  //Serial.print("LDR Value: ");
  //Serial.println(lightReading);
}

// ========================================================== >>
// Working with collected data

void refreshData() {
  readLdrValue();
  readDhtValues();
}

void publishData() {
  // Push data to server
  Serial.println("Publishing MQTT data...");
  digitalWrite(LED_PUBLISHING, LOW);
  
  Serial.print("    temperature : (");
  Serial.print(temperature);
  Serial.println(temperatureFeed.publish(temperature) ? ") success" : ") failure");

  Serial.print("    humidity    : (");
  Serial.print(humidity);
  Serial.println(humidityFeed.publish(humidity) ? ") success" : ") failure");

  Serial.print("    heatIndex   : (");
  Serial.print(heatIndex);
  Serial.println(heatIndexFeed.publish(heatIndex) ? ") success" : ") failure");

  Serial.print("    ldr         : (");
  Serial.print(lightReading);
  Serial.println(ldrFeed.publish(lightReading) ? ") success" : ") failure");
  
  digitalWrite(LED_PUBLISHING, HIGH);
}

void toggleConnectionLeds(bool connected) {
  digitalWrite(LED_DISCONNECTED, connected ? HIGH : LOW);
  digitalWrite(LED_CONNECTED, connected ? LOW : HIGH);
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    toggleConnectionLeds(true);
    return;
  }

  toggleConnectionLeds(false);
  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 5;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 3 seconds...");
       mqtt.disconnect();
       delay(3000);
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }

  toggleConnectionLeds(true);
  Serial.println("MQTT Connected!");
}


