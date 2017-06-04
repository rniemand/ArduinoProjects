#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

/************************* WiFi & Config *********************************/

#define WLAN_SSID         "PickMe"
#define WLAN_PASS         "fallout312345"
const char* mqtt_server   = "broker.mqtt-dashboard.com";

/************************* WiFi & Config *********************************/

U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ D5, /* data=*/ D6, /* reset=*/ U8X8_PIN_NONE);
WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;
String line1 = "";
String line2 = "";


void setup(void) {
  u8g2.begin();
  waitForWiFiConnection();
  connectMqtt();
}

void loop(void) {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    print("Publishing", msg);
    client.publish("rnOutTopic", msg);
  }
  
  //updateScreen();
  delay(1000);  
}

void connectMqtt() {
  print("Connecting MQTT", mqtt_server);
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
  //line1 = String("Message [" + topic + "]");
  line1 = topic;
  char tmp[length];
  
  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
    tmp[i] = (char)payload[i];
  }

  line2 = String(tmp);
  //Serial.println();
  updateScreen();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    print("Attempting MQTT connection...", "");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      print("MQTT connected", "");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      print("failed, rc=" + client.state(), "Waiting 5 secs.");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void updateScreen() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_courR08_tf);
  u8g2.drawStr(0,8,"IP: ");
  u8g2.drawStr(20,8, getIpAddress());
  //u8g2.drawStr(0,16,"AP: " + WLAN_SSID);
  u8g2.drawStr(0,16, "AP: ");
  u8g2.drawStr(20,16, WLAN_SSID);

  int line1length = line1.length() + 1;
  char char_array_1[line1length];
  line1.toCharArray(char_array_1, line1length);
  u8g2.drawStr(0,24, char_array_1);
  
  int line2length = line2.length() + 1;
  char char_array_2[line2length];
  line2.toCharArray(char_array_2, line2length);
  u8g2.drawStr(0,32, char_array_2);
  
  u8g2.sendBuffer();
}

void waitForWiFiConnection() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_courR08_tf);
  u8g2.drawStr(0,8,"Connecting to");
  u8g2.drawStr(0,16, WLAN_SSID);
  u8g2.sendBuffer();
  char dots[20] = "";
  int pos = 0;
  
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    dots[pos] = '.';
    u8g2.drawStr(0,24, dots);
    u8g2.sendBuffer();
    delay(500);
    pos++;
  }

  u8g2.drawStr(0,32,"Connected!");
  u8g2.sendBuffer();
  delay(500);

  line1 = "WiFi: Connected";
  line2 = "Testing...";
}

String ipToString(IPAddress ip){
  String s="";
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}

void print(String l1, String l2) {
  line1 = l1;
  line2 = l2;

  updateScreen();
}

char* getIpAddress() {
  int str_len = 15;
  char char_array[str_len];
  ipToString(WiFi.localIP()).toCharArray(char_array, str_len);
  return char_array;
}





