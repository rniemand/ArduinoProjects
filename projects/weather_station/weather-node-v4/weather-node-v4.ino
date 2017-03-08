#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DHT.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi and MQTT Config *********************************/

#define WLAN_SSID       "PickMe"
#define WLAN_PASS       "fallout312345"
#define AIO_SERVER      "mqtt.thingspeak.com"
#define AIO_SERVERPORT  1883
#define CHANNEL_ID      ""
#define PUBLISH_KEY     ""

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

/************ Global State (you don't need to change this!) ******************/

WiFiClient              client;
ESP8266WebServer        server ( 80 );
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

  dht.begin();
  waitForWiFiConnection();
  waitForDhtReady();
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
  // We only want to publish temperature data every "PUB_INT_TEMP" ticks
  if( tickCountTemp < PUB_INT_TEMP ) {
    tickCountTemp++;
    return;
  }

  // Ensure that we are connected
  runPing(true);

  // NOTE: data is being refreshed in the loop going forward...
  //// Refresh our readings
  //readLdrValue();
  //readDhtValues();

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

void getCss() {
  String out = "";

  out += "* {\n";
  out += "  font-family: 'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;\n";
  out += "  font-size: 1.05em;\n";
  out += "}\n";
  out += "\n";
  out += "body {\n";
  out += "  text-align: center;\n";
  out += "}\n";
  out += "\n";
  out += "h1 {\n";
  out += "  font-size: 2em;\n";
  out += "}\n";
  out += "\n";
  out += ".controls {\n";
  out += "  display: inline-block;\n";
  out += "  width: 200px;\n";
  out += "}\n";
  out += "\n";
  out += "/* The switch - the box around the slider */\n";
  out += ".switch {\n";
  out += "  position: relative;\n";
  out += "  display: inline-block;\n";
  out += "  width: 60px;\n";
  out += "  height: 34px;\n";
  out += "}\n";
  out += "\n";
  out += "/* Hide default HTML checkbox */\n";
  out += ".switch input {display:none;}\n";
  out += "\n";
  out += "/* The slider */\n";
  out += ".slider {\n";
  out += "  position: absolute;\n";
  out += "  cursor: pointer;\n";
  out += "  top: 0;\n";
  out += "  left: 0;\n";
  out += "  right: 0;\n";
  out += "  bottom: 0;\n";
  out += "  background-color: #ccc;\n";
  out += "  -webkit-transition: .4s;\n";
  out += "  transition: .4s;\n";
  out += "}\n";
  out += "\n";
  out += ".slider:before {\n";
  out += "  position: absolute;\n";
  out += "  content: \"\";\n";
  out += "  height: 26px;\n";
  out += "  width: 26px;\n";
  out += "  left: 4px;\n";
  out += "  bottom: 4px;\n";
  out += "  background-color: white;\n";
  out += "  -webkit-transition: .4s;\n";
  out += "  transition: .4s;\n";
  out += "}\n";
  out += "\n";
  out += "input:checked + .slider {\n";
  out += "  background-color: #2196F3;\n";
  out += "}\n";
  out += "\n";
  out += "input:focus + .slider {\n";
  out += "  box-shadow: 0 0 1px #2196F3;\n";
  out += "}\n";
  out += "\n";
  out += "input:checked + .slider:before {\n";
  out += "  -webkit-transform: translateX(26px);\n";
  out += "  -ms-transform: translateX(26px);\n";
  out += "  transform: translateX(26px);\n";
  out += "}\n";
  out += "\n";
  out += "/* Rounded sliders */\n";
  out += ".slider.round {\n";
  out += "  border-radius: 34px;\n";
  out += "}\n";
  out += "\n";
  out += ".slider.round:before {\n";
  out += "  border-radius: 50%;\n";
  out += "}\n";
  out += "\n";
  out += ".sensor_value td:first-child {\n";
  out += "  font-weight: bold;\n";
  out += "  color: #2819d1;\n";
  out += "  text-align: left;\n";
  out += "  padding-right: 5px;\n";
  out += "}\n";
  out += "\n";
  out += ".sensor_value td:nth-child(2) {\n";
  out += "  width: 80px !important;\n";
  out += "  text-align: right;\n";
  out += "}\n";
  out += "\n";
  out += ".sensors {\n";
  out += "  display: inline-block;\n";
  out += "  width: 300px;\n";
  out += "}\n";
  out += "\n";
  out += ".sensors tbody {\n";
  out += "  width: 300px;\n";
  out += "}\n";

  server.send ( 200, "text/css", out);
}

