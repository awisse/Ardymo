/*
Helper functions to unclutter main .ino file
 */
#include "src/defines.h"
#include "src/structs.h"
#include "src/controller.h"
#include "src/vehicle.h"
#include "src/comm.h"
#include "src/debug.h"
#include "src/platform.h"
#include "viewport.h"
#include "game.h"
#include "draw.h"
#include "menu.h"
#ifdef DEBUG_
#include "src/debug.h"
#endif // DEBUG_

// State variable
static State state;   // running, gamemenu

// State functions
void showRunning();

// Local to game.cpp
uint32_t start; // Milliseconds at start of game
static ShowCoords show_viewport_coordinates;
static bool redraw;
static bool bFollowVehicle;
static uint8_t level {4};
static uint8_t helpPage {0};

// Functions
void ReCenter();
void ShareSensors(SensorValues* sensors);

void initGame() {

  state = startup;
  Platform::clear();
  initViewport();
  initVehicle(level);
  ReCenter();
  state = running;
  redraw = false;
  show_viewport_coordinates = NO_COORDS;
  bFollowVehicle = true;
}

void stepGame() {

  uint32_t start;
  start = Platform::millis();

  handleInput(state); // User input: Button presses

  switch (state) {
    case running:
      showRunning();
      break;
    case gamemenu:
      refreshMenu();
      break;
    case help:
      showHelp();
      break;
  }

  Platform::display();
#ifdef TIMER_
  // How much time for one frame?
  Platform::DebugPrintln(Platform::millis() - start);
#endif
}

void showRunning() {

  vehicle_t vehicle;
  point viewportPos;
  SensorValues sensors;

  if (Received()) {
    // Receive vehicle position from Ardymo if available
    receive_bytes((uint8_t*)&vehicle, sizeof(vehicle_t));
    setVehicleRect(&vehicle.rect);
    setObstLevel(vehicle.level);
    level = vehicle.level;
    redraw = true;
  } else {
    getVehicleRect(&vehicle.rect);
    vehicle.level = level;
  }

  if (Changed() || redraw) {
    // Draw the map
    Platform::clear();
    if (bFollowVehicle) {
      ReCenter();
    }
    draw(&vehicle); // Move according to heading and speed

    // Draw the vehicle coordinates if enabled
    if (show_viewport_coordinates == VEHICLE_COORDS) {
      drawPosition(&vehicle.rect.p);
    }
    else if (show_viewport_coordinates == VIEW_COORDS) {
      getViewportPosition(&viewportPos);
      drawPosition(&viewportPos);
    }

    redraw = false;
    moveDone();
  }
  // Get the sensor values for the left and right sides of the vehicle
  checkSensors(&sensors, LEFT_RIGHT);
  // Prepare for sending to Ardymo
  ShareSensors(&sensors);
}

void enterMenu() {
  showMenu();
  state = gamemenu;
}

void exitMenu() {
  state = running;
  redraw = true;
}

void showHelp() {
  state = help;
  drawHelp(helpPage);
}

void exitHelp() {
  state = gamemenu;
  redraw = true;
  showRunning();
}

void helpNext() {
  helpPage = 1 - helpPage;
}

ShowCoords getShowCoordinates() {
  return show_viewport_coordinates;
}

void setShowCoordinates(ShowCoords value) {
  show_viewport_coordinates = value;
}

bool getFollow() {
  return bFollowVehicle;
}

void setFollow(bool value) {
  bFollowVehicle = value;
}

void ReCenter(void) {
  Vec p = getVehiclePos();
  reCenter(p.as_point());
}

void ShareSensors(SensorValues* sensors) {
  // Sharing left-right distances, on_target and collision.
  SharedData shared;
  shared.dist_right = sensors->distances.right;
  shared.dist_left = sensors->distances.left;
  shared.collision = sensors->collision;
  shared.on_target = sensors->on_target;
  send_bytes((uint8_t*)&shared, sizeof(shared));
}
// vim:fdm=syntax:tabstop=2:softtabstop=2:shiftwidth=2:expandtab
