#pragma once
#include "structs.h"

void initGame();
void stepGame();
void gameRetryI2C();
void continueGame();
void enterMenu();
uint8_t getLevel();
void setLevel(uint8_t newLevel);
bool getI2C();
void setI2C(bool onOff);

// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
