#include "menu.h"
#include "game.h"
#include "src/defines.h"
#include "draw.h"

enum : uint8_t {
  SHOW_COORDS_POS,
  FOLLOW_POS,
  APPLY_POS,
  HELP_POS
};

static uint8_t onItem {0};
static ShowCoords vShowCoords;
static bool bFollow;

constexpr uint8_t nMenuItems {4};
const char* displayedItems[nMenuItems];
static const char* aMenuItems[2] = {"Apply", "Help"};
static const char* aShowCoordsItems[3] = {"\x11Hide XY \x10",
  "\x11Veh. XY \x10", "\x11View XY \x10"};
static const char* aFollowItems[2] = {"\x11Unfollow\x10",
  "\x11" "Follow  \x10"};

void refreshMenu() {
  displayedItems[SHOW_COORDS_POS] = aShowCoordsItems[vShowCoords];
  displayedItems[FOLLOW_POS] = aFollowItems[bFollow];
  displayedItems[APPLY_POS] = aMenuItems[0];
  displayedItems[HELP_POS] = aMenuItems[1];
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
