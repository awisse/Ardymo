// vim: ts=2:sts=2:expandtab
//
// Vehicle manouvering
//
#ifdef ARDUINO
#include "math.h"
#else
#include <limits>
#define INFINITY std::numeric_limits<float>::infinity()
#endif
#include "vehicle.h"
#include "intersection.h"
#include "globals.h"
#include "defines.h"
#include "shapes.h"
#include "target.h"
#include "utils.h"

static Vehicle vehicle;
static Target target;

// Direction of vector v of sensor to check for distance.
enum direction_t {
  POSITIVE = 0,
  NEGATIVE = 1
};

// Information about closest obstacle - or collision.
struct closest_t {
  uint8_t index_positive; // Index of closest obstacle, positive v-direction
  uint8_t index_negative; // Index of closest obstacle, negative v-direction
  distance_t type; // FREE (No obstacle in sight), INTERSECTION, COLLISION.
};

// Helper functions
closest_t GetDistances(LineVector sensor, float* distances);

void InitVehicle() {
  vehicle = Vehicle(kVehicle);
  target = InitTarget();
}

void GetVehicleRect(rectangle_t* rect) {
  *rect = vehicle.as_rectangle();
}

void SetVehicleRect(rectangle_t* rect) {
  vehicle.set_rectangle(rect);
}

void TurnRight(void) {
  vehicle.turn(AngleStep);
}

void TurnLeft(void) {
  vehicle.turn(-AngleStep);
}

void AccelerateForward(void) {
  vehicle.accelerate_forward();
}

void AccelerateBackward(void) {
  vehicle.accelerate_backwards();
}

void MoveVehicle(void) {
  vehicle.move();
}

void CheckSensors(SensorValues* sensors) {
  // There are in principle eight sensor rays
  // emanating from the vehicle, two from each corner parallel
  // to the sides of the vehicle (see Figure 7 in ardymo.pdf).
  LineVector sensor;
  Vec tgt_heading;
  float d; // Temporary distance variable
  float distances[2];
  uint8_t n;
  closest_t closest; // Information on closest obstacle

  // First set sensors without computations
  sensors->heading = vehicle.v().normalized().as_point();

  // Position: Center of front
  sensors->position = vehicle.p() + vehicle.v() + vehicle.front() / 2.0;
  sensors->alpha = vehicle.rho();
  sensors->speed = vehicle.get_speed();

  // Target
  tgt_heading = target.p - sensors->position;
  sensors->tgt_distance = tgt_heading.length() - target.r;
  // Compute angle with respect to vehicle heading
  sensors->tgt_heading = tgt_heading
    .rotate(180 - sensors->alpha)
    .normalized()
    .as_point();

  // Reset sensors to be computed
  sensors->on_target = false;
  sensors->collision = NONE;

  // ======================== Front left-right ============================
  sensor = LineVector(vehicle.p() + vehicle.v(), -vehicle.front(),
      vehicle.width(), 0);
  closest = GetDistances(sensor, distances);
  // Collision - Front
  if (closest.type == COLLISION) {
      sensors->collision = FRONT;
      if (closest.index_positive == 0) {
        sensors->on_target = true;
      }
      return;
  }
  // RIGHTWARD
  sensors->distances.right = distances[NEGATIVE] - vehicle.width();
  // LEFTWARD (-vehicle.front() points left)
  sensors->distances.left = distances[POSITIVE];

  // ======================== Left forward-rearward =======================
  sensor = LineVector(vehicle.p() + vehicle.v(), vehicle.v(),
      vehicle.length(), 0);
  closest = GetDistances(sensor, distances);
  // Collision - Left
  if (closest.type == COLLISION) {
      sensors->collision = LEFT;
      if (closest.index_positive == 0) {
        sensors->on_target = true;
      }
      return;
  }
  // FORWARD
  sensors->distances.front = distances[POSITIVE];
  // REARWARD
  sensors->distances.rear = distances[NEGATIVE] - vehicle.length();

  // ======================== Right forward-rearward ======================
  sensor = LineVector(vehicle.p() + vehicle.front(), -vehicle.v(),
      vehicle.length(), 0);
  closest =  GetDistances(sensor, distances);
  // Collision - Right
  if (closest.type == COLLISION) {
    sensors->collision = RIGHT;
    if (closest.index_positive == 0) {
      sensors->on_target = true;
    }
    return;
  }
  // REARWARD (-vehicle.v points rearward)
  if (distances[POSITIVE] < sensors->distances.rear) {
    sensors->distances.rear = distances[POSITIVE];
  }
  // FORWARD
  if (distances[NEGATIVE] < sensors->distances.front + vehicle.length()) {
    sensors->distances.front = distances[NEGATIVE] - vehicle.length();
  }

  // ======================== Rear left-right =============================
  sensor = LineVector(vehicle.p(), -vehicle.front(),
      vehicle.width(), 0);
  closest = GetDistances(sensor, distances);
  // Collision - Rear
  if (closest.type == COLLISION) {
    sensors->collision = REAR;
    if (closest.index_positive == 0) {
      sensors->on_target = true;
    }
    return;
  }
  // RIGHTWARD (-vehicle.front points leftward)
  if (distances[NEGATIVE] < (sensors->distances.right + vehicle.width())) {
    sensors->distances.right = distances[NEGATIVE] - vehicle.width();
    }
  // LEFTWARD
  if (distances[POSITIVE] < sensors->distances.left) {
    sensors->distances.left = distances[POSITIVE];
  }

}

