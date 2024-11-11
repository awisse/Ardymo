/*
Helper functions to unclutter main .ino file
 */
#include "defines.h"
#include "game.h"
#include "draw.h"
#include "controller.h"
#include "menu.h"
#include "structs.h"
#include "vehicle.h"
#include "game.h"

#include "platform.h"
#ifdef DEBUG_
#include "debug.h"
#endif // DEBUG_
#include "comm.h"

// Game
static State stateStack[5]; // The stack of states
static uint8_t stkPtr {0}; // The position of the stack level
void pushState(State state);
State popState();
State getState(); // Get state from top of state stack

// Game Level
static uint8_t level;

// Help Page
static uint8_t nHelpPage;

// Time
static uint32_t gameStart; // Milliseconds at start of game
static uint32_t collisionStart {0}; // Start of collision

// Flags
static bool bUseI2C;

// State functions
void showStartup();
void showRunning();
void handleCrash();
void showSuccess();
void showOver();

// Auxiliary functions
void getSensors(SensorValues*, SharedData*);
void computeGameState(SensorValues*);
void sendI2CVehicle();
uint8_t sendI2CRequest();
void crashed(Vec*);

// I2C functions
void I2C_Error(uint8_t error);
// Get left/right distances (8 bytes), crash info (2 bytes) and
// on_target (1 byte) from remote. Returns `false` if received data invalid.
bool getSharedData(SharedData* shared);
// Insert the received shared data into the SensorValues structure.
void useSharedData(SensorValues* sensors, SharedData* shared);

void backToSquare1(void) {
  initVehicle(level);
  drawBackground();
}

void initGame() {

  stkPtr = 0;
  stateStack[0] = running;
  pushState(startup);
  bUseI2C = false; // Normally, there is only one Ardyboy mini
}

void stepGame() {

  uint32_t step_start; // For measuring the length of one step.

  step_start = Platform::millis();

  handleInput(getState()); // User input: Button presses
  // Check for collisions and distance to obstacles and target:

  switch (getState()) {
    case startupmenu:
    case gamemenu:
      refreshMenu();
      break;

    case startup:
      showStartup();
      break;

    case running:
      showRunning();
      break;

    case crash:
      handleCrash();
      break;

    case over:
      showOver();
      break;

    case success:
      showSuccess();
      break;

    case help:
      drawHelp(nHelpPage);
      break;
  }

  Platform::display();

#ifdef TIMER_
  // How long for one frame?
  Platform::DebugPrintln(Platform::millis() - step_start);
#endif // TIMER_

}

void restartGame(void) {
  popState();
  backToSquare1();
}

void enterMenu(State menu) {
  showMenu(menu);
  pushState(menu);

}

void exitMenu(State state) {
  popState();
  drawBackground();
  if (state == startupmenu) {
    backToSquare1();
    popState();
  }
}

uint8_t getLevel(void) {
  return level;
}

void setLevel(uint8_t newLevel) {
  level = newLevel;
  setObstLevel(level);
}

bool getI2C() {
  return bUseI2C;
}

void setI2C(bool onOff) {
  bUseI2C = onOff;
}

void showHelp() {
  drawHelp(nHelpPage);
  pushState(help);
}

void exitHelp() {
  popState(); // Help State
  switch (getState()) {
    case startupmenu:
      popState();
      break;
    case gamemenu:
      drawBackground();
      showRunning();
  }
}


void nextHelp() {
  nHelpPage = (nHelpPage + 1) % kHelpPages;
}

void previousHelp() {
  nHelpPage = nHelpPage > 0 ? nHelpPage - 1 : kHelpPages - 1;
}

void pushState(State state) {
  stateStack[++stkPtr] = state;
}

State popState() {
  uint8_t lvl = stkPtr == 0 ? 0 : --stkPtr;
  return stateStack[lvl];
}

State getState() {
  return stateStack[stkPtr];
}

void showStartup() {
  drawStartup();
  pushState(startupmenu);
  showMenu(startupmenu);
}

void showRunning() {
  SensorValues sensors {0, 0, FREE};
  SharedData shared; // Remote data received from Map

  moveVehicle(); // Move according to heading and speed
  // Try to receive sensor data from Map
  getSensors(&sensors, &shared);
  computeGameState(&sensors);

  drawI2C(bUseI2C);
  drawCompass(sensors.heading, sensors.alpha, sensors.tgt_heading);
  drawStatus(sensors.speed, sensors.tgt_distance);
  drawDistances(&sensors.distances);
  drawPosition(sensors.position);

  // Send vehicle rectangle to Map
  sendI2CVehicle();
  sendI2CRequest();
}

void handleCrash(void) {
  // Start of collision
  if (collisionStart == 0) {
    Vec position = getVehiclePos();
    drawCrash(&position);
    collisionStart = Platform::millis();
    // Reposition Vehicle at start.
    initVehicle(level);
    // Send new position to Map to give it time to recompute crash data
    sendI2CVehicle();
  } else {
    // Timeout Ended
    if (Platform::millis() > collisionStart + kCollisionPause) {
      popState();
      drawBackground();
      collisionStart = 0;
    }
  }
}

void showSuccess() {
  // Show result.
  uint16_t elapsed = (Platform::millis() - gameStart) / 1000;
  drawSuccess(elapsed);
  pushState(waiting);
}

void showOver() {
  uint16_t elapsed = (Platform::millis() - gameStart) / 1000;
  drawGameOver(elapsed);
  pushState(waiting);
}

void getSensors(SensorValues* sensors, SharedData* shared) {

  bool receiveOK; // True if received data is usable
  if (bUseI2C) {
    receiveOK = getSharedData(shared);
  }
  if (bUseI2C && receiveOK) {
    // User sensor data from Map
      checkSensors(sensors, FORWARD_REARWARD);
      useSharedData(sensors, shared);
  } else {
    // Compute sensor data ourselves
    checkSensors(sensors, BOTH);
  }
}

void computeGameState(SensorValues* sensors) {

  if (sensors->on_target) {
    pushState(over);
  } else if (sensors->collision != NONE) {
    pushState(crash);
  } else if ((sensors->tgt_distance < kTargetReached)
      && (sensors->speed == 0)) {
    pushState(success);
  }
}

void sendI2CVehicle() {
  I2CErrors error {I2C_NONE}; // For I2C communication
  vehicle_t vehicle;
  if (bUseI2C) {
    getVehicleRect(&vehicle.rect);
    vehicle.level = level;
    error = Platform::master_send((uint8_t*)&vehicle,
      sizeof(vehicle_t), I2C_SLAVE_ADDR);
    if (error) {
      I2C_Error(error);
      bUseI2C = false;
    }
  }
}

uint8_t sendI2CRequest() {
  uint8_t received;
  if (bUseI2C) {
    received = Platform::master_request(I2C_SLAVE_ADDR, sizeof(SharedData));
  }
  return received;
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
      break;
    default:
      drawMessage("I2C Error");
  }
  Platform::display();
  Platform::delay(5000);
  Platform::eraseRectRow(6, 24, kScreenWidth - 12, 15);
}

// Try to receive data requested from Map
bool getSharedData(SharedData* shared) {
  uint8_t received;
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
