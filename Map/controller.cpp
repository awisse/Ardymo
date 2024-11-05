// Dispatching user events to the appropriate functions,
// depending on the global State type variable state.
// Emulates "registring listeners"
#include "src/controller.h"
#include "src/defines.h"
#include "src/platform.h"
#include "viewport.h"
#include "menu.h"
#include "game.h"

static uint8_t previousButtons, currentButtons;
uint8_t debouncedButtons();
void dispatchShortA(State);
void dispatchLongA(State);
void dispatchShortB(State);
void dispatchLongB(State);
void dispatchLeft(State);
void dispatchRight(State);
void dispatchUp(State);
void dispatchUpA(State);   // Up with modifier A
void dispatchDown(State);
void dispatchDownA(State); // Down with modifier A


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

  if (currentButtons & INPUT_UP) {
    if (AButtonDown) {
      dispatchUpA(gameState);
    } else {
      dispatchUp(gameState);
    }
  }
  else if (currentButtons & INPUT_DOWN) {
    if (AButtonDown) {
      dispatchDownA(gameState);
    } else {
      dispatchDown(gameState);
    }
  }
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
      menuSelect();
      break;
  }
}

void dispatchLongA(State gameState) { }

void dispatchShortB(State gameState) {
  switch (gameState) {
    case gamemenu:
      exitMenu();
      break;
    case running:
      enterMenu();
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
      panLeft();
      break;
    case gamemenu:
      menuLeft();
      break;
    case help:
      helpNext();
      break;
  }
}

void dispatchRight(State gameState) {
  switch (gameState) {
    case running:
      panRight();
      break;
    case gamemenu:
      menuRight();
      break;
    case help:
      helpNext();
      break;
  }
}

void dispatchUp(State gameState) {
  switch (gameState) {
    case running:
      panUp();
      break;
    case gamemenu:
      menuUp();
      break;
  }
}

void dispatchUpA(State gameState) {
  switch (gameState) {
    case running:
      zoomOut();
      break;
  }
}

void dispatchDown(State gameState) {
  switch (gameState) {
    case running:
      panDown();
      break;
    case gamemenu:
      menuDown();
      break;
  }
}

void dispatchDownA(State gameState) {
  switch (gameState) {
    case running:
      zoomIn();
      break;
  }
}

// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
