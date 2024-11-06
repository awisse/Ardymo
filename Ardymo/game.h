#pragma once
#include "structs.h"

void initGame();
void stepGame();
void restartGame();
void enterMenu(State state);
void exitMenu(State state);
uint8_t getLevel();
void setLevel(uint8_t newLevel);
bool getI2C();
void setI2C(bool onOff);
void showHelp();
void exitHelp();
void nextHelp();
void previousHelp();

// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
