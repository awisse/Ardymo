#include "shapes.h"
#include "viewport.h"
#include "vehicle.h"

rectangle_t vehicle;

void InitVehicle(void) {
  vehicle = kVehicle;
}

void GetVehicle(rectangle_t* rect) {
  *rect = vehicle;
}

void SetVehicle(rectangle_t* rect) {
  vehicle = *rect;
}

void ReCenter(void) {
  ReCenter(vehicle.p);  
}
// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
