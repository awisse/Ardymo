#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include "EEPROM.h"
#include "../Ardymo/defines.h"
#include "../Ardymo/controller.h"
#include "../Ardymo/game.h"
#include "../Ardymo/platform.h"
#include "Font5x7.h"

#define ZOOM_SCALE 4

SDL_Window* AppWindow = nullptr;
SDL_Renderer* AppRenderer = nullptr;
SDL_Surface* AppSurface = nullptr;
uint8_t sBuffer[kScreenWidth * kScreenHeight / 8];
EEPROM eeprom;
unsigned long StartTime;
int kZoomScale;
bool textRawMode;
#ifdef DEBUG_
int counter;
#endif // DEBUG_

uint8_t InputMask = 0;
void cleanup();

//private:
size_t write(uint8_t c); // Write one character at the cursor.
size_t write(const char str[]); // Write a string at the cursor.
size_t printNumber(uint32_t n, int base);
size_t printFloat(float x, int decimals);
void formatFloat(char* strfloat, float x, int decimals);
uint8_t previousButtonState, currentButtonState;
struct {
  int16_t x, y;
} cursor;

// Helper
void SetColour(uint8_t colour) {

  if (colour == COLOUR_WHITE) {
    SDL_SetRenderDrawColor(AppRenderer, 255, 255, 255, 255);
  } else {
    SDL_SetRenderDrawColor(AppRenderer, 0, 0, 0, 255);
  }
}

// From Platform.h
// General
uint8_t* Platform::getBuffer() {
  return sBuffer;
}

// (I2C) Communication. 
// Maybe move to sockets/UDP for PC-to-PC communication
uint8_t Platform::slave_receive(uint8_t* bytes, uint8_t n) {
  return 0;
}
void Platform::slave_send(uint8_t* bytes, uint8_t n) {
}
uint8_t Platform::master_receive(uint8_t* bytes, uint8_t n) {
  return 0;
}
uint8_t Platform::master_request(uint8_t address, uint8_t bytes) {
  // Receive bytes from slave at address
  // This is unfinished. Not needed for now.
  uint8_t n {};
  return n;
}
I2CErrors Platform::master_send(uint8_t* bytes, uint8_t n, uint8_t address) {
  return I2C_TIMEOUT;
}

// Buttons
uint8_t Platform::buttonsState()
{
  return InputMask;
}

bool Platform::pressed(uint8_t buttons) {
  return (buttonsState() & buttons) == buttons;
}

bool Platform::notPressed(uint8_t buttons) {
  return (buttonsState() & buttons) == 0;
}

void Platform::pollButtons(void) {

  previousButtonState = currentButtonState;
  currentButtonState = buttonsState();
}

bool Platform::justPressed(uint8_t button) {

  return (!(previousButtonState & button) && (currentButtonState & button));
}

bool Platform::justReleased(uint8_t button) {

  return ((previousButtonState & button) && !(currentButtonState & button));

}

// Drawing
void Platform::drawPixel(int16_t x, int16_t y, uint8_t colour) {

  // Draw to sBuffer
  // If outside of screen nothing to do
  if ((x<0) || (x>=kScreenWidth) || (y<0) || (y>=kScreenHeight)) {
    return;
  }
  SetColour(colour);
  if ((SDL_RenderDrawPoint(AppRenderer, x, y) < 0)) {
    std::cerr << "drawPixel:RenderDrawPoint" << SDL_GetError() << "\n";
  }
}

void drawPixel(int16_t x, int16_t y, uint8_t colour) {

  // Draw to renderer
  // If outside of screen nothing to do
  if ((x<0) || (x>=kScreenWidth) || (y<0) || (y>=kScreenHeight)) {
    return;
  }
  SetColour(colour);
  if ((SDL_RenderDrawPoint(AppRenderer, x, y) < 0)) {
    std::cerr << "drawPixel:RenderDrawPoint" << SDL_GetError() << "\n";
  }
}

