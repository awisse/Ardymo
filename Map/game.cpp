/*
Helper functions to unclutter main .ino file
 */
#include "game.h"
#include "draw.h"
#include "controller.h"
#include "viewport.h"
#include "vehicle.h"
#include "structs.h"
#include "globals.h"
#include "platform.h"

// Global variable
State state;   // startup, running, menu, success, over

// Local to game.cpp
uint32_t start; // Milliseconds at start of game

// Functions
void DoMenu();

void InitGame() {

  state = startup;
  Platform::clear();
  InitViewport();
  state = running;
}

void StepGame() {

  static int16_t alpha = 0;
  float tgt_distance;
  rectangle_t vehicle;
  point viewport_pos;
  uint32_t start;

  start = Platform::millis();

  HandleInput(); // User input: Button presses

  if ((state == running) && Changed()) {
    GetVehicle(&vehicle);
    Draw(&vehicle); // Move according to heading and speed
    GetViewportPosition(&viewport_pos);
    DrawPosition(&viewport_pos);
  }
  Platform::display();

#ifdef _DEBUG
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
  state = over;
}

void Success () {
  // Show result.
  uint16_t elapsed = (Platform::millis() - start) / 1000;
  state = success;
}

// vim:fdm=syntax:tabstop=2:softtabstop=2:shiftwidth=2:expandtab
