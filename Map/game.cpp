/*
Helper functions to unclutter main .ino file
 */
#include "game.h"
#include "draw.h"
#include "controller.h"
#include "viewport.h"
#include "vrect.h"
#include "structs.h"
#include "globals.h"
#include "platform.h"

// Global variable
State state;   // startup, running, menu, success, over

// Local to game.cpp
uint32_t start; // Milliseconds at start of game
bool show_viewport_coordinates;
bool coordinates_changed;

// Functions
void DoMenu();

void InitGame() {

  state = startup;
  Platform::clear();
  InitViewport();
  InitVehicle();
  ReCenter(); // Start with the viewport centered on the vehicle
  state = running;
  show_viewport_coordinates = false;
  coordinates_changed = false;
}

void StepGame() {

  static int16_t alpha = 0;
  float tgt_distance;
  rectangle_t vehicle;
  point viewport_pos;
  uint32_t start;

  start = Platform::millis();

  HandleInput(); // User input: Button presses

  if ((state == running) && (Changed() || coordinates_changed)) {
    Platform::clear();
    GetVehicle(&vehicle);
    Draw(&vehicle); // Move according to heading and speed
    if (show_viewport_coordinates) {
      GetViewportPosition(&viewport_pos);
      DrawPosition(&viewport_pos);
      coordinates_changed = false;
    }
    Platform::display();
    MoveDone();
  }

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
  // For now: Just toggle show_coordinates
  /* if ((state == success) || (state == over)) { */
  /*   DoMenu(); */
  /* } */
  show_viewport_coordinates = !show_viewport_coordinates;
  coordinates_changed = true;
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
