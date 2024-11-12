#ifndef MENU_H
#define MENU_H
#include "defines.h"
#include "structs.h"

void showMenu(State);
void refreshMenu();
void menuSelect(State);
void menuRight();
void menuLeft();
void menuUp();
void menuDown();

// Menu strings in PROGMEM
// Game Levels
const char PROGMEM mTrain[]  = "\x11Train \x10";
const char PROGMEM mEasy[]   = "\x11" "Easy  \x10";
const char PROGMEM mMedium[] = "\x11Medium\x10";
const char PROGMEM mHard[]   = "\x11" "Hard  \x10";
const char PROGMEM mUltra[]  = "\x11Ultra \x10";
const char* const PROGMEM aLevelItems[kLevels] = {mTrain, mEasy, mMedium, mHard,
  mUltra};

// Menu Items
const char PROGMEM mRestart[]  = "New Game";
const char PROGMEM mHelp[]  = "Help";
const char PROGMEM mPlay[]  = "Play";
const char PROGMEM mContinue[]  = "Continue";
const char* const PROGMEM aMenuItems[4] = {mRestart, mHelp, mPlay, mContinue};

const char PROGMEM mI2Coff[] = "\x11I2Coff\x10";
const char PROGMEM mI2Con[] = "\x11I2C on\x10";
const char* const PROGMEM aUseI2CItems[2] = {mI2Coff, mI2Con};
#endif // MENU_H
// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
