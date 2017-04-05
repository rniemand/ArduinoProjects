#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DHT.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <SPI.h>
#include <SD.h>

/************************* WiFi and MQTT Config *********************************/

#define WLAN_SSID       "PickMe"
#define WLAN_PASS       "fallout312345"
#define AIO_SERVER      "mqtt.thingspeak.com"
#define AIO_SERVERPORT  1883
#define CHANNEL_ID      "t"
#define PUBLISH_KEY     "t"

/************************* Pin Configuration *********************************/

#define LED_DISCONNECTED  D0
#define LED_CONNECTED     D1
#define LED_PUBLISHING    D2
#define DHTPIN            D3
#define RELAY_PIN         D4
#define LDR_PIN           A0

/************************* Misc Configuration *********************************/

#define DHTTYPE           DHT11
#define MAX_CON_RETRY     12
#define CON_RETRY_DELAY   5000

bool DEBUG                = false;  // Indicator that we should print data onto the Serial connection.

bool SENSORS_CONNECTED    = true;   // If set to false we will skip the waiting on, and publishing of all sensor data

short TICK_INTERVAL_MS    = 100;    // The sleep time between runs of the loop() method. This is value is used as the multiplier 
                                    // when configuring the publish interval for temperature and light data.
                                    // NOTE: in addition to configuring the interval for publishing sensor data this sleep time
                                    //       also has a direct effect on the "server.handleClient()" call. The Arduino cannot
                                    //       respond to client requests while executing the delay(). I would suggest making this
                                    //       value as low as possible to get a more responsive web server.

short PUB_INT_TEMP        = 3000;   // Interval to publish the collected sensor data - this is affected by the TICK_INTERVAL_MS.
                                    // Assuming that we have a TICK_INTERVAL_MS of 100ms, and wanting to only publish data every
                                    // 5 min we would need to set this to a value of 3000
                                    //
                                    // 5 min * 60 sec * (1000 / TICK_INTERVAL_MS) => 3000
                                    // 5     * 60     * (1000 / 100)              => 3000
                                    
short MQTT_PING_INT       = 100;    // Interval to ping the MQTT server (a.k.a. client's heartbeat)
                                    // Like the example above (PUB_INT_TEMP) we will need to calculate this value based on the
                                    // TICK_INTERVAL_MS value. If I want to ping the server every 10 seconds I will need to use:
                                    // 
                                    // 10 sec * (1000 / TICK_INTERVAL_MS) => 100

short UPDATE_SENSOR_INT   = 15;     // Interval in which to update the sensor values - these values need to be updated often
                                    // to ensure that the web client provides accurate information.

short tickCountTemp       = PUB_INT_TEMP; // Force sensor data publication on startup :P
short tickCountPing       = 0;
short tickCountSensor     = UPDATE_SENSOR_INT; // Force immediate update of sensor data
const String WWW_ROOT     = "www"; // Root folder to use when looking for HTM files

/************ Global State (you don't need to change this!) ******************/

WiFiClient              client;
ESP8266WebServer        server(80);
Adafruit_MQTT_Client    mqtt(&client, AIO_SERVER, AIO_SERVERPORT);
DHT                     dht(DHTPIN, DHTTYPE);

/*************************** Sketch Code ************************************/

bool dhtReadSuccess       = false;
float humidity            = 0;
float temperature         = 0;
float heatIndex           = 0;
int lightReading          = 0;
bool lightsOn             = false;

// TODO: Add debug methods that make use of the DEBUG flag...

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
  pinMode(RELAY_PIN , OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  // Set initial LED state
  RGB(true, false, false);

  // Boot application
  Serial.println("Home Weather Station 1.0.0");
  
  //initSdCard();
  waitForWiFiConnection();

  if( SENSORS_CONNECTED ) {
    dht.begin();
    waitForDhtReady();
  }

  initSdCard();
  configureWebServer();

  // Setup MQTT subscription for onoff feed.
  //mqtt.subscribe(&ledSubscription);
}

void loop() {
  // Ensure that we have accurate sensor data
  updateReadings();

  // Look for and handle any client requests
  server.handleClient();

  // Attempt to run PING and PUBLISH on MQTT channel
  runPing(false);
  publishData();
  //runLEDTest();

  // Go to sleep for a bit
  delay(TICK_INTERVAL_MS);
}

// ========================================================== >>
// Publishing related methods

