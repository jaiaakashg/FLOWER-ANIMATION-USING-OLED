#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void drawPetal(int x, int y, int size, float angle, int width);
void drawFlower(int x, int y, int size, int petalCount);
void drawLeaf(int x, int y, int size, float angle);
void drawStem(int x, int y, int length);
void animateFlower();

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.display();
}

void loop() {
  animateFlower();
  delay(50);
}

void drawPetal(int x, int y, int size, float angle, int width) {
  float cosAngle = cos(angle);
  float sinAngle = sin(angle);
  int petalHeight = size / 2;

  for (int i = -width; i <= width; i++) {
    for (int j = -petalHeight; j <= petalHeight; j++) {
      int px = x + i * cosAngle - j * sinAngle;
      int py = y + i * sinAngle + j * cosAngle;
      if (px >= 0 && px < SCREEN_WIDTH && py >= 0 && py < SCREEN_HEIGHT) {
        display.drawPixel(px, py, SSD1306_WHITE);
      }
    }
  }
}

void drawFlower(int x, int y, int size, int petalCount) {
  float angleStep = 2 * PI / petalCount;
  int petalWidth = size / 5;

  for (int layer = 0; layer < 3; layer++) {
    int layerSize = size - layer * 5;
    for (int i = 0; i < petalCount; i++) {
      float angle = i * angleStep + (layer * angleStep / 2);
      drawPetal(x, y, layerSize, angle, petalWidth);
    }
  }

  display.fillCircle(x, y, size / 6, SSD1306_WHITE);  
}

void drawLeaf(int x, int y, int size, float angle) {
  float cosAngle = cos(angle);
  float sinAngle = sin(angle);
  int leafWidth = size / 3;
  int leafHeight = size / 2;

  for (int i = -leafWidth; i <= leafWidth; i++) {
    for (int j = -leafHeight; j <= leafHeight; j++) {
      int px = x + i * cosAngle - j * sinAngle;
      int py = y + i * sinAngle + j * cosAngle;
      if (px >= 0 && px < SCREEN_WIDTH && py >= 0 && py < SCREEN_HEIGHT) {
        display.drawPixel(px, py, SSD1306_WHITE);
      }
    }
  }
}

void drawStem(int x, int y, int length) {
  for (int i = 0; i < length; i++) {
    int px = x;
    int py = y + i;
    if (px >= 0 && px < SCREEN_WIDTH && py >= 0 && py < SCREEN_HEIGHT) {
      display.drawPixel(px, py, SSD1306_WHITE);
    }
  }
}

void animateFlower() {
  static int frame = 0;
  display.clearDisplay();
  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2;
  int maxSize = 25;
  int petalCount = 12;
  int stemLength = 20;
  int size = (sin(frame * 0.1) * (maxSize / 2)) + (maxSize / 2);
  int oscillation = 5 * sin(frame * 0.05);
  drawStem(centerX, centerY + size / 2, stemLength);
  drawLeaf(centerX - 15, centerY + size / 2 + stemLength - 5, 10, -PI / 4);
  drawLeaf(centerX + 15, centerY + size / 2 + stemLength - 5, 10, PI / 4);
  drawFlower(centerX + oscillation, centerY, size, petalCount);

  display.display();
  frame++;
}

