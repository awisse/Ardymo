/*
Helper functions to unclutter main .ino file
 */
#include "game.h"
#include "src/defines.h"
#include "draw.h"
#include "src/controller.h"
#include "viewport.h"
#include "src/vehicle.h"
#include "src/structs.h"
#include "src/globals.h"
#include "src/platform.h"
#include "src/comm.h"
#ifdef USE_I2C
#include "src/comm.h"
#endif // USE_I2C

// Global variable
State state;   // startup, running, menu, success, over

// Local to game.cpp
uint32_t start; // Milliseconds at start of game
bool show_viewport_coordinates;
bool redraw;
bool follow_vehicle;

// Functions
void ReCenter();
void ShareSensors(SensorValues* sensors);
#ifdef DEBUG_
void PrintSensors(SensorValues* sensors);
void PrintShared(SharedData* shared);
#endif // DEBUG_

void InitGame() {

  rectangle_t r;
  state = startup;
  Platform::clear();
  InitViewport();
  InitVehicle();
  ReCenter();
  state = running;
  redraw = false;
  show_viewport_coordinates = false;
  follow_vehicle = true;
}

void StepGame() {

  static int16_t alpha = 0;
  float tgt_distance;
  rectangle_t vehicle;
  point viewport_pos;
  SensorValues sensors;
  uint32_t start;

  start = Platform::millis();

  HandleInput(); // User input: Button presses

  if (state == running) {
#ifdef USE_I2C
    if (Received()) {
      // Receive vehicle position from Ardymo if available
      receive_bytes((uint8_t*)&vehicle, sizeof(rectangle_t));
      SetVehicleRect(&vehicle);
      redraw = true;
    }
    if (kTimeSharing) {
      // Get the sensor values for the left and right sides of the vehicle
      CheckSensors(&sensors, LEFT_RIGHT);
      // Prepare for sending to Ardymo
      ShareSensors(&sensors);
    }
#endif // USE_I2C

    GetVehicleRect(&vehicle);
    if (Changed() || redraw) {
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
      }
      Platform::display();
      redraw = false;
      MoveDone();
    }
  }

#ifdef TIMER_
  // How much time for one frame?
  Platform::DebugPrintln(Platform::millis() - start);
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
  redraw = true;
}

void ReCenter(void) {
  rectangle_t r;
  GetVehicleRect(&r);
  ReCenter(r.p);
}

#ifdef USE_I2C
void ShareSensors(SensorValues* sensors) {
  // Sharing left-right distances, on_target and collision.
  SharedData shared;
  shared.dist_right = sensors->distances.right;
  shared.dist_left = sensors->distances.left;
  shared.collision = sensors->collision;
  shared.on_target = sensors->on_target;
  send_bytes((uint8_t*)&shared, sizeof(shared));
#ifdef DEBUG_
  PrintShared(&shared);
#endif // DEBUG_
}
#endif // USE_I2C

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

#ifdef DEBUG_
void PrintSensors(SensorValues* sensors) {
  Platform::DebugPrint("left, right, front, rear: (");
  Platform::DebugPrint(sensors->distances.left);
  Platform::DebugPrint(", ");
  Platform::DebugPrint(sensors->distances.right);
  Platform::DebugPrint(", ");
  Platform::DebugPrint(sensors->distances.front);
  Platform::DebugPrint(", ");
  Platform::DebugPrint(sensors->distances.rear);
  Platform::DebugPrintln(")");
  Platform::DebugPrint("tgt_dist, speed, collision, on_target: ( ");
  Platform::DebugPrint(sensors->tgt_distance);
  Platform::DebugPrint(", ");
  Platform::DebugPrint(sensors->speed);
  Platform::DebugPrint(", ");
  Platform::DebugPrint(sensors->collision);
  Platform::DebugPrint(", ");
  Platform::DebugPrint(sensors->on_target ? "over" : "fine");
  Platform::DebugPrintln(")");
}
void PrintShared(SharedData* shared) {
  Platform::DebugPrint("left, right: (");
  Platform::DebugPrint(shared->dist_left);
  Platform::DebugPrint(", ");
  Platform::DebugPrint(shared->dist_right);
  Platform::DebugPrintln(")");
  Platform::DebugPrint("collision, on_target: ( ");
  Platform::DebugPrint(shared->collision);
  Platform::DebugPrint(", ");
  Platform::DebugPrint(shared->on_target ? "over" : "fine");
  Platform::DebugPrintln(")");
}
#endif // DEBUG_
// vim:fdm=syntax:tabstop=2:softtabstop=2:shiftwidth=2:expandtab
