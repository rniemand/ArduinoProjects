#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/*
 * D7   ORANGE    Play/Pause    Mode
 * D6   YELLOW    Repeat
 * D5   BLACK     Prev          Vol --
 * D0   WHITE     Next          Vol++
*/

#define POWER             D1
#define PLAY_PAUSE        D7
#define REPEAT_BTN        D6
#define PREVIOUS          D5
#define NEXT              D0
#define WLAN_SSID         "PickMe"
#define WLAN_PASS         "fallout312345"

#define LONG_PRESS        1000
#define SHORT_PRESS       250

const char* mqtt_server   = "mqtt.rniemand.com";
String CLIENT_NAME        = "mp3";
bool poweredOn            = false;

WiFiClient espClient;
PubSubClient client(espClient);

// Core methods
void setup() {
  Serial.begin(9600);

  waitForWiFiConnection();
  connectMqtt();
  
  pinMode(PLAY_PAUSE, OUTPUT);
  pinMode(REPEAT_BTN, OUTPUT);
  pinMode(PREVIOUS, OUTPUT);
  pinMode(NEXT, OUTPUT);
  
  pinMode(POWER, OUTPUT);
  digitalWrite(POWER, LOW);
  poweredOn = false;

  Serial.println("Booted");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  /*
  Serial.println("Play Pause");
  
  Serial.println("HIGH");
  digitalWrite(PLAY_PAUSE, HIGH);
  delay(500);

  Serial.println("LOW");
  digitalWrite(PLAY_PAUSE, LOW);
  
  delay(2000);
  */
}

// MQTT callback
void callback(char* topic, byte* payload, unsigned int length) {
  String command = getCommand(payload, length);

  if(command == "play") {
    pushButton(PLAY_PAUSE, SHORT_PRESS);
    client.publish("mp3out", "play");
  }

  if(command == "next") {
    pushButton(NEXT, SHORT_PRESS);
    client.publish("mp3out", "next");
  }

  if(command == "previous") {
    pushButton(PREVIOUS, SHORT_PRESS);
    client.publish("mp3out", "previous");
  }

  if(command == "vol+") {
    pushButton(NEXT, LONG_PRESS);
    client.publish("mp3out", "vol+");
  }

  if(command == "vol-") {
    pushButton(PREVIOUS, LONG_PRESS);
    client.publish("mp3out", "vol-");
  }

  if(command == "mode") {
    pushButton(PLAY_PAUSE, LONG_PRESS);
    client.publish("mp3out", "mode");
  }

  if(command == "power") {
    if(poweredOn) {
      digitalWrite(POWER, LOW);
      client.publish("mp3out", "power off");
    } else {
      digitalWrite(POWER, LOW);
      client.publish("mp3out", "power on");
    }

    poweredOn = !poweredOn;
  }
}

// Helper methods
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
  client.setCallback(callback);
  reconnect();
}

void reconnect() {
  Serial.print("Attempting to connect to MQTT server: ");
  Serial.println(mqtt_server);
  
  while (!client.connected()) {
    if (client.connect(CLIENT_NAME.c_str(), "mp3", "pass")) {
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

String getCommand(byte* payload, unsigned int length) {
   char msg[length + 1];
   
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msg[i] = (char) payload[i];
  }
  Serial.println();
  
  msg[length] = '\0';

  return String(msg);
}

void pushButton(int button, int timeMs) {
  digitalWrite(button, HIGH);
  delay(timeMs);
  digitalWrite(button, LOW);
}



