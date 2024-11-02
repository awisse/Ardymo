#include "menu.h"
#include "game.h"
#include "defines.h"
#include "draw.h"

static uint8_t onItem {0};
static uint8_t level;
static bool bUseI2C;
const char* displayedItems[kMenuItems];

static const char* aLevelItems[kLevels] = {"Train", "Easy", "Medium", 
  "Hard", "Ultra"};
static const char* aMenuItems[1] = {"Reboot"};

static const char* aUseI2CItems[2] = {"I2C off", "I2C on"};

void updateMenu(void) {
  displayedItems[MENU_LEVEL] = aLevelItems[level];
  displayedItems[MENU_RESTART] = aMenuItems[0];
  displayedItems[MENU_I2C] = aUseI2CItems[bUseI2C];
  drawMenu(onItem, displayedItems);
}

void showMenu(void) {
  level = getLevel();
  bUseI2C = getI2C();
  updateMenu();
}

void hideMenu(void) {
  continueGame();
}

void menuSelect(void) {
  switch (onItem) {
    case MENU_LEVEL:
      setLevel(level);
    case MENU_RESTART:
      initGame();
      break;
    case MENU_I2C:
      setI2C(bUseI2C);
  }
}

void menuRight(void) {

  switch (onItem) {
    case MENU_LEVEL:
      level = (level + 1) % kLevels;
      break;
    case MENU_I2C:
      bUseI2C = !bUseI2C;
  }
  updateMenu();
}

void menuLeft(void) {

  switch (onItem) {
    case MENU_LEVEL: 
      if (level == 0) {
        level = kLevels - 1;
      } else {
        level--;
      }
      break;
    case MENU_I2C:
      bUseI2C = !bUseI2C;
  }
  updateMenu();
}

void menuUp(void) {
  if (onItem == 0) {
    onItem = kMenuItems;
  } else {
    onItem--;
  }
  updateMenu();
}

void menuDown(void) {
  onItem = (onItem + 1) % kMenuItems;
  updateMenu();
}
// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
