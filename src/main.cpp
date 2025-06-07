#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "FullSizeFont.h"
#include <WiFi.h>

#define SCREEN_WIDTH 124
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Screen buffer of size 992 bytes (124 * 64 / 8)
uint8_t screenBuffer[SCREEN_WIDTH * SCREEN_HEIGHT / 8] = {0};

// Function to set a pixel in the buffer
void setPixel(int x, int y) {
  if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
  int page = y / 8;
  int index = x + page * SCREEN_WIDTH;
  screenBuffer[index] |= (1 << (y % 8));
}

std::string text = "7307286329"; // Example text
void setText(){
    for(int i = 0; i < text.length(); i++) {
        std::string character = std::string(1, text[i]);
        if (fullSizeFont.find(character) != fullSizeFont.end()) {
            auto& charData = fullSizeFont[character];
            for (int y = 0; y < 9; y++) {
                for (int x = 0; x < 5; x++) {
                    if (charData[y][x] == 1) {
                        setPixel((i*6)+ x, y); // Adjust position for spacing
                    }
                }
            }
        }else{
            // Chracter not found in font map, handle accordingly
        }
    }
}

// Function to clear a pixel
void clearPixel(int x, int y) {
  if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
  int page = y / 8;
  int index = x + page * SCREEN_WIDTH;
  screenBuffer[index] &= ~(1 << (y % 8));
}

// Copy our buffer to the SSD1306 and display it
void updateDisplayFromBuffer() {
  display.clearDisplay();
  for (int page = 0; page < 8; page++) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
      int index = x + page * SCREEN_WIDTH;
      display.drawFastVLine(x, page * 8, 8, 0); // Clear vertical strip
      for (int bit = 0; bit < 8; bit++) {
        if (screenBuffer[index] & (1 << bit)) {
          display.drawPixel(x, page * 8 + bit, SSD1306_WHITE);
        }
      }
    }
  }
  display.display();
}

void setup() {
  Serial.begin(115200);

  // const char* ssid = "Wokwi-GUEST";
  // const char* password = "";

  // Serial.print("Connecting to ");
  // Serial.println(ssid);

  // WiFi.begin(ssid, password);

  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }

  // Serial.println("WiFi connected");
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());


  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  display.clearDisplay();

  setText();

  updateDisplayFromBuffer();
}

void loop() {
  // Add logic to update buffer if needed
}
