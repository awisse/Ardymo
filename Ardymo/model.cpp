// vim: ts=2:sts=2:expandtab

#include "model.h"
#include "structs.h"
#include "globals.h"

// For printing to Serial when debugging
#ifdef _DEBUG
#include "platform.h"
#endif

void CheckFront(void);
void CheckRear(void);
void CheckLeft(void);
void CheckRight(void);
void CheckTarget(void);

void MoveVehicle() {
  // Move vehicle by speed in its direction
}

/* Code for checking distances to all obstacles goes here. */
void CheckSensors() {

  // Populate the sensor_values structure
  CheckFront();
  CheckRear();
  CheckLeft();
  CheckRight();
  CheckTarget();
}


void CheckFront() {
}

void CheckRear() {
}

void CheckLeft() {
}

void CheckRight() {
}

void CheckTarget() {
}

