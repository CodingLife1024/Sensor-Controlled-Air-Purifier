/*********************************************************************
This version reads real-time PM2.5, PM10 from the Plantower PMS7003
sensor and adjusts the fan speed based on PM2.5. It also displays
the time, PM2.5, PM10, and fan speed on an OLED display.
**********************************************************************/

#include "Plantower_PMS7003.h"
#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "FS.h"
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

#define CS_PIN D0  // CS Pin of the SD Card connects to this in the XIAO ESP32-S3 Board

// Sensor Variables
int pm25 = 0;
int pm10 = 0;

const char *filename = "/data.csv";

// RTC
RTC_DS3231 rtc;
String datetime = "";

// PMS7003 (Using HardwareSerial instead of SoftwareSerial)
HardwareSerial ss(1); // Use Serial1 for PMS7003
Plantower_PMS7003 pms7003;

const int inPin = D2;

// OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// WiFi credentials
const char* ssid = <"your-SSID">; // Replace with your network SSID
const char* password = <"your-PASSWORD">; // Replace with your network password

// Firebase credentials
#define FIREBASE_HOST <"your-project-id.firebaseio.com">
#define FIREBASE_AUTH <"your-database-secret">

// Firebase objects
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

// Speed control variables
const int pwmPin = 4;  // GPIO4 for PWM output
int fanSpeed = 10;

void setup()
{
  // Serial Monitor
  Serial.begin(115200);
  Serial.println("Setup: Initializing system...");
  pinMode(inPin, INPUT_PULLUP);
  attachInterrupt(inPin,0, RISING);

  // PMS7003 Setup
  Serial.println("Setup: Initializing PMS7003 Sensor...");
  ss.begin(9600, SERIAL_8N1, 44, 43); // RX = GPI44, TX = GPI43
  pms7003.init(&ss);
  Serial.println("PMS7003 Sensor initialized.");

  // RTC Setup
  Serial.println("Setup: Initializing RTC...");
  if (!rtc.begin()) {
    Serial.println("Error: Couldn't find RTC!");
    while (1) delay(10);
  }
  Serial.println("RTC initialized successfully.");
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //reset this after every new upload

  // OLED Setup
  Serial.println("Setup: Initializing OLED display...");
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Error: SSD1306 allocation failed!");
    while (1);
  }
  Serial.println("OLED display initialized.");

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Configure Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  // Initialize Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // SD Card Setup
  while (!Serial);  // Wait for Serial to initialize
  Serial.println("Initializing SD card...");
  if (!SD.begin(CS_PIN)) {
      Serial.println("Card Mount Failed");
      return;
  }
  Serial.println("SD card initialized successfully.");

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Setup complete.");
  display.display();
  // Display SD card info
  displaySDCardInfo();

  // Perform read/write performance test
  performReadWriteTest();
  writeToFile(filename, "Time, PM10, PM2.5, Fan");
  delay(1000);

  // AC Dimmer Setup
  Serial.println("Setup: Initializing AC Dimmer...");
  // acd.begin(NORMAL_MODE, ON);
  // Serial.println("AC Dimmer initialized.");

  // Fan Speed control initialisation
  pinMode(pwmPin, OUTPUT);
  // analogWrite(pwmPin, 255);
  Serial.println("Fan control initialized");

  Serial.println("Setup: System ready. Starting loop...");
}

