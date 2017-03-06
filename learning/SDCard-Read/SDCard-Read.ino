// Required for web server
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Required for SD Card
#include <SPI.h>
#include <SD.h>

#define DEBUG_MODE      true

const char *ssid        = "PickMe";
const char *password    = "fallout312345";
const String WWW_ROOT   = "www";

ESP8266WebServer server ( 80 );

void setup() {
  setupSerial();

  startWifi();
  initSdCard();

  //Serial.println("index found " + String(SD.exists("index.html")));
  
  //writeToSd();
  //readFromSd();
  //writeToSd2();

  /*
  File root = SD.open("/");
  //printDirectory(root, 0);

  Serial.println("--------");
  File entry =  root.openNextFile();
  Serial.print(entry.name());
  Serial.println("--------");
  */
  

  server.on ( "/", handleRoot );
  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println ( "HTTP server started" );
}

void loop() {
  server.handleClient();
}

void setupSerial() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  if( !DEBUG_MODE ) {
    return;
  }
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}


void initSdCard() {
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}

void writeToSd() {
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void writeToSd2() {
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File myFile = SD.open("index.htm", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to index.html...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening index.html");
  }
}

void readFromSd() {
  // re-open the file for reading:
  File myFile = SD.open("test.txt");
  
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void startWifi() {
  Serial.print("Attempting to connect to: ");
  Serial.println(ssid);
  
  WiFi.begin ( ssid, password );

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
}


void handleNotFound() {
  String filePath = WWW_ROOT + server.uri();

  Serial.println("Attempting to send file: " + filePath);
  
  String message = "";
  int code = 200;
  String contentType = "text/plain";

  if( !SD.exists(filePath) ) {
    code = 404;
    
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
  } else {
    File myFile = SD.open(filePath);
  
    if (myFile) {
      while (myFile.available()) {
        message += (char) myFile.read();
      }
      myFile.close();
    } else {
      Serial.println("error opening +" + filePath);
    }
  }
  
  server.send ( code, contentType, message );
}

void handleRoot() {
  String out = "";
  //Serial.print(F("FreeRam: "));
  //Serial.println(ESP.getFreeHeap());

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

void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}



