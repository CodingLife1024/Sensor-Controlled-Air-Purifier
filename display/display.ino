/*********************************************************************
This code displays real-time PM2.5, PM10 from the Plantower PMS7003
sensor and adjusts the fan speed based on PM2.5. It also displays
the time, PM2.5, PM10, and fan speed on an OLED display.
**********************************************************************/

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup()
{
  // Serial Monitor
  Serial.begin(115200);
  Serial.println("Setup: Initializing system...");

  // OLED Setup
  Serial.println("Setup: Initializing OLED display...");
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Error: SSD1306 allocation failed!");
    while (1);
  }
  Serial.println("OLED display initialized.");

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Setup complete.");
  display.display();
}

void loop()
{
    // Display the data on the OLED
    Serial.println("Updating OLED display...");
    oled(datetime, pm25, pm10, fanSpeed);

    String line = String(datetime) + "," + String(pm10) + "," + String(pm25) + "," + String(fanSpeed);

    // Print to Serial to check the result
    Serial.println("Formatted line: " + line);
    delay(5000); // Wait 5 seconds before the next loop
}

void oled(String dateTime, int pm25, int pm10, int fanSpeed){
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