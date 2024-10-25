// Dispatching user events to the appropriate functions,
// depending on the global State type variable state.
// Emulates "registring listeners"
#include "controller.h"
#include "viewport.h"
#include "vehicle.h"
#include "game.h"
#include "defines.h"
#include "platform.h"

uint8_t previousButtons, currentButtons;
uint8_t DebouncedButtons();


void Dispatch(Event e) {
/* For each event, call all "listeners" that subscribe to that event.
 * The listeners decide whether to act on the event. */
  switch (e) {
    case Short_A:
      ZoomIn();
      break;

    case Long_A:
      ReCenter();
      break;

    case Short_B:
      ZoomOut();
      break;

    case Long_B:
      break;

    case Left:
      PanLeft();
      break;

    case Right:
      PanRight();
      break;

    case Up:
      PanUp();
      break;

    case Down:
      PanDown();
      break;
    default:
      ;
#ifdef _DEBUG
      Platform::DebugPrint("default in Dispatch");
      Platform::DebugPrintln();
#endif
  }
}

bool JustPressed(uint8_t buttons) {
  return ((buttons & currentButtons) && !(buttons & previousButtons));
}

bool JustReleased(uint8_t buttons) {
  return (!(buttons & currentButtons) && (buttons & previousButtons));
}

void HandleInput() {

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
    Dispatch(Short_A);
  }

  if (JustReleased(INPUT_A)) {
    AButtonDown = false;
    startAPress = Platform::millis();
  }

  if (AButtonDown && (Platform::millis() - startAPress) > LONG_PRESS) {
    startAPress = Platform::millis();
    Dispatch(Long_A);
  }

  // These events available for now:
  if (JustPressed(INPUT_B)) {
    BButtonDown = true;
    startBPress = Platform::millis();
    Dispatch(Short_B);
  }

  if (JustReleased(INPUT_B)) {
    BButtonDown = false;
    startBPress = Platform::millis();
    BButtonLongPressCycles = 0;
  }

  if (BButtonDown && (Platform::millis() - startBPress) > LONG_PRESS) {
    startBPress = Platform::millis();
    BButtonLongPressCycles++;
    Dispatch(Long_B);
  }

  if (currentButtons & INPUT_UP) Dispatch(Up);
  else if (currentButtons & INPUT_DOWN) Dispatch(Down);
  else if (currentButtons & INPUT_LEFT) Dispatch(Left);
  else if (currentButtons & INPUT_RIGHT) Dispatch(Right);

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

// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab
