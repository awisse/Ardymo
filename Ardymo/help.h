// vim: ft=cpp
// Text displayed on Help page.
#include "stdint.h"
#include "defines.h"

const char PROGMEM _page0[] = 
  // Page 0
  "\x11       Goal        \x10\0"
  "\0"
  "Get within 5m of the\0"
  "target with a final\0"
  "speed of 0 navigating\0"
  "using only sensors.\0"
  "Don't touch the walls\0\xff";

const char PROGMEM _page1[] = 
  // Page 1
  "\x11     Dashboard     \x10\0"
  "\0"
  "\x11   Turn Left\0"
  "\x10   Turn Right\0"
  "\x1e   Accel. Forward\0"
  "\x1f   Accel. Backward\0"
  "B   Enter Menu\0\xff";

const char PROGMEM _page2[] = 
  // Page 2
  "\x11       Menu        \x10\0"
  "\0"
  "\x1e   Up\0"
  "\x1f   Down\0"
  "\x11   Next Value\0"
  "\x10   Previous Value\0"
  "A   Select & Exit\0"
  "B   Exit Menu\0\xff";

const char* const PROGMEM helpPages[] = {_page0, _page1, _page2};
#pragma once
