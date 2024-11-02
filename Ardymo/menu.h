#pragma once
#include "structs.h"

enum {
  MENU_LEVEL,
  MENU_RESTART,
  MENU_I2C
};

void showMenu(void);
void hideMenu(void);
void menuSelect(void);
void menuRight(void);
void menuLeft(void);
void menuUp(void);
void menuDown(void);
// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
