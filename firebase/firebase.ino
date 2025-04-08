/*********************************************************************
This version reads real-time PM2.5, PM10 from the Plantower PMS7003
sensor and adjusts the fan speed based on PM2.5. It also displays
the time, PM2.5, PM10, and fan speed on an OLED display.
**********************************************************************/

#include <WiFi.h>
#include <FirebaseESP32.h>

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

void setup()
{
    // Serial Monitor
    Serial.begin(115200);
    Serial.println("Setup: Initializing system...");

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
}

void loop(){
    sendData(datetime, pm25, pm10, fanSpeed);

    delay(5000); // Wait 5 seconds before the next loop
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
    }
    else {
        Serial.println("Failed to add data: " + firebaseData.errorReason());
    }
}