/* vim: ts=2:sts=2:expandtab
 *
 * Implementation of intersection and distance functions
 * If the intersection is found, the variable X is set to the point
 * of intersection. We don't use pointers as return values. Takes too much
 * overhead.
 *
 * Descriptions of the functions in intersection.h
 *
 */
#include "math.h"
#include "intersection.h"

static intersection_t X[2]; // Intersection points if they exists

uint8_t intersects_line(const LineVector* sensor, const line_t* line);
uint8_t intersects_line_vector(const LineVector* sensor,
    const LineVector* line);
uint8_t intersects_circle(const LineVector* sensor,
    const circle_t* circle);
uint8_t intersects_rectangle(const LineVector* sensor,
    const rectangle_t* rect);

intersection_t intersect_point(uint8_t i) {
  return X[i];
}

uint8_t intersects(const LineVector* sensor, const obstacle_t* obst) {

  switch (obst->type) {

    case LINE:
      return intersects_line(sensor, &(obst->item).line);
      break;

    case TARGET:
    case CIRCLE:
      return intersects_circle(sensor, &(obst->item).circle);
      break;

    case BORDER:
    case RECTANGLE:
      return intersects_rectangle(sensor, &(obst->item).rectangle);
      break;

    default:
      return 0;
  }
}

float distance(Vec origin, uint8_t n) {
  // Distance of `origin` to intersection point `n`.

  if (n > 1) return NAN; // At least avoid access outside bounds
  float d = origin.distance(X[n].p);
  return d;
}

inline bool cmp01(float x) {
  return (x >= 0.0) && (x <= 1);
}

uint8_t intersects_line(const LineVector* sensor, const line_t* line) {
  LineVector lv = LineVector(*line);
  return intersects_line_vector(sensor, &lv);
}

uint8_t intersects_line_vector(const LineVector* sensor,
    const LineVector* line) {
  float det; // determinant
  float tau, nu; // scalars determining intersection point
  float s_xy, l_xy; // Either the x or the y component of a point
  Vec pl_minus_ps = line->p - sensor->p;
  uint8_t n = 0;

  det = -sensor->v.det(line->v); // = det(sensor.v, -line.v)
  // Are the lines parallel ?
  if (fabsf(det) < epsilon) {
    // Yes. Are (p_sigma - p_l) and v_l collinear?
    if (fabsf(pl_minus_ps.det(line->v)) > epsilon) {
      // No intersection.
      return 0;
    }
    // Yes. Is sensor a segment?
    if (!sensor->seg) {
      // No. sensor is unbound. The endpoints of the `line` segment
      // are on sensor.
      // We have to find the υ of the intersection points.
      nu = pl_minus_ps.div(sensor->v);
      X[0] = {line->p, nu};
      nu = (pl_minus_ps+line->v).div(sensor->v);
      X[1] = {line->p + line->v, nu};
      return 2;
    }
    // Yes, sensor is a segment. Check the four cases for intersections.
    // Intersection points are the endpoints of one segment which are
    // included in the other segment
    tau = -pl_minus_ps.div(line->v);
    if (cmp01(tau)) {
      X[n++] = {sensor->p, 0.0};
    }
    tau = (sensor->v - pl_minus_ps).div(line->v);
    if (cmp01(tau)) {
      X[n++] = {sensor->p + sensor->v, 1.0};
      if (n == 2) return 2;
    }
    nu = pl_minus_ps.div(sensor->v);
    if (cmp01(nu)) {
      X[n++] = {line->p, nu};
      if (n == 2) return 2;
    }
    nu = (line->v+pl_minus_ps).div(sensor->v);
    if (cmp01(nu)) {
      X[n++] = {line->p + line->v, nu};
    }
    return n;
  }

  // pl_minus_ps = (p_l - p_sigma)
  tau = (sensor->v.x * pl_minus_ps.y - sensor->v.y * pl_minus_ps.x) / det;

  // If line is a segment, the intersection must be within the segment.
  if (line->seg && ((tau < 0) || (tau > 1))) {
    return 0;
  }
  nu = (line->v.x * pl_minus_ps.y - line->v.y * pl_minus_ps.x) / det;

  // Is `sensor` a segment?
  if (sensor->seg == 1) {
    // The intersection must be within the segment.
    // pl_minus_ps = -(p_sigma - p_l)
    if ((nu < 0) || (nu > 1)) {
      return 0;
    }
  }

  // Compute intersection point
  X[0] = {line->v * tau + line->p, nu};

  return 1;
}

uint8_t intersects_circle(const LineVector* sensor,
    const circle_t* circle) {

  Vec psig = sensor->p;
  Vec vsig = sensor->v;
  Vec pc = Vec(circle->p);
  Vec psigc = psig - pc;
  float b2 = vsig * psigc; // b2 = b/2
  float a = vsig.square();
  // b4ac = (b/2)^2 - ac:
  float b4ac = b2 * b2 - a * (psigc.square() - circle->r * circle->r);
  float xi;
  uint8_t n = 0;

  if (b4ac < 0) {
    return 0;
  }

  // xi_+ = (-b2 + sqrt(b4ac))/a
  xi = (-b2 + sqrtf(b4ac)) / a;
  if (!sensor->seg || cmp01(xi)) {
    X[n++] = {psig + vsig * xi, xi};
  }

  // One intersection only?
  if (b4ac < epsilon) {
    return n;
  }

  xi = (-b2 - sqrtf(b4ac)) / a;
  if (!sensor->seg || cmp01(xi)) {
    X[n++] = {psig + vsig * xi, xi};
  }
  return n;
}

uint8_t intersects_rectangle(const LineVector* sensor,
    const rectangle_t* rect) {
  uint8_t n_int, found; // Number of intersections
  LineVector rect_side; // Check the four sides of the rectangle
  intersection_t store_ix; // Store first intersection point found
  Vec v, front;

  // Left side
  rect_side = LineVector(Vec(rect->p), rect->l, rect->rho, 1);
  v = rect_side.v; // Make a copy
  n_int = intersects_line_vector(sensor, &rect_side);
  if (n_int) {
    // Make a copy of X[0] which will be overwritten
    // if there is a second intersection
    store_ix = X[0];
  }

  front = Vec(0, rect->w).rotate(rect->rho + 90);

  // Front
  rect_side.p += v;
  rect_side.v = front;
  rect_side.l = rect->w;
  found = intersects_line_vector(sensor, &rect_side);
  n_int += found;
  if (n_int == 2) {
    X[1] = store_ix;
    return 2;
  }
  if (found) {
    store_ix = X[0];
  }

  // Right side
  rect_side.p += front;
  rect_side.v = -v;
  rect_side.l = rect->l;
  found = intersects_line_vector(sensor, &rect_side);
  n_int += found;
  if (n_int == 2) {
    X[1] = store_ix;
    return 2;
  }
  if (found) {
    store_ix = X[0];
  }

  // Rear
  rect_side.p += rect_side.v;
  rect_side.v = -front;
  rect_side.l = rect->w;
  n_int += intersects_line_vector(sensor, &rect_side);
  if (n_int == 2) {
    X[1] = store_ix;
  }

  return n_int;
}
