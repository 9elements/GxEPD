/************************************************************************************
   class GxGDEW0213Z16 : Display class example for GDEW0213Z16 e-Paper from Dalian Good Display Co., Ltd.: www.good-display.com

   based on Demo Example from Good Display, available here: http://www.good-display.com/download_detail/downloadsId=529.html

   Author : J-M Zingg

   modified by :

   Version : 2.2

   Support: limited, provided as example, no claim to be fit for serious use

   Controller: maybe IL0398 : http://www.good-display.com/download_detail/downloadsId=537.html

   connection to the e-Paper display is through DESTM32-S2 connection board, available from GoodDisplay

   DESTM32-S2 pinout (top, component side view):
         |-------------------------------------------------
         |  VCC  |o o| VCC 5V  not needed
         |  GND  |o o| GND
         |  3.3  |o o| 3.3     3.3V
         |  nc   |o o| nc
         |  nc   |o o| nc
         |  nc   |o o| nc
   MOSI  |  DIN  |o o| CLK     SCK
   SS    |  CS   |o o| DC      e.g. D3
   D4    |  RST  |o o| BUSY    e.g. D2
         |  nc   |o o| BS      GND
         |-------------------------------------------------

*/
#ifndef _GxGDEW0213Z16_H_
#define _GxGDEW0213Z16_H_

#include "../GxEPD.h"

#define GxGDEW0213Z16_WIDTH 104
#define GxGDEW0213Z16_HEIGHT 212

#define GxGDEW0213Z16_BUFFER_SIZE (uint32_t(GxGDEW0213Z16_WIDTH) * uint32_t(GxGDEW0213Z16_HEIGHT) / 8)

// divisor for AVR, should be factor of GxGDEW0213Z16_HEIGHT
#define GxGDEW0213Z16_PAGES 8 // not a factor, workaround needed

#define GxGDEW0213Z16_PAGE_HEIGHT (GxGDEW0213Z16_HEIGHT / (GxGDEW0213Z16_PAGES - 1))
#define GxGDEW0213Z16_PAGE_SIZE (GxGDEW0213Z16_BUFFER_SIZE / (GxGDEW0213Z16_PAGES - 1))

// mapping example from Waveshare 2.7inch e-Paper to Wemos D1 mini
// BUSY -> D2, RST -> D4, DC -> D3, CS -> D8, CLK -> D5, DIN -> D7, GND -> GND, 3.3V -> 3.3V

// mapping example for AVR, UNO, NANO etc.
// BUSY -> 7, RST -> 9, DC -> 8, C S-> 10, CLK -> 13, DIN -> 11

class GxGDEW0213Z16 : public GxEPD
{
  public:
#if defined(ESP8266)
    GxGDEW0213Z16(GxIO& io, uint8_t rst = D4, uint8_t busy = D2);
#else
    GxGDEW0213Z16(GxIO& io, uint8_t rst = 9, uint8_t busy = 7);
#endif
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void init(void);
    void fillScreen(uint16_t color); // to buffer
    void update(void);
    // to buffer, may be cropped, drawPixel() used, update needed
    void  drawBitmap(const uint8_t *bitmap, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, int16_t mode = bm_normal);
    // to full screen, filled with white if size is less, no update needed, black  /white / red, for example bitmaps
    void drawExamplePicture(const uint8_t* black_bitmap, const uint8_t* red_bitmap, uint32_t black_size, uint32_t red_size);
    // to full screen, filled with white if size is less, no update needed, black  /white / red, general version
    void drawPicture(const uint8_t* black_bitmap, const uint8_t* red_bitmap, uint32_t black_size, uint32_t red_size, int16_t mode = bm_normal);
    // to full screen, filled with white if size is less, no update needed
    void drawBitmap(const uint8_t *bitmap, uint32_t size, int16_t mode = bm_normal); // only bm_normal, bm_invert, bm_partial_update modes implemented
    void eraseDisplay(bool using_partial_update = false);
    // partial update of rectangle from buffer to screen, does not power off
    void updateWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool using_rotation = true);
    // partial update of rectangle at (xs,ys) from buffer to screen at (xd,yd), does not power off
    void updateToWindow(uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h, bool using_rotation = true);
    // terminate cleanly updateWindow or updateToWindow before removing power or long delays
    void powerDown();
    // paged drawing, for limited RAM, drawCallback() is called GxGDEW0213Z16_PAGES times
    // each call of drawCallback() should draw the same
    void drawPaged(void (*drawCallback)(void));
    void drawPaged(void (*drawCallback)(uint32_t), uint32_t);
    void drawPaged(void (*drawCallback)(const void*), const void*);
    void drawPaged(void (*drawCallback)(const void*, const void*), const void*, const void*);
    // paged drawing to screen rectangle at (x,y) using partial update
    void drawPagedToWindow(void (*drawCallback)(void), uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void drawPagedToWindow(void (*drawCallback)(uint32_t), uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t);
    void drawPagedToWindow(void (*drawCallback)(const void*), uint16_t x, uint16_t y, uint16_t w, uint16_t h, const void*);
    void drawPagedToWindow(void (*drawCallback)(const void*, const void*), uint16_t x, uint16_t y, uint16_t w, uint16_t h, const void*, const void*);
    void drawCornerTest(uint8_t em = 0x01);
  private:
    void _writeToWindow(uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h, bool using_rotation = true);
    uint16_t _setPartialRamArea(uint16_t x, uint16_t y, uint16_t xe, uint16_t ye);
    void _writeData(uint8_t data);
    void _writeCommand(uint8_t command);
    void _wakeUp();
    void _sleep();
    void _waitWhileBusy(const char* comment = 0);
    void _rotate(uint16_t& x, uint16_t& y, uint16_t& w, uint16_t& h);
  private:
#if defined(__AVR)
    uint8_t _black_buffer[GxGDEW0213Z16_PAGE_SIZE];
    uint8_t _red_buffer[GxGDEW0213Z16_PAGE_SIZE];
#else
    uint8_t _black_buffer[GxGDEW0213Z16_BUFFER_SIZE];
    uint8_t _red_buffer[GxGDEW0213Z16_BUFFER_SIZE];
#endif
    GxIO& IO;
    int16_t _current_page;
    bool _using_partial_mode;
    uint8_t _rst;
    uint8_t _busy;
#if defined(ESP8266) || defined(ESP32)
  public:
    // the compiler of these packages has a problem with signature matching to base classes
    void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color)
    {
      Adafruit_GFX::drawBitmap(x, y, bitmap, w, h, color);
    };
#endif
};

#define GxEPD_Class GxGDEW0213Z16

#define GxEPD_WIDTH GxGDEW0213Z16_WIDTH
#define GxEPD_HEIGHT GxGDEW0213Z16_HEIGHT
#define GxEPD_BitmapExamples <GxGDEW0213Z16/BitmapExamples.h>
#define GxEPD_BitmapExamplesQ "GxGDEW0213Z16/BitmapExamples.h"

#endif
