/* vim: ts=2:sts=2:expandtab
 *
 * Implementation of intersection and distance functions
 * If the intersection is found, the variable X is set to the point
 * of intersection. We don't use pointers as return values. Takes too much
 * overhead.
 *
 */
#include "math.h"
#include "intersection.h"

static Vec X[2]; // Intersection points if they exists

uint8_t intersects_line(LineVector sensor, line_t line);
uint8_t intersects_line_vector(LineVector sensor, LineVector line);
uint8_t intersects_circle(LineVector sensor, circle_t circle);
uint8_t intersects_rectangle(LineVector sensor, rectangle_t rect);

Vec intersect_point(uint8_t i) {
  return X[i];
}

uint8_t intersects(LineVector sensor, obstacle obst) {

  switch (obst.type) {

    case LINE:
      return intersects_line(sensor, obst.line);
      break;

    case CIRCLE:
      return intersects_circle(sensor, obst.circle);
      break;

    case RECTANGLE:
      return intersects_rectangle(sensor, obst.rectangle);
      break;

    default:
      return 0;
  }
}

// Distance is
float distance(Vec origin, uint8_t n) {

  float d1 = origin.distance(X[0]), d2;

  if (n == 1) {
    return  d1;
  } else if (n > 1) {
    d2 = origin.distance(X[1]);
    return d1 < d2 ? d1 : d2;
  }

  // n = 0: Distance to nothing
  return NAN;
}

inline bool cmp01(float x) {
  return (x >= 0.0) && (x <= 1);
}

uint8_t intersects_line(LineVector sensor, line_t line) {
  return intersects_line_vector(sensor, LineVector(line));
}

uint8_t intersects_line_vector(LineVector sensor, LineVector line) {
  float det; // determinant
  float tau, nu; // scalars determining intersection point
  float s_xy, l_xy; // Either the x or the y component of a point
  Vec pl_minus_ps = line.p - sensor.p;
  uint8_t n = 0;

  det = -sensor.v.det(line.v); // = det(sensor.v, -line.v)
  // Are the lines parallel ?
  if (fabsf(det) < epsilon) {
    // Yes. Are (p_sigma - p_l) and v_l collinear?
    if (fabsf(pl_minus_ps.det(line.v)) > epsilon) {
      // No intersection.
      return 0;
    }
    // Yes. Is sensor a segment?
    if (!sensor.seg) {
      // No. sensor is unbound. The endpoints of the `line` segment
      // are on sensor.
      X[0] = line.p;
      X[1] = line.p + line.v;
      return 2;
    }
    // Yes, sensor is a segment. Check the four cases for intersections
    // intersection points are the endpoints of one segment which are
    // included in the other segment
    tau = -pl_minus_ps.div(line.v);
    if (cmp01(tau)) {
      X[n++] = sensor.p;
    }
    tau = (sensor.v - pl_minus_ps).div(line.v);
    if (cmp01(tau)) {
      X[n++] = sensor.p + sensor.v;
      if (n == 2) return 2;
    }
    nu = pl_minus_ps.div(sensor.v);
    if (cmp01(nu)) {
      X[n++] = line.p;
      if (n == 2) return 2;
    }
    nu = (line.v+pl_minus_ps).div(sensor.v);
    if (cmp01(nu)) {
      X[n++] = line.p + line.v;
    }
    return n;
  }

  // pl_minus_ps = (p_l - p_sigma)
  tau = (sensor.v.x * pl_minus_ps.y - sensor.v.y * pl_minus_ps.x) / det;

  // If line is a segment, the intersection must be within the segment.
  if (line.seg && ((tau < 0) || (tau > 1))) {
    return 0;
  }

  // Is `sensor` a segment?
  if (sensor.seg == 1) {
    // pl_minus_ps = -(p_sigma - p_l)
    nu = (line.v.x * pl_minus_ps.y - line.v.y * pl_minus_ps.x) / det;
    if ((nu < 0) || (nu > 1)) {
      return 0;
    }
  }

  // Compute intersection point
  X[0] = line.v * tau + line.p ;

  return 1;
}

uint8_t intersects_circle(LineVector sensor, circle_t circle) {

  Vec psig = sensor.p;
  Vec vsig = sensor.v;
  Vec pc = Vec(circle.p);
  Vec psigc = psig - pc;
  float b2 = vsig * psigc; // b2 = b/2
  float a = vsig.sq();
  // b4ac = (b/2)^2 - ac:
  float b4ac = b2 * b2 - a * (psigc.sq() - circle.r * circle.r);
  float xi;
  uint8_t n = 0;

  if (b4ac < 0) {
    return 0;
  }

  // xi_+ = (-b2 + sqrt(b4ac))/a
  xi = (-b2 + sqrtf(b4ac)) / a;
  if (!sensor.seg || cmp01(xi)) {
    X[n++] = psig + vsig * xi;
  }

  // One intersection only?
  if (b4ac < epsilon) {
    return n;
  }

  xi = (-b2 - sqrtf(b4ac)) / a;
  if (!sensor.seg || cmp01(xi)) {
    X[n++] = psig + vsig * xi;
  }
  return n;
}

uint8_t intersects_rectangle(LineVector sensor, rectangle_t rect) {
  uint8_t n_int; // Number of intersections
  LineVector rect_side; // Not initialized !!
  Vec v;

  // Left side
  rect_side = LineVector({rect.p, rect.w, rect.rho, true});
  v = rect_side.v; // Make a copy
  n_int = intersects_line_vector(sensor, rect_side);
  if (n_int) {
    // Make a copy of X[0] which will be overwritten
    // if there is a second intersection
    X[1] = X[0];
  }

  // Front
  rect_side.p += v;
  rect_side.v = v.rotate(90) * rect.mu;
  n_int += intersects_line_vector(sensor, rect_side);
  if (n_int == 2) {
    return 2;
  }

  // Right side
  rect_side.p += rect_side.v;
  rect_side.v = -v;
  n_int += intersects_line_vector(sensor, rect_side);
  if (n_int == 2) {
    return 2;
  }

  // Back
  rect_side.p += rect_side.v;
  rect_side.v = v.rotate(-90) * rect.mu;
  n_int += intersects_line_vector(sensor, rect_side);

  return n_int;
}

side_t collides(Vehicle veh, obstacle obst) {
  // Collision of a vehicle with an obstacle
  // Returns the first side where the collision was detected
  LineVector rect_side;

  // Detect a frontal collision first
  rect_side = LineVector(veh.p + veh.v, // p + v = front-left
                         veh.front);
  if (intersects(rect_side, obst))
    return FRONT;

  // Then a rear collision
  rect_side.p = veh.p + veh.front; // p+front = rear-right
  rect_side.v = -veh.front;
  if (intersects(rect_side, obst))
    return REAR;

  // Left side. p = rear-left
  rect_side = LineVector(veh.p, veh.v);
  if (intersects(rect_side, obst))
    return LEFT;

  // Right side
  rect_side.p += veh.v + veh.front;
  rect_side.v = -rect_side.v;
  if (intersects(rect_side, obst))
    return RIGHT;

  return NONE;
}
