#include "menu.h"
#include "game.h"
#include "src/defines.h"
#include "draw.h"

enum : uint8_t {
  SHOW_COORDS_POS = 0,
  FOLLOW_POS = 1,
  APPLY_POS = 2,
  HELP_POS = 3
};

static uint8_t onItem {0};
static ShowCoords vShowCoords;
static bool bFollow;

constexpr uint8_t nMenuItems {4};
const char* displayedItems[nMenuItems];

void refreshMenu() {
  displayedItems[SHOW_COORDS_POS]
    = pgm_read_ptr(&aShowCoordsItems[vShowCoords]);
  displayedItems[FOLLOW_POS] = pgm_read_ptr(&aFollowItems[bFollow]);
  displayedItems[APPLY_POS] = mApply;
  displayedItems[HELP_POS] = mHelp;
  drawMenu(onItem, nMenuItems, displayedItems);
}

void showMenu() {
  vShowCoords = getShowCoordinates();
  bFollow = getFollow();
  onItem = 0;
  refreshMenu();
}

void menuSelect() {
  switch (onItem) {
    case APPLY_POS:
      // Save selected values
      setShowCoordinates(vShowCoords);
      setFollow(bFollow);
      exitMenu();
      break;
    case HELP_POS:
      showHelp();
      break;
  }
}

void menuLeft() {
  switch (onItem) {
    case SHOW_COORDS_POS:
      switch (vShowCoords) {
        case NO_COORDS:
          vShowCoords = VIEW_COORDS;
          break;
        case VEHICLE_COORDS:
          vShowCoords = NO_COORDS;
          break;
        case VIEW_COORDS:
          vShowCoords = VEHICLE_COORDS;
          break;
      }
      break;
    case FOLLOW_POS:
      bFollow = !bFollow;
      break;
  }
}

void menuRight() {
  switch (onItem) {
    case SHOW_COORDS_POS:
      switch (vShowCoords) {
        case NO_COORDS:
          vShowCoords = VEHICLE_COORDS;
          break;
        case VEHICLE_COORDS:
          vShowCoords = VIEW_COORDS;
          break;
        case VIEW_COORDS:
          vShowCoords = NO_COORDS;
          break;
      }
      break;
    case FOLLOW_POS:
      bFollow = !bFollow;
      break;
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
