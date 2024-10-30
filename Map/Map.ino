/* vim: ts=2:sts=2:expandtab
 *
 * Drawing and Rotating an arrow
 *
 */

#include <Arduboy2.h>
#include <stdint.h>
#include "comm.h"
#include "game.h"
#include "defines.h"
#include "platform.h"
#ifdef USE_I2C
#include <Wire.h>
#endif

Arduboy2 arduboy;

void setup() {
  // put your setup code here, to run once:

  arduboy.begin();
#ifdef DEBUG_
  Serial.begin(9600);
#endif

#ifdef USE_I2C
  // Map is the slave
  power_twi_enable();
  Wire.begin(I2C_SLAVE_ADDR);
  Wire.onReceive(I2C_SlaveReceive);
  Wire.onRequest(I2C_MasterRequest);
#endif

  // Wait for button to be pressed before beginning
  while (!arduboy.pressed(A_BUTTON)) {
    arduboy.idle();
  }

  arduboy.setFrameDuration(kFrameDuration);
  InitGame();
}

void loop() {

  if (arduboy.nextFrame()) {
    StepGame();
  }
}

/******* Begin Platform Class Function Implementations ********/

// General
uint8_t* Platform::getBuffer() {
  return arduboy.getBuffer();
}

// I2C Communication
uint8_t Platform::slave_receive(uint8_t* bytes, uint8_t n) {
  // Receive bytes from master
  uint8_t received = 0; // Number of bytes received
  int16_t available = Wire.available();

  while ((received < n) && (received < available)) {
    bytes[received++] = Wire.read();
  }
  return received;
}

void Platform::slave_send(uint8_t* bytes, uint8_t n) {
  // Send n bytes to master
  if (n > 32) {
    n = 32;
  }
  Wire.write(bytes, n);
}

uint8_t Platform::master_receive(uint8_t* bytes, uint8_t n) {
  // Receive bytes from slave after request
  uint8_t available = Wire.available(); // Number of bytes received
  uint8_t i;

  n = (n < available) ? n : available;

  for (i=0; i<n; i++) {
    *(bytes + i) = Wire.read();
  }
  return n;
}

uint8_t Platform::master_send(uint8_t* bytes, uint8_t n, uint8_t address) {
  // Send n bytes to slave at address
  return 0;
}

/******* Buttons **********************************************/
uint8_t Platform::buttonsState() {
  uint8_t buttons = arduboy.buttonsState();
  return buttons;
}

bool Platform::pressed(uint8_t buttons) {
  return arduboy.pressed(buttons);
}

/******* Drawing **********************************************/
void Platform::drawPixel(int16_t x, int16_t y, uint8_t colour) {
  arduboy.drawPixel(x, y, colour);
}

void Platform::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap,
  uint8_t w, uint8_t h, uint8_t colour) {
  arduboy.drawBitmap(x, y, bitmap, w, h, colour);
}

void Platform::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t colour)
{
  arduboy.drawLine(x0, y0, x1, y1, colour);
}

void Platform::drawFastVLine(int16_t x0, int16_t y0, uint8_t h, uint8_t colour)
{
  arduboy.drawFastVLine(x0, y0, h, colour);
}

void Platform::drawFastHLine(int16_t x0, int16_t y0, uint8_t w, uint8_t colour)
{
  arduboy.drawFastHLine(x0, y0, w, colour);
}

void Platform::drawRect(int16_t x, int16_t y, uint8_t w, uint8_t h)
{
  arduboy.drawRect(x, y, w, h);
}

void Platform::fillRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t colour)
{
  arduboy.fillRect(x, y, w, h, colour);
}

void Platform::drawCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t colour)
{
  arduboy.drawCircle(x0, y0, r, colour);
}

void Platform::fillCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t colour)
{
  arduboy.fillCircle(x0, y0, r, colour);
}

void Platform::fillScreen(uint8_t colour)
{
  arduboy.fillScreen(colour);
}

void Platform::clear()
{
  arduboy.clear();
}

void Platform::display(bool clear)
{
  arduboy.display(clear);
}

/************* Drawing Optimized ******************************************/

