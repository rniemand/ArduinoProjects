#include <Arduino.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

#define DHTPIN         D2
#define DHTTYPE        DHT11
#define LDR_PIN        A0

const String deviceId  = "0E97314A-8921-499F-BDEB-585276EB3020";

float humidity         = 0;
float temperature      = 0;
float heatIndex        = 0;
int lightReading       = 0;
bool dhtReadSuccess    = false;
String endPoint        = "http://192.168.0.5/DataCollector/API/v1/Test/Post";
//String endPoint        = "http://arduino.rniemand.com/API/v1/Test/Post";
int sleepInterval      = 5 * 60 * 1000; // 5 min

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  USE_SERIAL.begin(9600);

  // Boot DHT  
  dht.begin();

  // Setup LDR
  pinMode(LDR_PIN, INPUT);
}

void loop() {
  readDhtValues();
  readLdrValue();
  
  printTemperatureData();
  printLdrData();
  
  delay(500);
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

// =============================== >
// LDR Methods

void readLdrValue() {
  lightReading = analogRead(A0);
}

void printLdrData() {
  Serial.print("LDR Value: ");
  Serial.println(lightReading);
}




