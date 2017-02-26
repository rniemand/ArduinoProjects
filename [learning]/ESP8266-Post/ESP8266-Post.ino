#include <Arduino.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

#define DHTPIN         D2
#define DHTTYPE        DHT11
const char* ssid       = "PickMe";
const char* password   = "fallout312345";
const String deviceId  = "0E97314A-8921-499F-BDEB-585276EB3020";

float humidity         = 0;
float temperature      = 0;
float heatIndex        = 0;
bool dhtReadSuccess    = false;
String endPoint        = "http://192.168.0.71/DataCollector/API/v1/Test/Post";

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  USE_SERIAL.begin(9600);

  // Boot DHT  
  dht.begin();

  // Boot WiFi
  waitForWifiReady();
  WiFiMulti.addAP(ssid, password);
}

void loop() {
  readDhtValues();
  submitData();
  
  delay(dhtReadSuccess ? 250 : 100);
}

// =============================== >
// WiFi Methods

void waitForWifiReady() {
      for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }
}

String generatePostData() {
  String a01 = "Temperature=";
  String a02 = "&Humidity=";
  String a03 = "&HeatIndex=";
  String a04 = "&DeviceId=";
  
  return a01 + temperature + a02 + humidity + a03 + heatIndex + a04 + deviceId;
}

void submitData() {
  // We only want to submit data if we have it
  if (isnan(humidity) || isnan(temperature)) {
    return;
  }
  
  // wait for WiFi connection
  if((WiFiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    http.begin(endPoint);

    String postData = generatePostData();
    Serial.println(postData);
    
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.POST(postData);
    http.writeToStream(&Serial);

    http.end();
  }
}

// =============================== >
// DHT Methods

void readDhtValues() {
  dhtReadSuccess = false;
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  dhtReadSuccess = true;
  heatIndex = dht.computeHeatIndex(temperature, humidity, false);
  printTemperatureData();
}

void printTemperatureData() {
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C ");
  Serial.print("Heat index: ");
  Serial.print(heatIndex);
  Serial.print(" *C ");
  Serial.println();
}





