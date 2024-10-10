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

uint8_t intersects_line(line_t sensor, line_t line);
uint8_t intersects_circle(line_t sensor, circle_t circle);
uint8_t intersects_rectangle(line_t sensor, rectangle_t rect);
line_t to_segment(Vec p, Vec v);

/* det(v_0, v_1): Determinant of 2-dimensional matrix composed of two column
 * vectors v_0 and v_1.
 */
float determinant(point v0, point v1) {
  return v0.x * v1.y - v0.y * v1.x;
}

Vec intersect_point(uint8_t i) {
  return X[i];
}

uint8_t intersects(line_t sensor, obstacle obst) {

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

uint8_t intersects_line(line_t sensor, line_t line) {
  float det; // determinant
  float tau, nu; // scalars determining intersection point
  float s_xy, l_xy; // Either the x or the y component of a point
  Vec pl = Vec(line.p);
  Vec ps = Vec(sensor.p);
  Vec vl = Vec(line.v);
  Vec vs = Vec(sensor.v);
  Vec ps_minus_pl = ps - pl;
  uint8_t n = 0;

  det = -vs.det(vl); // = det(sensor.v, -line.v)
  // Are the lines parallel ?
  if (fabsf(det) < epsilon) {
    // Yes. Are (p_sigma - p_l) and v_l collinear?
    if (fabsf(ps_minus_pl.det(vl)) > epsilon) {
      // No intersection.
      return 0;
    }
    // Yes. Is sensor a segment?
    if (!sensor.seg) {
      // No. sensor is unbound. The endpoints of the `line` segment
      // are on sensor.
      X[0] = pl;
      X[1] = pl + vl;
      return 2;
    }
    // Yes, sensor is a segment. Check the four cases for intersections
    // intersection points are the endpoints of one segment which are
    // included in the other segment
    tau = ps_minus_pl.div(vl);
    if (cmp01(tau)) {
      X[n++] = ps;
    }
    tau = (ps_minus_pl + vs).div(vl);
    if (cmp01(tau)) {
      X[n++] = ps + vs;
      if (n == 2) return 2;
    }
    nu = -ps_minus_pl.div(vs);
    if (cmp01(nu)) {
      X[n++] = pl;
      if (n == 2) return 2;
    }
    nu = (vl-ps_minus_pl).div(vs);
    if (cmp01(nu)) {
      X[n++] = pl + vl;
    }
    return n;
  }

  // ps_minus_pl = -(p_l - p_sigma)
  tau = (sensor.v.y * ps_minus_pl.x - sensor.v.x * ps_minus_pl.y) / det;

  // If this is a segment, the intersection must be within the segment.
  if (line.seg && ((tau < 0) || (tau > 1))) {
    return 0;
  }

  // Is `sensor` a segment?
  if (sensor.seg == 1) {
    // Compute nu
    nu = (line.v.y * ps_minus_pl.x - line.v.y * ps_minus_pl.y) / det;
    if ((nu < 0) || (nu > 1)) {
      return 0;
    }
  }

  // Compute intersection point
  X[0] = Vec(line.v) * tau + line.p ;

  return 1;
}

uint8_t intersects_circle(line_t sensor, circle_t circle) {

  Vec psig = Vec(sensor.p);
  Vec vsig = Vec(sensor.v);
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

inline line_t to_segment(Vec p, Vec v) {
  line_t line = {p.as_point(), v.as_point(), 1};
  return line;
}

uint8_t intersects_rectangle(line_t sensor, rectangle_t rect) {
  Vec v = Vec(rect.v);
  Vec p = Vec(rect.p);
  Vec side;
  uint8_t n_int; // Number of intersections
  line_t rect_side = {rect.p, rect.v, 1};

  // Left side
  n_int = intersects_line(sensor, rect_side);
  if (n_int) {
    // Make a copy of X[0] which will be overwritten
    // if there is a second intersection
    X[1] = X[0];
  }

  // Front
  side = v.rotate(90) * rect.mu;
  p += v;
  rect_side = to_segment(p, side);
  n_int += intersects_line(sensor, rect_side);
  if (n_int == 2) {
    return 2;
  }

  // Right side
  p += side;
  rect_side = to_segment(p, -v);
  n_int += intersects_line(sensor, rect_side);
  if (n_int == 2) {
    return 2;
  }

  // Back
  p += -v;
  rect_side = to_segment(p, -side);
  n_int += intersects_line(sensor, rect_side);

  return n_int;
}

side_t collides(rectangle_t rect, obstacle obst) {
  // Collision of a rectangle with an obstacle
  // Returns the first side where the collision was detected
  Vec v = Vec(rect.v);
  Vec p = Vec(rect.p);
  Vec front;
  line_t rect_side;

  // Detect a frontal collision first
  front = v.rotate(90) * rect.mu;
  rect_side = to_segment(p + v, front); // p+v = front-left
  if (intersects(rect_side, obst))
    return FRONT;

  // Left side. p = rear-left
  rect_side = {rect.p, rect.v, 1};
  if (intersects(rect_side, obst))
    return LEFT;


  // Right side
  p += v + front; // p = front-right
  rect_side = to_segment(p, -v);
  if (intersects(rect_side, obst))
    return RIGHT;

  // Rear
  p += -v; // p = rear-right
  rect_side = to_segment(p, -front);
  if (intersects(rect_side, obst))
    return REAR;

  return NONE;
}
