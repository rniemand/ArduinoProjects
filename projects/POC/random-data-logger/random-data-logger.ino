#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define WLAN_SSID         "PickMe"
#define WLAN_PASS         "fallout312345"
const char* mqtt_server   = "mqtt.rniemand.com";
String CLIENT_ID          = "esp8266";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  Serial.println("Booting up");

  waitForWiFiConnection();
  connectMqtt();

  Serial.println("booted");
}

void loop() {
  runClientLoop();

  // Publish a random number
  char _temp[10] = {0};
  dtostrf (random(0, 2048), 6 , 2, _temp);
  client.publish("randomNumber", _temp);
  Serial.print("randomNumber: ");
  Serial.println(_temp);
  
  // Wait a while before publishing again
  delay(1000);
}

void callback(char* topic, byte* payload, unsigned int length) {
  char tmp[length + 1];

  Serial.println(topic);
  Serial.println(length);
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    tmp[i] = (char)payload[i];
  }
  
  tmp[length] = '\0';

  Serial.print("Full payload: ");
  Serial.println(tmp);
}

// Helper methods
void runClientLoop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void waitForWiFiConnection() {
  Serial.println("Connecting to WiFi");
  
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println();
  Serial.println("Connected");
}

void connectMqtt() {
  Serial.println("Configuring MQTT client");
  
  Serial.print("Setting server: ");
  Serial.println(mqtt_server);
  client.setServer(mqtt_server, 1883);

  Serial.println("Setting callback");
  client.setCallback(callback);
  
  reconnect();
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection");
    
    if (client.connect(CLIENT_ID.c_str(), "esp8266", "bob")) {
      Serial.println("Connected");
      //client.publish("outTopic", "hello world");
      //client.subscribe("rnInTopic");
    } else {
      Serial.print("failed, rc=" + client.state());
      Serial.println("Waiting 5 secs.");
      delay(5000);
    }
  }
}



