#include <Wire.h>
#include <Adafruit_INA219.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define WLAN_SSID         "PickMe3"
#define WLAN_PASS         "fallout312345"
#define SLEEP_TIME        2000

const char* mqtt_server   = "192.168.0.5";
String CLIENT_NAME        = "solarLogger1";
bool poweredOn            = false;


Adafruit_INA219 ina219;
WiFiClient espClient;
PubSubClient client(espClient);


void setup(void) {
  Serial.begin(9600);

  waitForWiFiConnection();
  connectMqtt();
  
  uint32_t currentFrequency;
  
  Serial.println("Hello!");
  
  Serial.println("Measuring voltage and current with INA219 ...");
  ina219.begin();
}

void loop(void) {
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  client.publish("solar_panel/shuntvoltage", string2char(String(shuntvoltage, DEC)));
  client.publish("solar_panel/busvoltageltage", string2char(String(busvoltage, DEC)));
  client.publish("solar_panel/current_mA", string2char(String(current_mA, DEC)));
  client.publish("solar_panel/loadvoltage", string2char(String(loadvoltage, DEC)));
  
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.println("");

  delay(SLEEP_TIME);
}







char* string2char(String command){
    if(command.length()!=0){
        char *p = const_cast<char*>(command.c_str());
        return p;
    }
}

void waitForWiFiConnection() {
  Serial.println("Connecting to WiFi");
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println();
  Serial.print("Connected: ");
  Serial.println(WiFi.localIP());
}

void connectMqtt() {
  client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);
  reconnect();
}

void reconnect() {
  Serial.print("Attempting to connect to MQTT server: ");
  Serial.println(mqtt_server);
  
  while (!client.connected()) {
    if (client.connect(CLIENT_NAME.c_str(), "solarlogger", "solarpower")) {
      // Once connected, publish an announcement...
      client.publish("mp3", "client started");
      client.subscribe("mp3");
      Serial.println("connected");
    } else {
      Serial.println("unable to connect, waiting 5 seconds...");
      delay(5000);
    }
  }
}

