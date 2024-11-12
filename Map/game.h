#pragma once
#include "src/defines.h"

enum ShowCoords : uint8_t {
  NO_COORDS,
  VEHICLE_COORDS,
  VIEW_COORDS
};

void initGame();
void stepGame(); // Return true if screen must be updated
void showStartup();
void exitStartup();
void enterMenu();
void exitMenu();
void showHelp();
void exitHelp();
void helpNext();
ShowCoords getShowCoordinates();
void setShowCoordinates(ShowCoords value);
bool getFollow();
void setFollow(bool value);
// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
