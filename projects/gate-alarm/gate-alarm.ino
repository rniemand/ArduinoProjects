#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/************************* WiFi and MQTT Config *********************************/
#define WLAN_SSID                 "xxx"
#define WLAN_PASS                 "xxx"
const char* mqtt_server           = "192.168.0.5";
#define MQTT_PORT                 1883
String MQTT_CLIENT_NAME           = "node_lounge";
#define MQTT_USER                 "xxx"
#define MQTT_PASS                 "xxx"

/*************************    Configuration     *********************************/
#define DEBUG                     true
#define REEDSW_ENABLED            true
#define REEDSW_PIN                D1


/*************************     Sensor Vars     *********************************/
boolean   reed_state_changed      = false;
boolean   reed_switch_closed      = false;
long      reed_last_triggered     = 0;
short     reed_trigger_threshold  = 100;
short     reed_publish_interval   = 10 * 60 * 5; // Every 5 min
short     reed_loop_count         = reed_publish_interval; // Publish immediatley
String    reed_topic              = "security/reed_switch/" + MQTT_CLIENT_NAME;

/*************************     Global Vars     *********************************/
WiFiClient                      esp_client;
PubSubClient                    mqtt_client(esp_client);

void setup() {
  #if DEBUG == true
    Serial.begin(9600);
  #endif

  pinMode(REEDSW_PIN, INPUT_PULLUP);
  
  // Setup WiFi connection & connect to MQTT server
  waitForWiFiConnection();
  connectMqtt();
}

void loop(){
  // Ensure that the MQTT mqtt_client is connected
  if (!mqtt_client.connected()) { reconnect(); }
  mqtt_client.loop();

  reed_loop();
  
  delay(50);
}

// ############################## Reed Switch functions
void reed_loop() {
  reed_loop_count++;

  // Force a check of the reed switch]
  handleReedSwitch();

  // Test to see if the reed switch state has changed
  if(reed_state_changed) {
    reed_state_changed = false;
    publishReedSwitchState();
    return;
  }

  // Check to see if we need to publish a check in event
  if(reed_loop_count >= reed_publish_interval) {
    reed_loop_count = 0;
    publishReedSwitchState();
  }
}

void publishReedSwitchState() {
  // Update the state of the client over MQTT
  mqtt_client.publish(reed_topic.c_str(), reed_switch_closed ? "CLOSED" : "OPEN");

  #if DEBUG == true
    Serial.print("(MQTT) Reed switch state updated: ");
    Serial.println(reed_switch_closed ? "CLOSED" : "OPEN");
  #endif
}

void handleReedSwitch() {
  // TRUE = CLOSED - FALSE = OPEN
  boolean currentState = digitalRead(REEDSW_PIN) == LOW;

  // If the current state of the reed switch = last state - skip over
  if(currentState == reed_switch_closed) {
    // We don't need to waste instructions checking
    return;
  }

  // Decide if we need to raise an event
  if((millis() - reed_last_triggered) < reed_trigger_threshold) {
    #if DEBUG == true
      Serial.print("Reed switch state changed, waiting on time threshold of ");
      Serial.print(reed_trigger_threshold);
      Serial.println(" ms before raising an alert");
    #endif
    
    return;
  }

  // Ok we are good to raise an alert, set the relevant flags
  reed_state_changed  = true;           // Signal state change
  reed_switch_closed  = currentState;   // Set current state for logging
  reed_last_triggered = millis();       // Remember last change time
}

// ############################## WiFi functions
void waitForWiFiConnection() {
  #if DEBUG == true
    Serial.println("Connecting to WiFi");
  #endif
  
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    #if DEBUG == true
      Serial.print(".");
    #endif
    
    delay(250);
  }

  #if DEBUG == true
    Serial.println();
    Serial.print("Connected: ");
    Serial.println(WiFi.localIP());
  #endif
}


// ############################## MQTT functions
void connectMqtt() {
  mqtt_client.setServer(mqtt_server, MQTT_PORT);
  mqtt_client.setCallback(callback);
  reconnect();
}

void reconnect() {
  #if DEBUG == true
    Serial.print("Attempting to connect to MQTT server: ");
    Serial.println(mqtt_server);
 #endif

  while (!mqtt_client.connected()) {
    if (mqtt_client.connect(MQTT_CLIENT_NAME.c_str(), MQTT_USER, MQTT_PASS, ("node/" + MQTT_CLIENT_NAME).c_str(), 1, false, "timeout")) {
      mqtt_client.publish(("node/" + MQTT_CLIENT_NAME).c_str(), "connected");
      
      //mqtt_client.subscribe(String("cmd/" + MQTT_CLIENT_NAME).c_str());
      //mqtt_client.subscribe("cmd/node_kitchen");
      mqtt_client.subscribe(String("cmd/" + MQTT_CLIENT_NAME).c_str());
      
      #if DEBUG == true
        Serial.println("connected");
      #endif
    } else {
      #if DEBUG == true
        Serial.println("unable to connect, waiting 5 seconds...");
      #endif
      
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String command = getCommand(payload, length);

  if(command == "update_all") {
    publishReedSwitchState();
    return;
  }

  if(command == "update_reed") {
    #if DEBUG == true
      Serial.println("(MQTT) Updating reed switch information");
    #endif
    
    publishReedSwitchState();
    return;
  }
  
  #if DEBUG == true
    Serial.print("UNKNOWN MQTT COMMAND: ");
    Serial.println(command);
  #endif
}

String getCommand(byte* payload, unsigned int length) {
   char msg[length + 1];
   
  for (int i = 0; i < length; i++) {
    msg[i] = (char) payload[i];
  }
  
  msg[length] = '\0';

  return String(msg);
}



