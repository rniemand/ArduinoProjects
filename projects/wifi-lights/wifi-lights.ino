#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char *ssid = "PickMe";
const char *password = "fallout312345";
short RELAY_PIN = D6;
bool lightsOn = false;

ESP8266WebServer server ( 80 );


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

  server.send ( 200, "text/css", out);
}

void getJs() {
  String out = "";
  out += "var cb = document.getElementById('tvlights');\n";
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
  out += "ajax('status.html', updateBtn);\n";

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
  out += "  </style>\n";
  out += "</head>\n";
  out += "\n";
  out += "<body>\n";
  out += "  <h1>Lights</h1>\n";
  out += "\n";
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
  out += "</body>\n";
  out += "\n";
  out += "<script src=\"lights.js\"></script>\n";
  out += "\n";
  out += "</html>\n";
  
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

  server.send ( 404, "text/plain", message );
}

void setup ( void ) {
  pinMode(RELAY_PIN , OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  Serial.begin ( 9600 );
  WiFi.begin ( ssid, password );
  Serial.println ( "" );

  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );

  if ( MDNS.begin ( "esp8266" ) ) {
    Serial.println ( "MDNS responder started" );
  }

  server.on ( "/", handleRoot );
  server.on ( "/lights.css", getCss );
  server.on ( "/lights.js", getJs );
  server.on ( "/status.html", getStatus );
  server.on ( "/on.html", turnLightsOn );
  server.on ( "/off.html", turnLightsOff );
  
  /*
  server.on ( "/inline", []() {
    server.send ( 200, "text/plain", "this works as well" );
  } );
  */
  
  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println ( "HTTP server started" );
}

void turnLightsOn() {
  digitalWrite(RELAY_PIN, LOW);
  server.send ( 200, "text/html", "ON" );
  lightsOn = true;
}

void turnLightsOff() {
  digitalWrite(RELAY_PIN, HIGH);
  server.send ( 200, "text/html", "OFF" );
  lightsOn = false;
}

void loop ( void ) {
  server.handleClient();
}


