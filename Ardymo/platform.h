#pragma once

#include <stdint.h>
#include "defines.h"

// A namespace with static functions which are used in the code and implemented
// depending on the platform: Arduboy or PC. The implementations for the
// PC versions are in ../Desktop/ardusdl.cpp
class Platform
{
public:
  // General
  static uint8_t* getBuffer();

  // (I2C) Communication
  static uint8_t slave_receive(uint8_t* bytes, uint8_t n);
  static void slave_send(uint8_t* bytes, uint8_t n);
  static uint8_t master_request(uint8_t address, uint8_t n);
  static uint8_t master_receive(uint8_t* bytes, uint8_t n);
  static I2CErrors master_send(uint8_t* bytes, uint8_t n, uint8_t address);

  // Buttons
  static uint8_t buttonsState();
  static bool pressed(uint8_t buttons);
  static bool notPressed(uint8_t buttons);
  static void pollButtons(void);
  static bool justPressed(uint8_t button);
  static bool justReleased(uint8_t button);

  // Drawing Arduboy
  static void drawPixel(int16_t x, int16_t y, uint8_t colour=COLOUR_WHITE);
  static void drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap,
    uint8_t w, uint8_t h, uint8_t colour=COLOUR_WHITE);
  static void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
    uint8_t colour=COLOUR_WHITE);
  static void drawFastVLine(int16_t x0, int16_t y0, uint8_t h,
      uint8_t colour=COLOUR_WHITE);
  static void drawFastHLine(int16_t x0, int16_t y0, uint8_t w,
      uint8_t colour=COLOUR_WHITE);
  static void drawRect(int16_t x, int16_t y, uint8_t w, uint8_t h);
  static void fillRect(int16_t x, int16_t y, uint8_t w, uint8_t h,
    uint8_t colour=COLOUR_WHITE);
  static void drawCircle(int16_t x0, int16_t y0, uint8_t r,
    uint8_t colour=COLOUR_WHITE);
  static void fillCircle(int16_t x0, int16_t y0, uint8_t r,
    uint8_t colour=COLOUR_WHITE);
  static void fillScreen(uint8_t colour);
  static void clear();
  static void display(bool clear=false);

  // Drawing optimized
  static void eraseRectRow(uint8_t x, uint8_t y, uint8_t width, uint8_t height);

  // Timer
  static uint32_t millis();
  static void delay(uint32_t ms);

  // Text Functions
  static void setTextRawMode(bool raw);
  static void setCursor(int16_t x, int16_t y);
  static int16_t getCursorX(void);
  static int16_t getCursorY(void);
  static uint8_t getFullCharWidth(void); // With character spacing
  static uint8_t getLineHeight(void);

  // Print
  static size_t print(const char str[]);
  static size_t print(char c);
  static size_t print(unsigned char c);
  // Careful: `long` on Arduino is 16 bits, but 32 bits on a regular computer.
  static size_t print(int16_t x, uint8_t base=DEC);
  static size_t print(uint16_t x, uint8_t base=DEC);
  // Careful: `long` on Arduino is 32 bits, but 64 bits on a regular computer.
  static size_t print(int32_t x, uint8_t base=DEC);
  static size_t print(uint32_t x, uint8_t base=DEC);
  // Float conversion is not the same with the Arduino `Print` class and
  // the conversion on a desktop computer. The rounding approach
  // in the private method `Print::printFloat` is different from
  // standard C.
  static size_t print(float x, uint8_t decimals=2);
  // Float and double is the same on Arduboy:
  static size_t print(double x, uint8_t decimals=2);
  // Print string from PROGMEN
  static uint8_t print_P(const char str[]);

  static size_t println(void);
  static size_t println(const char str[]);
  static size_t println(char c);
  static size_t println(unsigned char c);
  static size_t println(int16_t x, uint8_t base=DEC);
  static size_t println(uint16_t x, uint8_t base=DEC);
  static size_t println(int32_t x, uint8_t base=DEC);
  static size_t println(uint32_t x, uint8_t base=DEC);
  static size_t println(float x, uint8_t decimals=2);
  static size_t println(double x, uint8_t decimals=2);


  // EEPROM

  // Debug
#if defined (DEBUG_) || defined (TIMER_)
  static void DebugPrint(uint8_t value, uint8_t base=DEC);
  static void DebugPrint(int16_t value, uint8_t base=DEC);
  static void DebugPrint(uint16_t value, uint8_t base=DEC);
  static void DebugPrint(uint32_t value, uint8_t base=DEC);
  static void DebugPrint(float value, uint8_t decimals=2);
  static void DebugPrint(double value, uint8_t decimals=2);
  static void DebugPrint(const char* text);
  static void DebugPrintln(uint8_t value, uint8_t base=DEC);
  static void DebugPrintln(int16_t value, uint8_t base=DEC);
  static void DebugPrintln(uint16_t value, uint8_t base=DEC);
  static void DebugPrintln(uint32_t value, uint8_t base=DEC);
  static void DebugPrintln(float value, uint8_t decimals=2);
  static void DebugPrintln(double value, uint8_t decimals=2);
  static void DebugPrintln(const char* text);
  static void DebugPrintln();
#endif // DEBUG_ || TIMER_
	
#ifndef ARDUINO

#endif
};
// vim:ft=cpp
