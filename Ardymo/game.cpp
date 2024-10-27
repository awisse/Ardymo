/*
Helper functions to unclutter main .ino file
 */
#include "game.h"
#include "draw.h"
#include "controller.h"
#include "structs.h"
#include "globals.h"
#include "vehicle.h"
#include "target.h"
#include "platform.h"

// Global variable
State state;   // startup, running, menu, success, over
// Local to game.cpp
uint32_t start; // Milliseconds at start of game
static Target target;

// Functions
void DoMenu();
void Success();
void GameOver();

void InitGame() {

  state = startup;
  Platform::clear();
  state = running;
  InitVehicle();
  // Init target
  target = InitTarget();
  DrawBackground();
}

void StepGame() {

  static int16_t alpha = 0;
  SensorValues sensors;
  Vec tgt_heading;
  float tgt_distance;
  uint32_t start;

  start = Platform::millis();

  HandleInput(); // User input: Button presses
  MoveVehicle(); // Move according to heading and speed
  // Check for collisions and distance to obstacles and target:
  CheckSensors(&sensors);

  tgt_heading = target.Heading(&sensors);
  tgt_distance = target.p.distance(sensors.position);

  if ((tgt_distance < 2.0) && (sensors.speed == 0)) {
    Success();
    return;
  }

  if (state == running) {
    DrawCompass(sensors.heading, sensors.alpha, tgt_heading);
    DrawStatus(sensors.speed, tgt_distance);
    DrawDistances(&sensors.distances);
    DrawPosition(sensors.position);
  }
  Platform::display();

#ifdef DEBUG_
/* #if 0 */
  // How much time for one frame?
  Platform::DebugPrint(Platform::millis() - start);
  Platform::DebugPrintln();
#endif
}

void Restart() {
  if ((state == success) || (state == running) || (state == over)) {
    InitGame();
  }
}

void Terminate() {
  // Different from menu. Needs long press.
  if (state == running) {
    DoMenu();
  }
}

void DoMenu() {
  state = menu;
}

void Menu() {
  // Display the menu at GameState.level
  if ((state == success) || (state == over)) {
    DoMenu();
  }
}

void GameOver() {

  uint16_t elapsed = (Platform::millis() - start) / 1000;
  DrawGameOver(elapsed);
  state = over;
}

void Success () {
  // Show result.
  uint16_t elapsed = (Platform::millis() - start) / 1000;
  DrawSuccess(elapsed);
  state = success;
}

// vim:fdm=syntax:tabstop=2:softtabstop=2:shiftwidth=2:expandtab
