/*
Helper functions to unclutter main .ino file
 */
#include "game.h"
#include "model.h"
#include "draw.h"
#include "controller.h"
#include "globals.h"
#include "platform.h"

// Global variable
State state;   // startup, running, menu, success, over
// Local to game.cpp
bool modified; // True if screen needs to be redrawn
uint32_t start; // Milliseconds at start of game

// Functions
void DoMenu();
void Success();
void GameOver();

void InitGame() {

  state = startup;
  Platform::clear();
  state = running;
  modified = true;

}

void StepGame() {

  HandleInput();
  MoveVehicle();
  CheckSensors();

//  Platform::println("StepGame");
  Platform::DebugPrint("StepGame\n");
  if (modified) {
    if (state == running) {
      DrawScreen();
    } 
    modified = false;
  }
  Platform::display();
}

void Restart() {
  if ((state == success) || (state == running) || (state == over)) {
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
  state = over;
}

void Success () {
  // Show result.
  uint8_t stars;
  uint16_t elapsed = (Platform::millis() - start) / 1000;

}

// vim:fdm=syntax:tabstop=2:softtabstop=2:shiftwidth=2:expandtab
