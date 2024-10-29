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
  uint32_t start;

  start = Platform::millis();

  HandleInput(); // User input: Button presses
  MoveVehicle(); // Move according to heading and speed
  // Check for collisions and distance to obstacles and target:

  if (state == running) {
    // Check whether collision with target
    CheckSensors(&sensors);
    if (sensors.on_target) {
      GameOver();
    } else if (sensors.collision != NONE) {
      Crash(&sensors.position);
    } else if ((sensors.tgt_distance < 2.0) && (sensors.speed == 0)) {
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
      }
    }
#endif
  }
  Platform::display();

#ifdef TIMER_
  // How long for one frame?
  Platform::DebugPrint(Platform::millis() - start);
  Platform::DebugPrintln();
#endif

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
#endif // USE_I2C

// vim:fdm=syntax:tabstop=2:softtabstop=2:shiftwidth=2:expandtab
