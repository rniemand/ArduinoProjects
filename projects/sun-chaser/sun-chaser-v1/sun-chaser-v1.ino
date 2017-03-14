#include <DHT.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define DHTTYPE           DHT11
#define DHTPIN            3
#define LDRPIN            A0

const int chipSelect      = 10;

Sd2Card                   card;
DHT                       dht(DHTPIN, DHTTYPE);
File                      dataFile;
Adafruit_PCD8544          display(7, 6, 5, 4, 8); // SCLK, DIN, DC, CS, RST

String data_sep           = ",";
String dataFileName       = "DATA.TXT";
int lightReading          = 0;
float maxVoltage          = 18.0;
float maxReadVoltage      = 3.6;
float readingSteps        = maxReadVoltage / 1023;
float voltage             = 0;
int rawVoltage            = 0;
long _millis              = 0;
long readingNumber        = 0;

void setup() {
  Serial.begin(9600);
  delay(10);
  pinMode(A1, INPUT);

  Serial.println("Setting up SD Card");
  initializeSdCard();
  
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
}

void loop() {
  readLdrValue();
  readVoltage();
  _millis = millis();
  readingNumber ++;

  writeCurrentDataToFile();
  showValues();

  delay(10000);
}

void readLdrValue() {
  lightReading = analogRead(LDRPIN);
}

void initializeSdCard() {
  Serial.println("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("ERROR!");
    return;
  }
}

void writeToDataFile(String content) {
  dataFile = SD.open(dataFileName, FILE_WRITE);
  Serial.println(content);

  if (dataFile) {
    dataFile.println(content);
    dataFile.close();
  } else {
    Serial.println("error writing!");
  }
}

void readVoltage() {
  rawVoltage = analogRead(A1);
  float currentReadingVoltage = rawVoltage * readingSteps;
  float aa = (currentReadingVoltage / maxReadVoltage) * 100;
  float bob = (aa * maxVoltage) / 100;
  voltage = bob;
}

void writeCurrentDataToFile() {
  String data = String(_millis, DEC);
  data += data_sep;

  data += String(readingNumber, DEC);
  data += data_sep;

  data += String(lightReading, DEC);
  data += data_sep;

  data += String(rawVoltage, DEC);
  data += data_sep;

  data += String(voltage, DEC);
  data += data_sep;

  writeToDataFile(data);
}

void showValues() {
  display.clearDisplay();
  
  display.print("LDR: ");
  display.println(lightReading);
  display.print("V: ");
  display.println(voltage);
  display.print("Vr: ");
  display.println(rawVoltage);
  display.print("R: ");
  display.println(readingNumber);
  display.print("M: ");
  display.println(_millis);

  display.display();
}