void loop()
{
  Serial.println("\n--- New Loop Iteration ---");

  // Get the current time from RTC
  Serial.println("Getting current time...");
  timeStamp();
  Serial.print("Current Time: ");
  Serial.println(datetime);

  // Get PMS7003 sensor values for PM2.5 and PM10
  Serial.println("Reading PMS7003 sensor values...");
  pmsRead();
  Serial.print("PM2.5: ");
  Serial.print(pm25);
  Serial.print(" µg/m3, PM10: ");
  Serial.print(pm10);
  Serial.println(" µg/m3");

  // Display the data on the OLED
  Serial.println("Updating OLED display...");
  oled(datetime, pm25, pm10, fanSpeed);
  sendData(datetime, pm25, pm10, fanSpeed);

  String line = String(datetime) + "," + String(pm10) + "," + String(pm25) + "," + String(fanSpeed);

  // Print to Serial to check the result
  Serial.println("Formatted line: " + line);

  // Assuming appendToFile function is defined elsewhere
  appendToFile(filename, line.c_str());
  readFromFile(filename);

  // Control fan speed based on PM2.5 levels
  Serial.println("Controlling fan speed...");
  acdimmer(pm25);

  delay(5000); // Wait 2 seconds before the next loop
}

void acdimmer(int pm25)
{
  Serial.print("Adjusting fan speed based on PM2.5: ");
  Serial.println(pm25);

  if (pm25 < 15) {
    fanSpeed = 30; // Very low fan speed
  }
  else if (pm25 >= 15 && pm25 < 50) {
    fanSpeed = 45; // Low fan speed
  }
  else if (pm25 >= 50 && pm25 < 100) {
    fanSpeed = 60; // Moderate fan speed
  }
  else if (pm25 >= 100 && pm25 < 150) {
    fanSpeed = 70; // High fan speed
  }
  else if (pm25 >= 150 && pm25 < 200) {
    fanSpeed = 90; // Very high fan speed
  }
  else {
    fanSpeed = 100; // Maximum fan speed
  }

  int fanSpeedAnalog = map(fanSpeed, 0, 100, 0, 255);
  analogWrite(pwmPin, fanSpeed);
  Serial.print("PWM value on GPIO4 (D3): ");
  Serial.println(fanSpeed);

  Serial.print("Fan Speed Set To: ");
  Serial.print(fanSpeed);
  Serial.println("%)");
}

void timeStamp()
{
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  DateTime now = rtc.now(); // Fetch the current time from the RTC
  datetime = String(now.year()) + "|" + String(now.month()) + "|" + String(now.day()) + "::"; // Format: YYYY/MM/DD
  datetime += String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()); // Format: HH:MM:SS

  // Log the updated time for debugging purposes
  Serial.print("Timer Reset: ");
  Serial.println(datetime);
}

void pmsRead()
{
  Serial.println("PMS7003: Waiting for new data...");
  for (int i = 0; i < 300; i++) {
    pms7003.updateFrame();
    if (pms7003.hasNewData()) {
      pm25 = pms7003.getPM_2_5();
      pm10 = pms7003.getPM_10_0();
      Serial.println("PMS7003: New data received.");
      return;
    }
  }
  Serial.println("PMS7003: Warning - No new data received!");
}

// Function to send JSON data
void sendData(String dateTime, int pm25, int pm10, int fanSpeed) {
  unsigned long timestamp = millis();  // Unique timestamp

  // Construct the Firebase path
  String path = "/data/" + dateTime;

  // Use FirebaseJson instead of ArduinoJson
  FirebaseJson json;
  json.set("timestamp", dateTime);
  json.set("pm25", pm25);
  json.set("pm10", pm10);
  json.set("fanSpeed", fanSpeed);

  // Send JSON data to Firebase
  if (Firebase.setJSON(firebaseData, path, json)) {
    Serial.println("Data added successfully:");
    Serial.println(json.raw());  // Print JSON data for debugging
  } else {
    Serial.println("Failed to add data: " + firebaseData.errorReason());
  }
}

void oled(String dateTime, int pm25, int pm10, int fanSpeed)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Display Date and Time
  display.setCursor(0, 0);
  display.println(dateTime);

  // Display PM2.5 and PM10 values
  display.setCursor(0, 10);
  display.print("PM2.5: ");
  display.print(pm25);
  display.print(", PM10: ");
  display.println(pm10);

  // Display Fan Speed
  display.setCursor(0, 20);
  display.print("Fan Speed: ");
  display.println(fanSpeed);

  display.display();
  Serial.println("OLED: Display updated.");
}