void Platform::drawBitmap(int16_t x, int16_t y, const uint8_t* bitmap,
    uint8_t w, uint8_t h, uint8_t colour)
{
  for (int j = 0; j < h; j++)
  {
    for (int i = 0; i < w; i++)
    {
      int blockX = i / 8;
      int blockY = j / 8;
      int blocksPerWidth = w / 8;
      int blockIndex = blockY * blocksPerWidth + blockX;
      uint8_t pixels = bitmap[blockIndex * 8 + i % 8];
      uint8_t mask = 1 << (j % 8);
      if (pixels & mask)
      {
        drawPixel(x + i, y + j, colour);
      }
    }
  }
}

void Platform::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
    uint8_t colour) {

  SetColour(colour);

  if (SDL_RenderDrawLine(AppRenderer, x0, y0, x1, y1)) {
    std::cout << SDL_GetError() << "\n";
  }
}

void Platform::drawFastVLine(int16_t x0, int16_t y0, uint8_t h, uint8_t colour) {
  drawLine(x0, y0, x0, y0 + h, colour);
}

void Platform::drawFastHLine(int16_t x0, int16_t y0, uint8_t w, uint8_t colour) {
  drawLine(x0, y0, x0 + w, y0, colour);
}

void Platform::drawRect(int16_t x, int16_t y, uint8_t w, uint8_t h) {
  SDL_Rect rect;

  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;

  SetColour(COLOUR_WHITE);

  if (SDL_RenderDrawRect(AppRenderer, &rect) < 0) {
    std::cout << SDL_GetError() << "\n";
  }
}

void Platform::fillRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t colour) {
  SDL_Rect rect;

  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;

  SetColour(colour);

  if (SDL_RenderFillRect(AppRenderer, &rect) < 0) {
    std::cout << SDL_GetError() << "\n";
  }
}

void Platform::drawCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t colour) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawPixel(x0, y0+r, colour);
  drawPixel(x0, y0-r, colour);
  drawPixel(x0+r, y0, colour);
  drawPixel(x0-r, y0, colour);

  while (x<y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }

    x++;
    ddF_x += 2;
    f += ddF_x;

    drawPixel(x0 + x, y0 + y, colour);
    drawPixel(x0 - x, y0 + y, colour);
    drawPixel(x0 + x, y0 - y, colour);
    drawPixel(x0 - x, y0 - y, colour);
    drawPixel(x0 + y, y0 + x, colour);
    drawPixel(x0 - y, y0 + x, colour);
    drawPixel(x0 + y, y0 - x, colour);
    drawPixel(x0 - y, y0 - x, colour);
  }
}

void Platform::fillCircle(int16_t x0, int16_t y0, uint8_t r,
    uint8_t colour) {

  SetColour(colour);

  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  // Horizontal center line
  drawLine(x0-r, y0, x0+r, y0, colour);

  while (x<y)
  {
    if (f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }

    x++;
    ddF_x += 2;
    f += ddF_x;

    drawLine(x0+x, y0-y, x0-x, y0-y, colour);
    drawLine(x0+y, y0-x, x0-y, y0-x, colour);
    drawLine(x0+x, y0+y, x0-x, y0+y, colour);
    drawLine(x0+y, y0+x, x0-y, y0+x, colour);
  }
}

void Platform::fillScreen(uint8_t colour) {

  // Set screen buffer to colour, too
  SetColour(colour);

  if (SDL_RenderClear(AppRenderer)) {
    std::cout << SDL_GetError() << "\n";
  }
}

void Platform::clear() {
  fillScreen(COLOUR_BLACK);
}

// Drawing desktop
void Platform::display(bool clear_screen) {
  if (clear_screen) clear();
  SDL_RenderPresent(AppRenderer);
}

/************* Drawing Optimized ******************************************/

void Platform::eraseRectRow(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
  // This function is "optimized" on the Arduboy. This is a simple replication
  // of its effects.
  uint8_t row, rows;

  // Don't write outside of the buffer. These are uint's. No sign to check.
  if ((x > kScreenWidth - 1) || (y > kScreenHeight - 1 ))
    return;

  // For the ardusdl version, compute rectangle and draw it

  row = y & 0xF8;
  rows = ((y + height) | 0x07) - row + 1;

  // Truncate
  width = (width > kScreenWidth - x) ? kScreenWidth - x : width;
  rows = (rows > kScreenHeight - row) ? kScreenHeight - row : rows;

  fillRect(x, row, width, rows, COLOUR_BLACK);

}

