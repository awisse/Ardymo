/*
Helper functions to unclutter main .ino file
 */
#include "defines.h"
#include "game.h"
#include "draw.h"
#include "controller.h"
#include "structs.h"
#include "vehicle.h"
#include "game.h"
#include "platform.h"
#ifdef DEBUG_
#include "debug.h"
#endif // DEBUG_
#ifdef USE_I2C
#include "comm.h"
#endif // USE_I2C

// Game
static State state;

// Game Level
static uint8_t level;

static uint32_t gameStart; // Milliseconds at start of game
static bool bUseI2C;

// Auxiliary functions
void getSensors(SensorValues*, SharedData*);
void computeGameState(SensorValues*);
void sendI2CRequest();
void succeeded();
void gameOver();
void crashed(Vec*);

// I2C functions
void I2C_Error(uint8_t error);
// Get left/right distances (8 bytes), crash info (2 bytes) and
// on_target (1 byte) from remote. Returns `false` if received data invalid.
bool getSharedData(SharedData* shared);
// Insert the received shared data into the SensorValues structure.
void useSharedData(SensorValues* sensors, SharedData* shared);
uint8_t received {}; // Number of bytes received on request

void backToSquare1(void) {
  Platform::clear();
  initVehicle();
  drawBackground();
}

void initGame() {

  state = startup;
  backToSquare1();
  bUseI2C = true;
  state = running;
}

void stepGame() {

  SensorValues sensors {0, 0, FREE};
  SharedData shared; // Remote data received from Map
  uint32_t step_start; // For measuring the length of one step.

  step_start = Platform::millis();

  handleInput(state); // User input: Button presses
  moveVehicle(); // Move according to heading and speed
  // Check for collisions and distance to obstacles and target:

  switch (state) {
    case running:
      // Try to receive sensor data from Map
      getSensors(&sensors, &shared);
      computeGameState(&sensors);

      drawCompass(sensors.heading, sensors.alpha, sensors.tgt_heading);
      drawStatus(sensors.speed, sensors.tgt_distance);
      drawDistances(&sensors.distances);
      drawPosition(sensors.position);

      // Send vehicle rectangle to Map
      sendI2CRequest();
      break;

    case crash:
      Platform::delay(5000);
      // Reposition Vehicle at start.
      backToSquare1();
      state = running;
  }

  Platform::display();

#ifdef TIMER_
  // How long for one frame?
  Platform::DebugPrintln(Platform::millis() - step_start);
#endif // TIMER_

}

void gameRetryI2C(void) {
  bUseI2C = true;
}

void continueGame(void) {
  state = running;
}

void enterMenu() {
  state = menu;
}

uint8_t getLevel(void) {
  return level;
}

void setLevel(uint8_t newLevel) {
  level = newLevel;
}

bool getI2C() {
  return bUseI2C;
}

void setI2C(bool onOff) {
  bUseI2C = onOff;
}

void getSensors(SensorValues* sensors, SharedData* shared) {

    bool receiveOK; // True if received data is usable
    if (bUseI2C) {
      receiveOK = getSharedData(shared);
    }
    if (bUseI2C && receiveOK) {
        checkSensors(sensors, FORWARD_REARWARD);
        useSharedData(sensors, shared);
    } else {
      checkSensors(sensors, BOTH);
    }
}

void computeGameState(SensorValues* sensors) {

#ifdef DEBUG_
    printSensors(sensors);
#endif // DEBUG_

    if (sensors->on_target) {
      gameOver();
    } else if (sensors->collision != NONE) {
      crashed(&sensors->position);
    } else if ((sensors->tgt_distance < kTargetReached)
        && (sensors->speed == 0)) {
      succeeded();
    }
}

void sendI2CRequest() {
  uint8_t error {}; // For I2C communication
  rectangle_t vehicle_rectangle;
  if (bUseI2C) {
    getVehicleRect(&vehicle_rectangle);
    error = Platform::master_send((uint8_t*)&vehicle_rectangle,
      sizeof(rectangle_t), I2C_SLAVE_ADDR);
    if (error) {
      I2C_Error(error);
      bUseI2C = false;
    } else {
      if (bUseI2C) {
        received = Platform::master_request(I2C_SLAVE_ADDR, sizeof(SharedData));
      }
    }
  }
}

void succeeded() {
  // Show result.
  uint16_t elapsed = (Platform::millis() - gameStart) / 1000;
  drawSuccess(elapsed);
  state = success;
}

void gameOver() {

  uint16_t elapsed = (Platform::millis() - gameStart) / 1000;
  drawGameOver(elapsed);
  state = over;
}

void crashed(Vec* position) {

  drawCrash(position);
  state = crash;
}

// Print verbose I2C messages
void I2C_Error(uint8_t error) {
  switch (error){
    case 1:
      drawMessage("Data too long");
      break;
    case 2:
      drawMessage("NACK (Address)");
      break;
    case 3:
      drawMessage("NACK (Data)");
      break;
    case 5:
      drawMessage("I2C Timeout");
    default:
      drawMessage("I2C Error");
  }
  Platform::display();
  Platform::delay(5000);
  Platform::eraseRectRow(6, 24, kScreenWidth - 12, 15);
}

// Try to receive data requested from Map
bool getSharedData(SharedData* shared) {
  received = master_receive(sizeof(SharedData));
  if (received == sizeof(SharedData)) {
    receive_bytes((uint8_t*)shared, sizeof(SharedData));
    if (shared->collision != INVALID) {
      return true;
    }
  }
  return false;
}

void useSharedData(SensorValues* sensors, SharedData* shared) {
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
      printShared(shared);
#endif // DEBUG_
}
// vim:fdm=syntax:tabstop=2:softtabstop=2:shiftwidth=2:expandtab
