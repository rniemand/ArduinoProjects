#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define WLAN_SSID         "PickMe3"
#define WLAN_PASS         "fallout312345"
const char* mqtt_server   = "192.168.0.5";
String CLIENT_NAME        = "pir1";
int calibrationTime       = 45;        

int pirPin                = D1;
int pin_motion            = D2;
int pin_boot              = D0;
int pin_mqtt              = D3;

boolean sensor_active     = false;
int sleep_delay           = 100;
int checkin_every_x_loops = 600; // (every 1 min)
int cooloff_loop_amount   = 50;  // 5 seconds
int current_loop_number   = 0;
int cooloff_counter       = 0;

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  Serial.begin(9600);
  
  pinMode(pirPin, INPUT);
  pinMode(pin_motion, OUTPUT);
  pinMode(pin_boot, OUTPUT);
  pinMode(pin_mqtt, OUTPUT);

  digitalWrite(pin_mqtt, LOW);
  digitalWrite(pin_boot, HIGH);
  digitalWrite(pirPin, LOW);

  waitForWiFiConnection();
  connectMqtt();
  digitalWrite(pin_mqtt, HIGH);
  client.publish("home/alarm/pir_kitchen", "pending");

  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
  for(int i = 0; i < calibrationTime; i++){
    Serial.print(i);
    Serial.print(" ");
    delay(1000);
  }
  digitalWrite(pin_boot, LOW);
  Serial.println("");

  reconnect();
  client.publish("home/alarm/pir_kitchen", "armed_home");
  Serial.println("SENSOR ACTIVE");
  delay(50);
}

void loop(){
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  // Check in periodically with the server
  if(current_loop_number >= checkin_every_x_loops) {
    current_loop_number = 0;
    client.publish("home/alarm/pir_kitchen", "armed_home");
  }
  
  if(digitalRead(pirPin) == HIGH && !sensor_active){
    sensor_active = true;
    Serial.println("triggered");
    
    digitalWrite(pin_motion, HIGH);
    digitalWrite(pin_mqtt, LOW);
    
    client.publish("home/alarm/pir_kitchen", "triggered");
  }
  
  if(digitalRead(pirPin) == LOW && sensor_active){
    if(cooloff_counter >= cooloff_loop_amount) {
      cooloff_counter = 0;
      sensor_active = false;
      Serial.println("reset");
      
      digitalWrite(pin_motion, LOW);
      digitalWrite(pin_mqtt, HIGH);

      client.publish("home/alarm/pir_kitchen", "armed_home");
    }
  }

  // increment counters
  if(sensor_active) { cooloff_counter++; }
  current_loop_number++;

  // Wait a bit
  delay(sleep_delay);
}


void callback(char* topic, byte* payload, unsigned int length) {
  String command = getCommand(payload, length);
  // client.publish("mp3out", "play");
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
    if (client.connect(CLIENT_NAME.c_str(), "niemandr", "2sav.2$$", "home/alarm/pir_kitchen", 1, false, "disarmed")) {
      // Once connected, publish an announcement...
      client.publish("home/alarm/pir_kitchen", "client started");
      //client.subscribe("mp3");
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


