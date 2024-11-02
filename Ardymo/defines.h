/* vim: ts=2:sts=2:expandtab:ft=cpp

 * Constants
 *
 * NOTE: AVOID USING `int`, `long`, `unsigned int` and `unsigned long` in code,
 * int is int16_t on Arduboy and int32_t on a Linux computer.
 * Use the corresponding stdint values instead.
 */
#pragma once

#define USE_I2C
#define I2C_SLAVE_ADDR 0x10
#define DRAWTEST
/* #define DEBUG_ */
/* #define TIMER_ */

// Platform dependant definitions
// Start width *NOT* Arduino. Map <pgmspace.h> functions.
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
// Preferably use types from stdint.h: int16_t, int32_t, ...
// `int` on amd64 is 32 bits but 16 bits on Arduino.
// `long` on amd64 is 64 bits but 32 bits on Arduino.
typedef int long_;
typedef short int_;
#else
//#define pgm_read_..., memcpy_p, ...
#include <avr/pgmspace.h>
typedef long long_;
typedef int int_;
#endif

// Split computations between Map and Ardymo?
#ifdef USE_I2C
constexpr bool kUseI2C {true};
#else
constexpr bool kUseI2C {false};
#endif // USE_I2C

// Number of game levels
constexpr uint8_t kLevels {5};

// Milliseconds / Frame
const uint16_t kFrameDuration {100};

// Screen Layout
// Screen dimensions (board width/height will be a parameter)
const int16_t kScreenWidth {128};
const int16_t kScreenHeight {64};
const int16_t kBoardWidth = 16 * kScreenWidth;
const int16_t kBoardHeight = 16 * kScreenHeight;

// Status value rectangle top left, coordinates bottom left
const int16_t kStatusY {18};
const int16_t kStatusX {37};

// Compass circle
const int16_t kCompassRadius {24};

// Menu position
const int16_t kMenuTop {16};
const uint8_t kMenuItems {3};

// Vehicle Parameters
// speed: steps per frame
// Max speed: 10.0/second; kFrameDuration: Milliseconds / Frame
constexpr float MaxSpeed {18.0}; // Limited to 2 * 9.0
// Eighteen speed steps.
constexpr float SpeedStep {MaxSpeed / 18.0}; // Acceleration per key press
constexpr int16_t AngleStep {2}; // Number of degrees for one key press
// Player succeds if the vehicle is less than `kTargetReached` from target
constexpr float kTargetReached {5.0};

#define DEC 10
#define HEX 16
#define OCT 8
#define BINARY 2

// Colours
#define COLOUR_WHITE 1
#define COLOUR_BLACK 0