void Vehicle::turn(float alpha) {
  // Avoid incremental rotations. Will be off due to float precision
  rect.rho += alpha;
  if (rect.rho > 180) {
    rect.rho -= 360;
  } else if (rect.rho < -180) {
    rect.rho += 360;
  }
  rect.v = Vec(0, rect.l).rotate(rect.rho);
  // Turning around the center of the rear of the vehicle, which is
  Vec rear_center = rect.p + rect.front / 2.0;
  // Now turn front and use it to recompute p
  rect.front = Vec(0, rect.w).rotate(rect.rho + 90);
  rect.p = rear_center - rect.front / 2.0;
}

void Vehicle::move(void) {
  // Move by one unit of speed
  rect.p += rect.v * step;
}

rectangle_t Vehicle::as_rectangle(void) {
  return rect.as_rectangle();
}

void Vehicle::accelerate_forward(void) {
  speed = speed >= MaxSpeed ? MaxSpeed : speed + SpeedStep;
  step = speed / 1000.0 * kFrameDuration / rect.l;
}

void Vehicle::accelerate_backwards(void) {
  // Max backward speed = half max speed
  speed = speed <= -MaxSpeed / 2.0 ? - MaxSpeed / 2.0 : speed - SpeedStep;
  step = speed / 1000.0 * kFrameDuration / rect.l;
}

void Vehicle::set_rectangle(const rectangle_t* r) {
  rect = RectVector(*r);
}

closest_t GetDistances(LineVector sensor, float* distances) {
  // Compute distance to all obstacles.
  // Return the index of the closest
  obstacle_t obst;
  intersection_t ix; // Point of intersection
  float d {};
  size_t obstacle_count = sizeof(obstacles) / sizeof(obstacle_t);
  int16_t i, j; // Loop variables
  uint8_t intersection_count {};
  closest_t closest {0, 0, FREE};

  // Set Distances to infinity
  distances[0] = distances[1] = INFINITY;
  for (i=0; i<obstacle_count; i++) {
    get_obstacle(&obst, i);
    intersection_count = intersects(sensor, obst);
    for (j=0; j<intersection_count; j++) {
      ix = intersect_point(j);
      d = distance(sensor.p, j);
      closest.type = INTERSECTION;
      if ((ix.nu > 0) && (d < distances[POSITIVE])) {
        distances[POSITIVE] = d;
        // obstacle i is now the closest
        closest.index_positive = i;
      } else if ((ix.nu > -1.0) && (ix.nu < 0)) {
        // Collision with obstacle i
        closest.type = COLLISION;
        closest.index_positive = i;
        return closest;
      } else if (d < distances[NEGATIVE]) {
        distances[NEGATIVE] = d;
        closest.index_negative = i;
      }
    }
  }
  return closest;
}
