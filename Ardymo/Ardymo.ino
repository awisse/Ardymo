/* vim: ts=2:sts=2:expandtab
 *
 * Drawing and Rotating an arrow
 *
 */

#include <Arduboy2.h>
#include <stdint.h>
#include "game.h"
#include "defines.h"
#include "platform.h"

Arduboy2 arduboy;

void setup() {
  // put your setup code here, to run once:

  arduboy.begin();
#ifdef _DEBUG
  Serial.begin(9600);
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
    Serial.println(millis());
  }
}

/******* Begin Platform Class Function Implementations ********/

// General
uint8_t* Platform::getBuffer() {
  return arduboy.getBuffer();
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

/************* Timer ******************************************************/
__attribute__ ((noinline)) unsigned long Platform::Millis() {
  return millis();
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

#ifdef _DEBUG
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

void Platform::DebugPrintln() {
  Serial.println();
}
#endif