/******************** Timer *********************************************/
uint32_t Platform::millis() {
  struct timespec ts;
  uint32_t ms;

  if (clock_gettime(CLOCK_REALTIME, &ts)) {
    std::cerr << "Can't get clock_gettime" << "\n";
  }
  ms = (1000 * ts.tv_sec  + ts.tv_nsec / 1000000) - StartTime;
  return ms;
}

void Platform::delay(uint32_t ms) {
  SDL_Delay(ms);
}
/******************** Text Functions *************************************/
void Platform::setTextRawMode(bool raw) {
  textRawMode = raw;
}

void Platform::setCursor(int16_t x, int16_t y) {
  cursor.x = x;
  cursor.y = y;
}

int16_t Platform::getCursorX(void) {
  return cursor.x;
}

int16_t Platform::getCursorY(void) {
  return cursor.y;
}

uint8_t Platform::getFullCharWidth(void) {
  return kFontWidth + kFontSpacing;
}

uint8_t Platform::getLineHeight(void) {
  return kFontHeight + kLineSpacing;
}

/******************** Print **********************************************/
size_t Platform::print(const char str[]) {
  return write(str);
}

size_t Platform::print(char c) {
  return write(c);
}

size_t Platform::print(unsigned char c) {
  return write(c);
}

// Careful: `int` is 16 bits on Arduino, but 32 bits on a regular computer.
size_t Platform::print(int16_t n, uint8_t base) {
  // Conversion to 16 bits

  return print((int32_t)n, base);
}

size_t Platform::print(uint16_t n, uint8_t base) {
  return print((uint32_t)n, base);
}

// Careful: `long` is 32 bits on Arduino, but 64 bits on a regular computer.
size_t Platform::print(int32_t n, uint8_t base) {
  size_t t = 0;


  if ((n < 0) && (base == 10)) {
    t = write('-');
    cursor.x += kFontWidth + kFontSpacing;
    n = -n;
  }

  t += printNumber(n, base);

  return t;
}

size_t Platform::print(uint32_t n, uint8_t base) {
  size_t t = printNumber(n, base);
  return t;
}

size_t Platform::print(float x, uint8_t decimals) {
  return printFloat(x, decimals);
}

size_t Platform::print(double x, uint8_t decimals) {
  return printFloat(float(x), decimals);
}

uint8_t Platform::print_P(const char s[]) {
  uint8_t pos {0};
  print(s);
  while (s[pos++]);
  return pos;
}

size_t Platform::println(void) {
  return write('\n');
}

size_t Platform::println(const char str[]) {
  size_t t;
  t = print(str);
  t += println();
  return t;
}

size_t Platform::println(char c) {
  size_t t;
  t = print(c);
  t += println();
  return t;
}

size_t Platform::println(unsigned char c) {
  return print((char)c);
}

size_t Platform::println(int16_t n, uint8_t base) {
  size_t t;
  t = print(n, base);
  t += println();
  return t;
}

size_t Platform::println(uint16_t n, uint8_t base) {
  size_t t;
  t = print(n, base);
  t += println();
  return t;
}

size_t Platform::println(int32_t n, uint8_t base) {
  size_t t;
  t = print(n, base);
  t += println();
  return t;
}

size_t Platform::println(uint32_t n, uint8_t base) {
  size_t t;
  t = print(n, base);
  t += println();
  return t;
}

size_t Platform::println(float x, uint8_t decimals) {
  size_t t = printFloat(x, decimals);
  t += println();
  return t;
}

size_t Platform::println(double x, uint8_t decimals) {
  size_t t = printFloat((float)x, decimals);
  t += println();
  return t;
}

//
#if defined (DEBUG_) || defined (TIMER_)
void Platform::DebugPrint(int16_t value, uint8_t base) {

  std::cout << value;
  std::cout.flush();
}

