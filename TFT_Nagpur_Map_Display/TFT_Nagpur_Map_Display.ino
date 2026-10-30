
#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <TJpg_Decoder.h>
#include <XPT2046_Touchscreen.h>

#define TFT_CS     5
#define TFT_DC     27
#define TFT_RST    33
#define TOUCH_CS   22
#define TOUCH_IRQ  21
#define SD_CS      25

#define TFT_SCLK   18
#define TFT_MISO   19
#define TFT_MOSI   23

#define SCREEN_W   320
#define SCREEN_H   240
#define TILE_SIZE  256

#define START_ZOOM 15
#define START_X    23579
#define START_Y    14411

// Faster SD SPI clock; start at 10 MHz for reliability.
#define SD_SPI_HZ 10000000

Adafruit_ST7789 tft(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen ts(TOUCH_CS, TOUCH_IRQ);

int zoomLevel = START_ZOOM;
int64_t mapX = (int64_t)START_X * TILE_SIZE;
int64_t mapY = (int64_t)START_Y * TILE_SIZE;

bool wasTouching = false;
bool zoomHandled = false;

int lastX = 0;
int lastY = 0;

uint32_t lastDraw = 0;
bool mapDirty = false;

// Draw decoded JPEG blocks directly on TFT.
bool jpgOutput(int16_t x, int16_t y,
               uint16_t w, uint16_t h,
               uint16_t* bitmap) {
  tft.drawRGBBitmap(x, y, bitmap, w, h);
  return true;
}

// Use your tested calibration directly; no averaging.
bool readTouch(int &sx, int &sy) {
  if (!ts.touched()) return false;

  TS_Point p = ts.getPoint();

  sx = map(p.x, 3820, 290, 0, SCREEN_W - 1);
  sy = map(p.y, 3720, 350, 0, SCREEN_H - 1);

  sx = constrain(sx, 0, SCREEN_W - 1);
  sy = constrain(sy, 0, SCREEN_H - 1);

  return true;
}

void drawTile(int tx, int ty, int px, int py) {
  char path[64];

  snprintf(path, sizeof(path),
           "/NagpurMap/%d/%d/%d.jpg",
           zoomLevel, tx, ty);

  if (!SD.exists(path)) {
    Serial.print("Missing tile: ");
    Serial.println(path);
    return;
  }

  // The decoder can return FAILED even when blocks appeared.
  // Don't print this warning on every redraw.
  TJpgDec.drawSdJpg(px, py, path);
}

void drawControls() {
  tft.fillRoundRect(8, 8, 42, 34, 5, ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(3);
  tft.setCursor(20, 13);
  tft.print("+");

  tft.fillRoundRect(58, 8, 42, 34, 5, ST77XX_BLACK);
  tft.setCursor(73, 13);
  tft.print("-");

  tft.fillRect(218, 8, 94, 25, ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setCursor(225, 17);
  tft.printf("ZOOM %d", zoomLevel);
}

void drawMap() {
  // Avoid touch bus activity during tile decoding.
  digitalWrite(TOUCH_CS, HIGH);
  digitalWrite(SD_CS, HIGH);
  digitalWrite(TFT_CS, HIGH);

  tft.fillScreen(ST77XX_WHITE);

  int64_t tileX0 = mapX / TILE_SIZE;
  int64_t tileY0 = mapY / TILE_SIZE;

  int offX = mapX - tileX0 * TILE_SIZE;
  int offY = mapY - tileY0 * TILE_SIZE;

  // Only tiles that can overlap the screen.
  for (int row = 0; row < 2; row++) {
    for (int col = 0; col < 3; col++) {
      int tx = (int)tileX0 + col;
      int ty = (int)tileY0 + row;

      int px = col * TILE_SIZE - offX;
      int py = row * TILE_SIZE - offY;

      drawTile(tx, ty, px, py);
    }
  }

  drawControls();
  Serial.printf("Map drawn at zoom %d\n", zoomLevel);
  lastDraw = millis();
  mapDirty = false;
}

void changeZoom(int direction) {
  int nextZoom = zoomLevel + direction;
  if (nextZoom < 10 || nextZoom > 15) return;

  if (direction > 0) {
    mapX *= 2;
    mapY *= 2;
  } else {
    mapX /= 2;
    mapY /= 2;
  }

  zoomLevel = nextZoom;
  mapDirty = true;
}

void setup() {
  Serial.begin(115200);
  delay(300);

  pinMode(TFT_CS, OUTPUT);
  pinMode(TOUCH_CS, OUTPUT);
  pinMode(SD_CS, OUTPUT);

  digitalWrite(TFT_CS, HIGH);
  digitalWrite(TOUCH_CS, HIGH);
  digitalWrite(SD_CS, HIGH);

  SPI.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, SD_CS);

  tft.init(240, 320);
  tft.setRotation(1);
  tft.sendCommand(ST77XX_INVOFF);
  tft.fillScreen(ST77XX_BLACK);

  Serial.println("Nagpur Offline Map Viewer");

  if (!SD.begin(SD_CS, SPI, SD_SPI_HZ)) {
    Serial.println("SD initialization failed at 10 MHz.");
    Serial.println("If needed, reduce SD_SPI_HZ to 4000000.");
    tft.setTextColor(ST77XX_RED);
    tft.setCursor(10, 30);
    tft.setTextSize(2);
    tft.println("SD ERROR");
    return;
  }

  Serial.println("SD card initialized at 10 MHz.");

  ts.begin(SPI);
  ts.setRotation(1);

  TJpgDec.setJpgScale(1);
  TJpgDec.setCallback(jpgOutput);

  drawMap();
}

void loop() {
  int x, y;
  bool touching = readTouch(x, y);

  if (!touching) {
    wasTouching = false;

    if (mapDirty) {
      drawMap();
    }
    return;
  }

  if (!wasTouching) {
    wasTouching = true;
    zoomHandled = false;
    lastX = x;
    lastY = y;

    if (y >= 8 && y <= 42) {
      if (x >= 8 && x <= 50) {
        changeZoom(+1);
        zoomHandled = true;
      } else if (x >= 58 && x <= 100) {
        changeZoom(-1);
        zoomHandled = true;
      }
    }
    return;
  }

  if (zoomHandled) return;

  int dx = x - lastX;
  int dy = y - lastY;

  lastX = x;
  lastY = y;

  if (abs(dx) + abs(dy) >= 2) {
    mapX -= dx;
    mapY -= dy;
    mapDirty = true;
  }

  // Redraw no more often than every 120 ms.
  if (mapDirty && millis() - lastDraw >= 120) {
    drawMap();
  }
}
