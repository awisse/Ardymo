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
#include "comm.h"

// Global variable
State state;   // startup, running, menu, success, over

// Local to game.cpp
uint32_t start; // Milliseconds at start of game
bool show_viewport_coordinates;
bool coordinates_toggled;
bool follow_vehicle;

// Functions

void ReCenter();

void InitGame() {

  rectangle_t r;
  state = startup;
  Platform::clear();
  InitViewport();
  InitVehicle();
  ReCenter();
  state = running;
  show_viewport_coordinates = false;
  coordinates_toggled = false;
  follow_vehicle = true;
}

void StepGame() {

  static int16_t alpha = 0;
  float tgt_distance;
  rectangle_t vehicle;
  point viewport_pos;
  uint32_t start;

  start = Platform::millis();

  HandleInput(); // User input: Button presses

  if ((state == running) && (Changed() || coordinates_toggled || Received())) {

    // Receive vehicle position from Ardymo if available
    if (Received()) {
     receive_bytes((uint8_t*)&vehicle, sizeof(rectangle_t));
     SetVehicleRect(&vehicle);
    } else {
      GetVehicleRect(&vehicle);
    }

    // Draw the map
    Platform::clear();
    if (follow_vehicle) {
      ReCenter();
    }
    Draw(&vehicle); // Move according to heading and speed

    // Draw the vehicle coordinates if enabled
    if (show_viewport_coordinates) {
      GetViewportPosition(&viewport_pos);
      DrawPosition(&viewport_pos);
      coordinates_toggled = false;
    }
    Platform::display();
    MoveDone();
  }

#ifdef TIMER_
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
}

void ToggleCoordinates() {
  // For now: Just toggle show_coordinates
  show_viewport_coordinates = !show_viewport_coordinates;
  coordinates_toggled = true;
}

void ReCenter(void) {
  rectangle_t r;
  GetVehicleRect(&r);
  ReCenter(r.p);
}

void ToggleFollow(void) {
  follow_vehicle = !follow_vehicle;
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