void Platform::DebugPrint(uint8_t value, uint8_t base) {
  std::cout << value;
  std::cout.flush();
}

void Platform::DebugPrint(uint16_t value, uint8_t base) {

  std::cout << value;
  std::cout.flush();
}

void Platform::DebugPrint(uint32_t value, uint8_t base) {
  std::cout << value;
  std::cout.flush();
}

void Platform::DebugPrint(float value, uint8_t decimals) {
  char strfloat[64];
  formatFloat(strfloat, value, decimals);
  std::cout << strfloat;
  std::cout.flush();
}

void Platform::DebugPrint(double value, uint8_t decimals) {
  char strdouble[64];
  formatFloat(strdouble, value, decimals);
  std::cout << strdouble;
  std::cout.flush();
}

void Platform::DebugPrint(const char* text) {
  std::cout << text;
  std::cout.flush();
}

void Platform::DebugPrintln(uint8_t value, uint8_t base) {
  std::cout << value << '\n';
  std::cout.flush();
}

void Platform::DebugPrintln(int16_t value, uint8_t base) {

  std::cout << value << '\n';
  std::cout.flush();
}

void Platform::DebugPrintln(uint16_t value, uint8_t base) {
  std::cout << value << '\n';
  std::cout.flush();
}

void Platform::DebugPrintln(uint32_t value, uint8_t base) {
  std::cout << value << '\n';
  std::cout.flush();
}

void Platform::DebugPrintln(float value, uint8_t decimals) {
  char strfloat[64];
  formatFloat(strfloat, value, decimals);
  std::cout << strfloat << '\n';
  std::cout.flush();
}

void Platform::DebugPrintln(double value, uint8_t decimals) {
  char strdouble[64];
  formatFloat(strdouble, value, decimals);
  std::cout << strdouble << '\n';
  std::cout.flush();
}

void Platform::DebugPrintln(const char* text) {
  std::cout << text << '\n';
  std::cout.flush();
}

void Platform::DebugPrintln(void) {
  std::cout << "\n";
  std::cout.flush();
}

#endif // defined (DEBUG_) || defined (_TARGET)


// TODO: EEPROM
/* SavedState Platform::ToEEPROM(const uint8_t *bytes, */
/*     const uint16_t offset, const uint16_t sz) { */

/*   if (offset < 0) { */
/*     return WrongOffset; */
/*   } */

/*   int insertAt = offset + EEPROM_STORAGE_SPACE_START; */

/*   if (insertAt + sz > eeprom.length()) { */
/*     return TooBig; */
/*   } */

/*   eeprom.write(bytes, insertAt, sz); */
/*   return Saved; */
/* } */

/* SavedState Platform::FromEEPROM(uint8_t *bytes, const uint16_t offset, */
/*     const uint16_t sz) { */
/*   int getFrom = offset + EEPROM_STORAGE_SPACE_START; */

/*   if (getFrom < 0) { */
/*     return WrongOffset; */
/*   } */
/*   if (getFrom + sz > eeprom.length()) { */
/*     return TooBig; */
/*   } */

/*   eeprom.read(bytes, getFrom, sz); */
/*   return Saved; */
/* } */

// From Game.h
int Random(int i0, int i1) {
  int r;

  if (i0 == i1) return i0;

  r = ((int)random() & 0xFFFFFFFF) % (i1 - i0);

  return i0 + r;
}

// Local Functions
//
void Initialize() {
  struct timespec ts;
  memset(sBuffer, 0, sizeof(sBuffer));
  // Initialize timer from start of program
  if (clock_gettime(CLOCK_REALTIME, &ts)) {
    std::cerr << "Can't get clock_gettime" << "\n";
  }
  StartTime = 1000 * ts.tv_sec + ts.tv_nsec / 1000000;
  // Initialize Arduboy Game in game.h
  initGame();
  // Initialize random number generator.
  srandom(StartTime);
}

