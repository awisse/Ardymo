#include "shapes.h"
#include "viewport.h"
#include "vrect.h"

RectVector vehicle;

void InitVehicle(void) {
  vehicle = RectVector(kVehicle);
}

void GetVehicle(rectangle_t* rect) {
  *rect = vehicle.as_rectangle();
}

void SetVehicle(rectangle_t* rect) {
  vehicle = RectVector(*rect);
}

void ReCenter(void) {
  ReCenter(vehicle.p.as_point());
}
// vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab:filetype=cpp
