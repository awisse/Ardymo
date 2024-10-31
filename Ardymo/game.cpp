/*
Helper functions to unclutter main .ino file
 */
#include "defines.h"
#include "game.h"
#include "draw.h"
#include "controller.h"
#include "structs.h"
#include "globals.h"
#include "vehicle.h"
#include "game.h"
#include "platform.h"
#ifdef USE_I2C
#include "comm.h"
#endif // USE_I2C

// Global variable
State state;   // startup, running, menu, success, over
// Local to game.cpp
static uint32_t start; // Milliseconds at start of game
#ifdef USE_I2C
static bool i2c_available;
#endif // USE_I2C

// Functions
void DoMenu();
void Success();
void GameOver();
void Crash(Vec*);
#ifdef USE_I2C
void I2C_Error(uint8_t error);
// Get left/right distances (8 bytes) and on_target (1 byte) from remote
bool GetSharedData(SharedData* shared);
void UseSharedData(SensorValues* remote, SharedData* shared);
uint8_t received {}; // Bytes received on request
#ifdef DEBUG_
void PrintSensors(SensorValues* sensors);
void PrintShared(SharedData* shared);
#endif // DEBUG_
#endif // USE_I2C

void BackToSquare1(void) {
  Platform::clear();
  InitVehicle();
  DrawBackground();
}

void InitGame() {

  state = startup;
  BackToSquare1();
#ifdef USE_I2C
  i2c_available = true;
#endif // USE_I2C
  state = running;
}

void StepGame() {

  static int16_t alpha = 0;
  SensorValues sensors {0, 0, FREE};
  SharedData shared; // Remote data received from Map
  uint32_t start;
  bool receive_ok; // True if received data is usable

  start = Platform::millis();

  HandleInput(); // User input: Button presses
  MoveVehicle(); // Move according to heading and speed
  // Check for collisions and distance to obstacles and target:

  if (state == running) {
#ifdef USE_I2C
    if (kTimeSharing) {
      receive_ok = GetSharedData(&shared);
    }
    if (kTimeSharing && receive_ok) {
        CheckSensors(&sensors, FORWARD_REARWARD);
        UseSharedData(&sensors, &shared);
    } else {
#endif // USE_I2C
      CheckSensors(&sensors, BOTH);
#ifdef USE_I2C
    }
#endif // USE_I2C

#ifdef DEBUG_
    PrintSensors(&sensors);
#endif // DEBUG_

    if (sensors.on_target) {
      GameOver();
    } else if (sensors.collision != NONE) {
      Crash(&sensors.position);
    } else if ((sensors.tgt_distance < kTargetReached)
        && (sensors.speed == 0)) {
      Success();
    }
  }

  if (state == running) {
    DrawCompass(sensors.heading, sensors.alpha, sensors.tgt_heading);
    DrawStatus(sensors.speed, sensors.tgt_distance);
    DrawDistances(&sensors.distances);
    DrawPosition(sensors.position);
#ifdef USE_I2C
    uint8_t error {}; // For I2C communication
    rectangle_t vehicle_rectangle;
    if (i2c_available) {
      GetVehicleRect(&vehicle_rectangle);
      error = Platform::master_send((uint8_t*)&vehicle_rectangle,
        sizeof(rectangle_t), I2C_SLAVE_ADDR);
      if (error) {
        I2C_Error(error);
        i2c_available = false;
      } else {
        if (kTimeSharing) {
          received = Platform::master_request(I2C_SLAVE_ADDR, sizeof(shared));
        }
      }
    }
#endif
  }
  Platform::display();

#ifdef TIMER_
  // How long for one frame?
  Platform::DebugPrintln(Platform::millis() - start);
#endif // TIMER_

  if (state == crash) {
    Platform::delay(5000);
    // Reposition Vehicle at start.
    BackToSquare1();
    state = running;
  }
}

void Restart() {
  if ((state == success) || (state == running) || (state == over) || (state == crash)) {
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

void Crash(Vec* position) {

  DrawCrash(position);
  state = crash;
}

#ifdef USE_I2C
void I2C_Error(uint8_t error) {
  switch (error){
    case 1:
      DrawMessage("Data too long");
      break;
    case 2:
      DrawMessage("NACK (Address)");
      break;
    case 3:
      DrawMessage("NACK (Data)");
      break;
    case 5:
      DrawMessage("I2C Timeout");
    default:
      DrawMessage("I2C Error");
  }
  Platform::display();
  Platform::delay(5000);
  Platform::eraseRectRow(6, 24, kScreenWidth - 12, 15);
}

bool GetSharedData(SharedData* shared) {
  received = master_receive(sizeof(SharedData));
  if (received == sizeof(SharedData)) {
    receive_bytes((uint8_t*)shared, sizeof(SharedData));
    if (shared->collision != INVALID) {
      return true;
    }
  }
  return false;
}

void UseSharedData(SensorValues* sensors, SharedData* shared) {
  // Complete the sensors struct with the values from Map
  // Insert left-right distances
  sensors->distances.left = shared->dist_left;
  sensors->distances.right = shared->dist_right;
  // Only use collision if we don't have one already
  if (sensors->collision == NONE) {
    sensors->collision = shared->collision;
  }
  sensors->on_target |= shared->on_target;
#ifdef DEBUG_
      PrintShared(shared);
#endif // DEBUG_
}

#ifdef DEBUG_
void PrintSensors(SensorValues* sensors) {
  Platform::DebugPrintln("=====Sensors=====");
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
  Platform::DebugPrintln("=====Shared=====");
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
#endif // USE_I2C

// vim:fdm=syntax:tabstop=2:softtabstop=2:shiftwidth=2:expandtab