void getJs() {
  String out = "";

  out += "var cb = document.getElementById('tvlights');\n";
  out += "\n";
  out += "var $ = function (id) {\n";
  out += "  return document.getElementById(id);\n";
  out += "}\n";
  out += "\n";
  out += "var ajax = function (url, callback) {\n";
  out += "  var xhttp = new XMLHttpRequest();\n";
  out += "  xhttp.onreadystatechange = function () {\n";
  out += "    if (this.readyState == 4 && this.status == 200) {\n";
  out += "      callback(this.responseText);\n";
  out += "    }\n";
  out += "  };\n";
  out += "  xhttp.open(\"GET\", url, true);\n";
  out += "  xhttp.send();\n";
  out += "};\n";
  out += "\n";
  out += "var updateBtn = function (x) {\n";
  out += "  cb.checked = x === 'ON';\n";
  out += "};\n";
  out += "\n";
  out += "function toggleLights() {\n";
  out += "  ajax((cb.checked ? 'on.html' : 'off.html'), updateBtn);\n";
  out += "}\n";
  out += "\n";
  out += "function getSensorData() {\n";
  out += "  ajax('sensors.json', function (data) {\n";
  out += "    data = JSON.parse(data);\n";
  out += "    $('humidity').innerText = data.humidity + ' %';\n";
  out += "    $('temp').innerText = data.temperature + ' * C';\n";
  out += "    $('heat').innerText = Math.round(data.heatIndex) + ' * C';\n";
  out += "    $('light').innerText = data.light;\n";
  out += "    setTimeout(getSensorData, 5000);\n";
  out += "  });\n";
  out += "}\n";
  out += "\n";
  out += "// Kick everything off\n";
  out += "ajax('status.html', updateBtn);\n";
  out += "getSensorData();\n";

  server.send ( 200, "application/javascript", out);
}

void handleRoot() {
  String out = "";
  
  out += "<!DOCTYPE html>\n";
  out += "<html lang=\"en\">\n";
  out += "\n";
  out += "<head>\n";
  out += "  <meta charset=\"UTF-8\">\n";
  out += "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  out += "  <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\">\n";
  out += "  <title>Lights</title>\n";
  out += "  <link rel=\"stylesheet\" href=\"lights.css\">\n";
  out += "  <style>\n";
  out += "\n";
  out += "  </style>\n";
  out += "</head>\n";
  out += "\n";
  out += "<body>\n";
  out += "  <h1>Lights</h1>\n";
  out += "  <table class=\"controls\">\n";
  out += "    <tr>\n";
  out += "      <td>\n";
  out += "        <label class=\"switch\">\n";
  out += "          <input type=\"checkbox\" id=\"tvlights\" onClick=\"toggleLights()\">\n";
  out += "          <div class=\"slider round\"></div>\n";
  out += "        </label>\n";
  out += "      </td>\n";
  out += "      <td>\n";
  out += "        TV Lights\n";
  out += "      </td>\n";
  out += "    </tr>\n";
  out += "  </table>\n";
  out += "\n";
  out += "  <h1>Sensors</h1>\n";
  out += "  <table class=\"sensors\">\n";
  out += "    <tr class=\"sensor_value\">\n";
  out += "      <td>Humidity:</td>\n";
  out += "      <td id=\"humidity\">.</td>\n";
  out += "    </tr>\n";
  out += "    <tr class=\"sensor_value\">\n";
  out += "      <td>Temperature:</td>\n";
  out += "      <td id=\"temp\">.</td>\n";
  out += "    </tr>\n";
  out += "    <tr class=\"sensor_value\">\n";
  out += "      <td>Heat Index:</td>\n";
  out += "      <td id=\"heat\">.</td>\n";
  out += "    </tr>\n";
  out += "    <tr class=\"sensor_value\">\n";
  out += "      <td>Light Reading:</td>\n";
  out += "      <td id=\"light\">.</td>\n";
  out += "    </tr>\n";
  out += "  </table>\n";
  out += "\n";
  out += "  <script src=\"lights.js\"></script>\n";
  out += "</body>\n";
  out += "\n";
  out += "</html>\n";

  Serial.println("Serving up new web page to client");
  server.send ( 200, "text/html", out );
}

void getStatus() {
  server.send ( 200, "text/html", lightsOn ? "ON" : "OFF" );
}

void handleNotFound() {
  String message = "File Not Found\n\n";
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

  Serial.println(message);
  server.send ( 404, "text/plain", message );
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
  
  out += "{\n";
  out += "  \"temperature\": " + String(temperature, DEC) + ",\n";
  out += "  \"heatIndex\": " + String(heatIndex, DEC) + ",\n";
  out += "  \"humidity\": " + String(humidity, DEC) + ",\n";
  out += "  \"light\": " + String(lightReading, DEC) + "\n";
  out += "}\n";

  Serial.println("Sending sensor data to client");
  Serial.println(out);
  server.send ( 200, "application/json", out );
}

void configureWebServer() {
  if ( MDNS.begin ( "esp8266" ) ) {
    Serial.println ( "MDNS responder started" );
  }

  server.on ( "/", handleRoot );
  server.on ( "/lights.css", getCss );
  server.on ( "/lights.js", getJs );
  server.on ( "/status.html", getStatus );
  server.on ( "/on.html", turnLightsOn );
  server.on ( "/off.html", turnLightsOff );
  server.on ( "/sensors.json", handleSensors );

  server.onNotFound ( handleNotFound );
  server.begin();
  
  Serial.println ( "HTTP server started" );
}


// ========================================================== >>
// Hardware methods

void updateReadings() {
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