int main(int argc, char* argv[])
{
  kZoomScale = ZOOM_SCALE;
  if (argc == 2) {
    kZoomScale = atoi(argv[1]);
    if ((kZoomScale < 1) || (kZoomScale > 8)) {
      kZoomScale = ZOOM_SCALE;
      std::cerr << "Zoom must be between 1 and 8. Using "\
        << kZoomScale <<"\n";
    }
  }

  SDL_Init(SDL_INIT_VIDEO);

  SDL_CreateWindowAndRenderer(kScreenWidth * kZoomScale, kScreenHeight * kZoomScale,
      SDL_WINDOW_RESIZABLE, &AppWindow, &AppRenderer);
  SDL_RenderSetLogicalSize(AppRenderer, kScreenWidth, kScreenHeight);
  AppSurface = SDL_GetWindowSurface(AppWindow);

  Initialize();

  bool running = true;
  InputMask = 0;

  while (running)
  {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {


        case SDL_QUIT:
          running = false;
          break;

        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {

            case SDLK_a:
              InputMask |= INPUT_A;
              break;

            case SDLK_b:
              InputMask |= INPUT_B;
              break;

            case SDLK_LEFT:
              InputMask |= INPUT_LEFT;
              break;

            case SDLK_RIGHT:
              InputMask |= INPUT_RIGHT;
              break;

            case SDLK_UP:
              InputMask |= INPUT_UP;
              break;

            case SDLK_DOWN:
              InputMask |= INPUT_DOWN;
              break;

            case SDLK_ESCAPE:
              running = false;
              break;

          }
          break;

        case SDL_KEYUP:
          switch (event.key.keysym.sym) {

            case SDLK_a:
              InputMask &= ~INPUT_A;
              break;

            case SDLK_b:
              InputMask &= ~INPUT_B;
              break;

            case SDLK_LEFT:
              InputMask &= ~INPUT_LEFT;
              break;

            case SDLK_RIGHT:
              InputMask &= ~INPUT_RIGHT;
              break;

            case SDLK_UP:
              InputMask &= ~INPUT_UP;
              break;

            case SDLK_DOWN:
              InputMask &= ~INPUT_DOWN;
              break;
          }
          break;
        }
      }

    stepGame();

    /* if (!eeprom.isSaved()) { */
    /*   eeprom.save(); */
    /* } */


    // FrameRate
    SDL_Delay(kFrameDuration);

  }

  cleanup();
  return 0;
}

void cleanup() {
  SDL_DestroyRenderer(AppRenderer);
  SDL_DestroyWindow(AppWindow);
  AppRenderer = nullptr;
  AppWindow = nullptr;
  SDL_Quit();
}

size_t write(uint8_t c) {

  // Erase rectangle first
  Platform::fillRect(cursor.x, cursor.y,
      kFontWidth + kFontSpacing, kFontHeight + kLineSpacing, COLOUR_BLACK);

  if (!textRawMode) {
    switch (c) {
    case 0xa:
      cursor.x = 0;
      cursor.y += kFontHeight + kLineSpacing;
      return 0;
    case 0xd:
      return 1;
    }
  }

  Platform::drawBitmap(cursor.x, cursor.y, &font5x7[kFontWidth * c], 5, 8);
  return 1;
}

size_t write(const char str[]) { // Write a string at the cursor.
  int char_pos = 0, t = 0;
  char c;

  while ((c=str[char_pos++]) && (cursor.x < kScreenWidth)) {
    t += write(c);
    cursor.x += kFontWidth + kFontSpacing;
  }
  return t;
}

size_t printNumber(uint32_t n, int base)
{
  char buf[8 * sizeof(uint32_t) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];

  // Fill buffer from right to left
  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

  do {
    char c = n % base;
    n /= base;

    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);

  return write(str);
}

size_t printFloat(float x, int decimals)
{
  size_t t;
  char strfloat[64]; // Resulting string

  formatFloat(strfloat, x, decimals);
  t = Platform::print(strfloat);

  return t;
}

void formatFloat(char* strfloat, float x, int decimals) {
  char format[6]; // Format string
  char fmt_tpl[] = "%%.%df";

  sprintf(format, fmt_tpl, decimals);
  sprintf(strfloat, format, x);
}
// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
