// Dispatching user events to the appropriate functions,
// depending on the global State type variable state.
// Emulates "registring listeners"
#include "controller.h"
#include "vehicle.h"
#include "game.h"
#include "menu.h"
#include "defines.h"
#include "platform.h"

static uint8_t previousButtons, currentButtons;
uint8_t debouncedButtons();
void dispatchShortA(State);
void dispatchLongA(State);
void dispatchShortB(State);
void dispatchLongB(State);
void dispatchLeft(State);
void dispatchRight(State);
void dispatchUp(State);
void dispatchDown(State);


bool JustPressed(uint8_t buttons) {
  return ((buttons & currentButtons) && !(buttons & previousButtons));
}

bool JustReleased(uint8_t buttons) {
  return (!(buttons & currentButtons) && (buttons & previousButtons));
}

void handleInput(State gameState) {

  static uint32_t startAPress;
  static bool AButtonDown;
  static uint32_t startBPress;
  static bool BButtonDown;
  static int BButtonLongPressCycles;

  previousButtons = currentButtons;
  currentButtons = Platform::buttonsState();

  if (JustPressed(INPUT_A)) {
    AButtonDown = true;
    startAPress = Platform::millis();
    dispatchShortA(gameState);
  }

  if (JustReleased(INPUT_A)) {
    AButtonDown = false;
    startAPress = Platform::millis();
  }

  if (AButtonDown && (Platform::millis() - startAPress) > LONG_PRESS) {
    startAPress = Platform::millis();
    dispatchLongA(gameState);
  }

  // These events available for now:
  if (JustPressed(INPUT_B)) {
    BButtonDown = true;
    startBPress = Platform::millis();
    dispatchShortB(gameState);
  }

  if (JustReleased(INPUT_B)) {
    BButtonDown = false;
    startBPress = Platform::millis();
    BButtonLongPressCycles = 0;
  }

  if (BButtonDown && (Platform::millis() - startBPress) > LONG_PRESS) {
    startBPress = Platform::millis();
    BButtonLongPressCycles++;
    dispatchLongB(gameState);
  }

  if (currentButtons & INPUT_UP) dispatchUp(gameState);
  else if (currentButtons & INPUT_DOWN) dispatchDown(gameState);
  else if (currentButtons & INPUT_LEFT) dispatchLeft(gameState);
  else if (currentButtons & INPUT_RIGHT) dispatchRight(gameState);

}

uint8_t DebouncedButtons() {
  // Wait for release
  static bool pressed;

  if (!currentButtons) {
    pressed = false;
    return 0;
  } else if (pressed) {
    return 0;
  }

  pressed = true;
  return currentButtons;
}

void dispatchShortA(State gameState) {
  switch (gameState) {
    case gamemenu:
    case startupmenu:
      menuSelect(gameState);
      break;
    case waiting:
      initGame();
      break;
  }
}

void dispatchLongA(State gameState) { }

void dispatchShortB(State gameState) {
  switch (gameState) {
    case gamemenu:
    case startupmenu:
      exitMenu(gameState);
      break;
    case running:
      enterMenu(gamemenu);
      break;
    case help:
      exitHelp();
      break;
  }
}

void dispatchLongB(State gameState) {}

void dispatchLeft(State gameState) {
  switch (gameState) {
    case running:
      turnLeft();
      break;
    case gamemenu:
    case startupmenu:
      menuLeft();
      break;
    case help:
      previousHelp();
      break;
  }
}

void dispatchRight(State gameState) {
  switch (gameState) {
    case running:
      turnRight();
      break;
    case gamemenu:
    case startupmenu:
      menuRight();
      break;
    case help:
      nextHelp();
      break;
  }
}
void dispatchUp(State gameState) {
  switch (gameState) {
    case running:
      accelerateForward();
      break;
    case gamemenu:
    case startupmenu:
      menuUp();
      break;
  }
}

void dispatchDown(State gameState) {
  switch (gameState) {
    case running:
      accelerateBackward();
      break;
    case gamemenu:
    case startupmenu:
      menuDown();
      break;
  }
}
// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
