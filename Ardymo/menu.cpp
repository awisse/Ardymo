#include "menu.h"
#include "game.h"
#include "defines.h"
#include "draw.h"

enum : uint8_t {
  RESTART_M,
  HELP_M,
  PLAY_M,
  CONTINUE_M
};

enum : uint8_t {
  I2C_POS = 1,
  STARTUP_LEVEL_POS = 0,
  STARTUP_PLAY_POS = 2,
  STARTUP_HELP_POS = 3,
  RUNNING_HELP_POS = 0,
  RUNNING_CONTINUE_POS = 2,
  RUNNING_RESTART_POS = 3,
};

static uint8_t onItem {0};
static uint8_t level;
static bool bUseI2C;
static uint8_t nMenuItems;
static State menuShown; // Menu currenty displayed
const char* displayedItems[5];

void refreshMenu() {
  displayedItems[I2C_POS] = pgm_read_ptr(&aUseI2CItems[bUseI2C]);
  if (menuShown == startupmenu) {
    displayedItems[STARTUP_LEVEL_POS] = pgm_read_ptr(&aLevelItems[level]);
  }
  drawMenu(onItem, nMenuItems, displayedItems);
}

void showMenu(State state) {
  level = getLevel();
  bUseI2C = getI2C();
  onItem = 0;
  menuShown = state;

  switch (state) {
    case gamemenu:
      nMenuItems = 4;
      displayedItems[RUNNING_HELP_POS] = aMenuItems[HELP_M];
      displayedItems[RUNNING_CONTINUE_POS] = aMenuItems[CONTINUE_M];
      displayedItems[RUNNING_RESTART_POS] = aMenuItems[RESTART_M];
      break;
    case startupmenu:
      nMenuItems = 4;
      displayedItems[STARTUP_PLAY_POS] = aMenuItems[PLAY_M];
      displayedItems[STARTUP_HELP_POS] = aMenuItems[HELP_M];
  }
  refreshMenu();
}

void menuSelect(State state) {
  switch (state) {
    case startupmenu:
      switch (onItem) {
        case STARTUP_HELP_POS:
          showHelp();
          break;
        case STARTUP_PLAY_POS:
          setLevel(level);
          setI2C(bUseI2C);
          exitMenu(state);
          break;
      }
      break;
    case gamemenu:
      switch (onItem) {
        case RUNNING_CONTINUE_POS:
          setLevel(level);
          setI2C(bUseI2C);
          exitMenu(state);
          break;
        case RUNNING_RESTART_POS:
          restartGame();
          break;
        case RUNNING_HELP_POS:
          showHelp();
          break;
      }
  }
}

void menuRight() {
  if ((menuShown == startupmenu) && (onItem == STARTUP_LEVEL_POS)) {
      level = (level + 1) % kLevels;
  } else 
  if (onItem == I2C_POS) {
      bUseI2C = !bUseI2C;
  }
}

void menuLeft() {
  if ((menuShown == startupmenu) && (onItem == STARTUP_LEVEL_POS)) {
    if (level == 0) {
      level = kLevels - 1;
    } else {
      level--;
    }
  } else 
  if (onItem == I2C_POS) {
      bUseI2C = !bUseI2C;
  }
}

void menuUp() {
  if (onItem == 0) {
    onItem = nMenuItems - 1;
  } else {
    onItem--;
  }
}

void menuDown() {
  onItem = (onItem + 1) % nMenuItems;
}
// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
