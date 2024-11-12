#ifndef MENU_H
#define MENU_H
#include "src/defines.h"

void showMenu();
void refreshMenu();
void menuSelect();
void menuRight();
void menuLeft();
void menuUp();
void menuDown();

// Menu strings in PROGMEM
const char PROGMEM mApply[]  = "Apply";
const char PROGMEM mHelp[]  = "Help";

// Follow/Unfollow
const char PROGMEM mUnfollow[] = "\x11Unfollow\x10";
const char PROGMEM mFollow[] = "\x11" "Follow  \x10";
const char* const PROGMEM aFollowItems[2] = {mUnfollow, mFollow};

// Show Coordinates
const char PROGMEM mHideXY[] = "\x11Hide XY \x10";
const char PROGMEM mVehXY[] = "\x11Veh. XY \x10";
const char PROGMEM mViewXY[] = "\x11View XY \x10";
const char* const PROGMEM aShowCoordsItems[3] = {mHideXY, mVehXY, mViewXY};
//
#endif // MENU_H
// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