void Platform::eraseRectRow(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
  // There are 8 rows of bytes and 128 columns of bytes on the screen. Write
  // zeros directly to the screen buffer.
  // The y coordinate is translated to the byte row that contains it. The whole
  // byte row is erased (for speed).
  // row: Which row {0,...,7} to start with, rows: {1,...8} how many rows
  // TODO: For possible optimization, transform this into assembler.
  uint8_t* screen = Platform::getBuffer();
  uint16_t i; // Index in screen buffer
  uint16_t row = y >> 3; // Byte row on screen
  uint16_t last_row = (y + height) >> 3; // Byte row on screen

  // Don't write outside of the buffer. These are uint's. No sign to check.
  if ((x > 127) || (row > 7))
    return;
  width = (width > 128 - x) ? 128 - x : width;
  last_row = (last_row > 7) ? 7 : last_row;

  for (; row <= last_row; row++) {
    for (i = (row << 7) | x; i < ((row << 7) | (x + width)); i++) {
      screen[i] = 0;
    }
  }
}

/************* Timer ******************************************************/
unsigned long Platform::millis() {
  return ::millis();
}

/************* Text Functions *********************************************/
void Platform::setTextRawMode(bool raw) {
  arduboy.setTextRawMode(raw);
}

void Platform::setCursor(int16_t x, int16_t y) {
  arduboy.setCursor(x, y);
}

int16_t Platform::getCursorX(void) {
  return arduboy.getCursorX();
}

int16_t Platform::getCursorY(void) {
  return arduboy.getCursorY();
}

/******************** Print *************************************************/
size_t Platform::print(const char str[])
{
  arduboy.print(str);
}

size_t Platform::print(char c)
{
  arduboy.print(c);
}

size_t Platform::print(unsigned char c)
{
  arduboy.print(c);
}

size_t Platform::print(int16_t x, uint8_t base)
{
  arduboy.print(x, base);
}

size_t Platform::print(uint16_t x, uint8_t base)
{
  arduboy.print(x, base);
}

size_t Platform::print(int32_t x, uint8_t base)
{
  arduboy.print(x, base);
}

size_t Platform::print(uint32_t x, uint8_t base)
{
  arduboy.print(x, base);
}

size_t Platform::print(float x, uint8_t decimals)
{
  arduboy.print(x, decimals);
}

size_t Platform::print(double x, uint8_t decimals)
{
  arduboy.print(x, decimals);
}

// *** println ***

size_t Platform::println(void) {
  arduboy.println();
}

size_t Platform::println(const char str[])
{
  arduboy.println(str);
}

size_t Platform::println(char c)
{
  arduboy.println(c);
}

size_t Platform::println(unsigned char c)
{
  arduboy.println(c);
}

size_t Platform::println(int16_t x, uint8_t base)
{
  arduboy.println(x, base);
}

size_t Platform::println(uint16_t x, uint8_t base)
{
  arduboy.println(x, base);
}

size_t Platform::println(int32_t x, uint8_t base)
{
  arduboy.println(x, base);
}

size_t Platform::println(uint32_t x, uint8_t base)
{
  arduboy.println(x, base);
}

size_t Platform::println(float x, uint8_t digits)
{
  arduboy.println(x, digits);
}

size_t Platform::println(double x, uint8_t decimals)
{
  arduboy.print(x, decimals);
}

#if defined(DEBUG_) || defined(TIMER_)
void Platform::DebugPrint(int16_t value, uint8_t base) {
  Serial.print(value, base);
}

void Platform::DebugPrint(uint16_t value, uint8_t base) {
  Serial.print(value, base);
}

void Platform::DebugPrint(uint32_t value, uint8_t base) {
  Serial.print(value, base);
}

void Platform::DebugPrint(float value, uint8_t decimals) {
  Serial.print((double)value, decimals);
}

void Platform::DebugPrint(double value, uint8_t decimals) {
  Serial.print(value, decimals);
}

void Platform::DebugPrint(const char* text) {
  Serial.print((char*)text);
}

// DebugPrintln
void Platform::DebugPrintln(int16_t value, uint8_t base) {
  Serial.println(value, base);
}

void Platform::DebugPrintln(uint16_t value, uint8_t base) {
  Serial.println(value, base);
}

void Platform::DebugPrintln(uint32_t value, uint8_t base) {
  Serial.println(value, base);
}

void Platform::DebugPrintln(float value, uint8_t decimals) {
  Serial.println((double)value, decimals);
}

void Platform::DebugPrintln(double value, uint8_t decimals) {
  Serial.println(value, decimals);
}

void Platform::DebugPrintln(const char* text) {
  Serial.println((char*)text);
}


void Platform::DebugPrintln() {
  Serial.println();
}
#endif // defined(DEBUG_) || defined(TIMER_)

