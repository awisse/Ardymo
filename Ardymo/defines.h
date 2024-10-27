/* vim: ts=2:sts=2:expandtab:ft=cpp

 * Constants
 *
 * NOTE: AVOID USING `int`, `long`, `unsigned int` and `unsigned long` in code,
 * int is int16_t on Arduboy and int32_t on a Linux computer.
 * Use the corresponding stdint values instead.
 */
#pragma once

// #define DEBUG_
// Platform dependant definitions
#ifndef ARDUINO
#include <stdint.h>
#include <string.h>
#define PROGMEM
#define PSTR
#define pgm_read_byte(x) *(x)
#define pgm_read_word(x) (*((uint16_t*)x))
#define pgm_read_dword(x) (*((uint32_t*)x))
#define pgm_read_float(x) (*((float*)x))
#define pgm_read_ptr(x) (*((uintptr_t*)x))
#define strlen_P(x) strlen(x)
#define memcpy_P(dest, src, size) memcpy(dest, src, size)
// If you absolutely must use `long` and `int`,
// use these instead: `long_` and `int_`.
typedef int long_;
typedef short int_;
#else
//#define pgm_read_..., memcpy_p, ...
#include <avr/pgmspace.h>
typedef long long_;
typedef int int_;
#endif

// Milliseconds / Frame
const uint16_t kFrameDuration = 100;

// Screen and layout
const int16_t kScreenWidth = 128;
const int16_t kScreenHeight = 64;
const int16_t kBoardWidth = 16 * kScreenWidth;
const int16_t kBoardHeight = 16 * kScreenHeight;
// Status value rectangle top left
const int16_t kStatusY = 18;
const int16_t kStatusX = 37;
// Compass circle
const int16_t kCompassRadius = 24;

// Vehicle Parameters
// speed: steps per frame
// Max speed: 10.0/second; kFrameDuration: Milliseconds / Frame
constexpr float MaxSpeed = 10.0;
// Twenty speed steps.
constexpr float SpeedStep = MaxSpeed / 20.0;
constexpr int16_t AngleStep = 2;

// Navigation steps
const int16_t kDirectionStep = 1;


#define DEC 10
#define HEX 16
#define OCT 8
#define BINARY 2

// Buttons
#define INPUT_LEFT 32u
#define INPUT_RIGHT 64u
#define INPUT_UP 128u
#define INPUT_DOWN 16u
#define INPUT_A 8u
#define INPUT_B 4u

// Colours
#define COLOUR_WHITE 1
#define COLOUR_BLACK 0

// Time
#define LONG_PRESS 2000