// Function to display SD card information
void displaySDCardInfo() {
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE) {
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("Card Type: ");
    switch (cardType) {
        case CARD_MMC:
            Serial.println("MMC");
            break;
        case CARD_SD:
            Serial.println("SDSC");
            break;
        case CARD_SDHC:
            Serial.println("SDHC");
            break;
        default:
            Serial.println("UNKNOWN");
    }

    uint64_t totalBytes = SD.totalBytes();
    uint64_t usedBytes = SD.usedBytes();

    Serial.printf("Total space: %lluMB\n", totalBytes / (1024 * 1024));
    Serial.printf("Used space: %lluMB\n", usedBytes / (1024 * 1024));
}

// Function to test read/write performance using smaller chunks
void performReadWriteTest() {
    const size_t chunkSize = 512;  // Use smaller chunk size
    char buffer[chunkSize];        // Buffer for data operations

    // Write data in chunks
    Serial.println("Starting write test...");
    File file = SD.open("/performance.txt", FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }

    unsigned long startTime = millis();
    for (size_t i = 0; i < (1024 * 1024) / chunkSize; i++) {
        memset(buffer, 'A', chunkSize);  // Fill buffer with data
        file.write((uint8_t *)buffer, chunkSize);
    }
    unsigned long writeTime = millis() - startTime;
    file.close();

    Serial.printf("1 MB written in %lu ms\n", writeTime);

    // Read data in chunks
    Serial.println("Starting read test...");
    file = SD.open("/performance.txt", FILE_READ);
    if (!file) {
        Serial.println("Failed to open file for reading");
        return;
    }

    startTime = millis();
    while (file.read((uint8_t *)buffer, chunkSize) > 0) {
        // Optionally process the buffer data here
    }
    unsigned long readTime = millis() - startTime;
    file.close();

    Serial.printf("1 MB read in %lu ms\n", readTime);
}

// Function to write a line to the file
void writeToFile(const char *filename, const char *line) {
    Serial.printf("Writing to file: %s\n", filename);
    File file = SD.open(filename, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }

    file.println(line);  // Write the given line to the file
    file.close();        // Close the file
    Serial.println("Line written successfully.");
}

// Function to append a line to the file
void appendToFile(const char *filename, const char *line) {
    Serial.printf("Appending to file: %s\n", filename);
    File file = SD.open(filename, FILE_APPEND);  // Open file in append mode
    if (!file) {
        Serial.println("Failed to open file for appending");
        return;
    }

    file.println(line);  // Append the given line to the file
    file.close();        // Close the file
    Serial.println("Line appended successfully.");
}

// Function to read and display the contents of the file
void readFromFile(const char *filename) {
    Serial.printf("Reading from file: %s\n", filename);
    File file = SD.open(filename, FILE_READ);
    if (!file) {
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.println("File contents:");
    while (file.available()) {
        char c = file.read();  // Read a character
        Serial.print(c);       // Print it to the Serial Monitor
    }
    file.close();              // Close the file
    Serial.println("\nFile reading completed.");
}

void clearSDCard() {
    Serial.println("Clearing SD card...");
    File root = SD.open("/");
    if (!root) {
        Serial.println("Failed to open root directory.");
        return;
    }
    deleteContents(root);
    Serial.println("SD card cleared successfully.");
}

void deleteContents(File dir) {
    File entry;
    while (entry = dir.openNextFile()) {
        if (entry.isDirectory()) {
            deleteContents(entry);  // Recursive call for directories
            if (SD.rmdir(entry.name())) {
                Serial.printf("Directory removed: %s\n", entry.name());
            } else {
                Serial.printf("Failed to remove directory: %s\n", entry.name());
            }
        } else {
            if (SD.remove(entry.name())) {
                Serial.printf("File removed: %s\n", entry.name());
            } else {
                Serial.printf("Failed to remove file: %s\n", entry.name());
            }
        }
        entry.close();
    }
}