void runPing(bool forcePing) {
  // No need to do this if there is no sensor data to publish
  if( !SENSORS_CONNECTED ) return;
  
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
  // If there are no sensors connected skip this
  if( !SENSORS_CONNECTED ) return;
  
  // We only want to publish temperature data every "PUB_INT_TEMP" ticks
  if( tickCountTemp < PUB_INT_TEMP ) {
    tickCountTemp++;
    return;
  }

  // Ensure that we are connected
  runPing(true);

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
// MQTT Methods

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
// Web Server Methods

void handleRoot() {
  String out = "";

  String filePath = WWW_ROOT + "/index.htm";
  Serial.println("Sending File: " + filePath);
  
  File myFile = SD.open(filePath);

  if (myFile) {
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      // Need to cast to CHAR
      out += (char) myFile.read();
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening +" + filePath);
  }
  
  server.send ( 200, "text/html", out );
}

void getStatus() {
  server.send ( 200, "text/html", lightsOn ? "ON" : "OFF" );
}

void handleNotFound() {
  String message = "";
  String filePath = WWW_ROOT + server.uri();
  Serial.println("Attempting to find file: " + filePath);
  int statusCode = 200;
  String contentType = "text/html";
  
  File myFile = SD.open(filePath);

  if (myFile) {
    while (myFile.available()) {
      message += (char) myFile.read();
    }
    myFile.close();

    if( filePath.endsWith(".js") ) {
      contentType = "application/javascript";
    }

    if( filePath.endsWith(".css") ) {
      contentType = "text/css";
    }
    
  } else {
    statusCode = 404;
    
    message += "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";

    for ( uint8_t i = 0; i < server.args(); i++ ) {
      message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
    }
  }
  
  server.send ( statusCode, contentType, message );
}

void turnLightsOn() {
  Serial.println("Turning lights ON");
  digitalWrite(RELAY_PIN, LOW);
  server.send ( 200, "text/html", "ON" );
  lightsOn = true;
}

void turnLightsOff() {
  Serial.println("Turning lights OFF");
  digitalWrite(RELAY_PIN, HIGH);
  server.send ( 200, "text/html", "OFF" );
  lightsOn = false;
}

void handleSensors() {
  String out = "";

  String temp = SENSORS_CONNECTED ? String(temperature, DEC) : "99";
  String heat = SENSORS_CONNECTED ? String(heatIndex, DEC) : "99";
  String humid = SENSORS_CONNECTED ? String(humidity, DEC) : "99";
  String light = SENSORS_CONNECTED ? String(lightReading, DEC) : "99";
  
  out += "{\n";
  out += "  \"temperature\": " + temp + ",\n";
  out += "  \"heatIndex\": " + heat + ",\n";
  out += "  \"humidity\": " + humid + ",\n";
  out += "  \"freeHeap\": " + String(ESP.getFreeHeap(), DEC) + ",\n";
  out += "  \"tickInt\": " + String(TICK_INTERVAL_MS, DEC) + ",\n";
  out += "  \"pubInt\": " + String(PUB_INT_TEMP, DEC) + ",\n";
  out += "  \"pingInt\": " + String(MQTT_PING_INT, DEC) + ",\n";
  out += "  \"updateSensorInt\": " + String(UPDATE_SENSOR_INT, DEC) + ",\n";
  out += "  \"pubTickCount\": " + String(tickCountTemp, DEC) + ",\n";
  out += "  \"pingTickCount\": " + String(tickCountPing, DEC) + ",\n";
  out += "  \"updateTickCount\": " + String(tickCountSensor, DEC) + ",\n";
  out += "  \"light\": " + light + "\n";
  out += "}\n";

  //Serial.println("Sending sensor data to client");
  //Serial.println(out);
  server.send ( 200, "application/json", out );
}

void configureWebServer() {
  if ( MDNS.begin ( "esp8266" ) ) {
    Serial.println ( "MDNS responder started" );
  }

  server.on ( "/", handleRoot );
  server.on ( "/status.htm", getStatus );
  server.on ( "/on.htm", turnLightsOn );
  server.on ( "/off.htm", turnLightsOff );
  server.on ( "/sensors.json", handleSensors );

  server.onNotFound ( handleNotFound );
  server.begin();
  
  Serial.println ( "HTTP server started" );
}


// ========================================================== >>
// SD Card functions
void initSdCard() {
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}


// ========================================================== >>
// Hardware methods

void updateReadings() {
  // If there are no sensors connected, skip this step
  if( !SENSORS_CONNECTED ) return;
  
  // We only want to update readings on the given schedule
  if( tickCountSensor < UPDATE_SENSOR_INT ) {
    tickCountSensor ++;
    return;
  }

  // Call appropriate sensor read methods
  readDhtValues();
  readLdrValue();

  tickCountSensor = 0;
}

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



