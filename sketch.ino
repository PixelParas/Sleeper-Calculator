#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  display.clearDisplay();

  // Example: draw a diagonal line
  for (int i = 0; i < SCREEN_HEIGHT && i < SCREEN_WIDTH; i++) {
    setPixel(i, i);
  }

  updateDisplayFromBuffer();
}

void loop() {
  // Add logic to update buffer if needed
}
