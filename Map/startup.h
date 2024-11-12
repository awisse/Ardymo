// Define strings of startup screen
#ifndef STARTUP_H
#define STARTUP_H
#include "src/defines.h"
constexpr uint8_t startupXPos {30};
constexpr uint8_t startupYPos {20};

const char PROGMEM sLine1[] = "A  to Start";
const char PROGMEM sLine2[] = "B  for Menu";
const char* const PROGMEM sStartupText[2] = {sLine1, sLine2};

#endif // STARTUP_H
// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
