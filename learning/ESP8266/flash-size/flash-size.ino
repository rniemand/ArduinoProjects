#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

// https://github.com/SensorsIot/Internet-of-Things-with-ESP8266/blob/master/BVB_WebConfig_OTA_V7/BVB_WebConfig_OTA_V7.ino

const char* host = "esp8266-ota";
const uint16_t aport = 8266;
WiFiServer TelnetServer(aport);
WiFiClient Telnet;
WiFiUDP OTA;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Serial.println("Flash Chip Size");
  Serial.println(ESP.getFlashChipSize());

  ConfigureWifi();
  otaInit();
}

void loop() {
  // put your main code here, to run repeatedly:
  otaReceive();
}


void ConfigureWifi()
{
  Serial.println("Configuring Wifi");

  WiFi.begin ("PickMe", "fallout312345");

  //WiFi.begin (config.ssid.c_str(), config.password.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    // led(red);
    delay(500);
  }
  //if (!config.dhcp)
  //{
    //WiFi.config(IPAddress(config.IP[0], config.IP[1], config.IP[2], config.IP[3] ),  IPAddress(config.Gateway[0], config.Gateway[1], config.Gateway[2], config.Gateway[3] ) , IPAddress(config.Netmask[0], config.Netmask[1], config.Netmask[2], config.Netmask[3] ));
  //}
}

void otaInit() {
  //for (int i = 0; i < 3; i++) beep(3);
  WiFi.mode(WIFI_AP);
  ConfigureWifi();
  MDNS.begin(host);
  MDNS.addService("arduino", "tcp", aport);
  OTA.setPassword((const char *)"123");
  OTA.begin(aport);
  TelnetServer.begin();
  TelnetServer.setNoDelay(true);
  Serial.print("IP address: ");
  //led(green);
  Serial.println(WiFi.localIP());
  Serial.println("OTA settings applied");
}

void otaReceive() {
  if (OTA.parsePacket()) {
    IPAddress remote = OTA.remoteIP();
    int cmd  = OTA.parseInt();
    int port = OTA.parseInt();
    int size   = OTA.parseInt();

    Serial.print("Update Start: ip:");
    Serial.print(remote);
    Serial.printf(", port:%d, size:%d\n", port, size);
    uint32_t startTime = millis();

    WiFiUDP::stopAll();

    if (!Update.begin(size)) {
      Serial.println("Update Begin Error");
      return;
    }

    WiFiClient client;
    if (client.connect(remote, port)) {

      uint32_t written;
      while (!Update.isFinished()) {
        written = Update.write(client);
        if (written > 0) client.print(written, DEC);
      }
      Serial.setDebugOutput(false);

      if (Update.end()) {
        client.println("OK");
        Serial.printf("Update Success: %u\nRebooting...\n", millis() - startTime);
        ESP.restart();
      } else {
        Update.printError(client);
        Update.printError(Serial);
      }
    } else {
      Serial.printf("Connect Failed: %u\n", millis() - startTime);
    }
  }
  //IDE Monitor (connected to Serial)
  
  if (TelnetServer.hasClient()) {
    if (!Telnet || !Telnet.connected()) {
      if (Telnet) Telnet.stop();
      Telnet = TelnetServer.available();
    } else {
      WiFiClient toKill = TelnetServer.available();
      toKill.stop();
    }
  }
  
  if (Telnet && Telnet.connected() && Telnet.available()) {
    while (Telnet.available())
      Serial.write(Telnet.read());
  }
  
  if (Serial.available()) {
    size_t len = Serial.available();
    uint8_t * sbuf = (uint8_t *)malloc(len);
    Serial.readBytes(sbuf, len);
    if (Telnet && Telnet.connected()) {
      Telnet.write((uint8_t *)sbuf, len);
      yield();
    }
    free(sbuf);
  }
}


