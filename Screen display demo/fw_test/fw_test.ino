

#include <Arduino_GFX_Library.h>
#include <SD.h>
#include <TAMC_GT911.h>
#include "BmpClass.h"

#define BMP_FILENAME "/Init.bmp"
#define TFT_BL 2

#define SD_SCK 12
#define SD_MISO 13
#define SD_MOSI 11
#define SD_CS 10

#define TOUCH_SCL 20
#define TOUCH_SDA 19
#define TOUCH_INT -1
#define TOUCH_RST 38
#define TOUCH_ROTATION ROTATION_NORMAL
#define TOUCH_MAP_X1 480
#define TOUCH_MAP_X2 0
#define TOUCH_MAP_Y1 270
#define TOUCH_MAP_Y2 0

Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    GFX_NOT_DEFINED /* CS */, GFX_NOT_DEFINED /* SCK */, GFX_NOT_DEFINED /* SDA */,
    40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
    45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
    5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
    8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */
);

// LCD 800x480
Arduino_RPi_DPI_RGBPanel *gfx = new Arduino_RPi_DPI_RGBPanel(
    bus,
    480 /* width */, 0 /* hsync_polarity */, 8 /* hsync_front_porch */, 4 /* hsync_pulse_width */, 8 /* hsync_back_porch */,
    270 /* height */, 0 /* vsync_polarity */, 8 /* vsync_front_porch */, 4 /* vsync_pulse_width */, 8 /* vsync_back_porch */,
    1 /* pclk_active_neg */, 16000000 /* prefer_speed */, true /* auto_flush */);

static BmpClass bmpClass;

int touch_last_x = 0, touch_last_y = 0;

TAMC_GT911 ts = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST, max(TOUCH_MAP_X1, TOUCH_MAP_X2), max(TOUCH_MAP_Y1, TOUCH_MAP_Y2));

void setup()
{
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    Serial.begin(115200);
    // while (!Serial);
    Serial.println("BMP Image Viewer");

    // Init Display
    gfx->begin();

        SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
    if (!SD.begin(SD_CS))
    {
        Serial.println(F("ERROR: File System Mount Failed!"));
        gfx->println(F("ERROR: File System Mount Failed!"));
    }
    else
    {
        unsigned long start = millis();

        File bmpFile = SD.open(BMP_FILENAME, "r");

        // read BMP file header
        bmpClass.draw(&bmpFile, bmpDrawCallback, false /* useBigEndian */,
                      0 /* x */, 0 /* y */, gfx->width() /* widthLimit */, gfx->height() /* heightLimit */);

        bmpFile.close();

        Serial.print("Time used: ");
        Serial.println(millis() - start);
    }

    delay(1000);

    gfx->fillScreen(WHITE);
    //gfx->fillRect(300, 160, 240, 160, BLACK);

    gfx->setTextSize(1);
    gfx->setTextColor(BLACK);
    gfx->setCursor(450, 210);
    gfx->println("TOUCH");
    gfx->setCursor(10, 220);
    gfx->println("TO");
    gfx->setCursor(10, 260);
    gfx->println("CONTINUE");

    touch_init();

    while (1)
    {
        if (touch_touched())
        {
            if (touch_last_x > 300 && touch_last_x < 540 && touch_last_y > 160 && touch_last_y < 320)
                break;
        }
        delay(10);
    }

}

void loop()
{
    touch_touched();
    delay(50);
}

// pixel drawing callback
static void bmpDrawCallback(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h)
{
    // Serial.printf("Draw pos = %d, %d. size = %d x %d\n", x, y, w, h);
    gfx->draw16bitRGBBitmap(x, y, bitmap, w, h);
}

void touch_init()
{
    Wire.begin(TOUCH_SDA, TOUCH_SCL);
    ts.begin();
    ts.setRotation(TOUCH_ROTATION);
}

bool touch_touched()
{
    ts.read();
    if (ts.isTouched)
    {
        touch_last_x = map(ts.points[0].x, TOUCH_MAP_X1, TOUCH_MAP_X2, 0, 800 - 1);
        touch_last_y = map(ts.points[0].y, TOUCH_MAP_Y1, TOUCH_MAP_Y2, 0, 480 - 1);

         Serial.print("ox = ");
         Serial.print(ts.points[0].x);
         Serial.print(", oy = ");
         Serial.print(ts.points[0].y);
         Serial.print(",x = ");
         Serial.print(touch_last_x);
         Serial.print(", y = ");
         Serial.print(touch_last_y);
         Serial.println();

        return true;
    }
    else
    {
        return false;
    }
}
