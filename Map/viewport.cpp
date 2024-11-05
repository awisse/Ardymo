#include "src/intersection.h"
#include "viewport.h"

ViewPort viewport;

void initViewport(void) {
    viewport = ViewPort(default_map, default_view);
}

void panRight(void) {
  viewport.pan_right();
}

void panLeft(void) {
  viewport.pan_left();
}

void panDown(void) {
  viewport.pan_down();
}

void panUp(void) {
  viewport.pan_up();
}

void moveTo(const point p) {
  viewport.move_to(p);
}

void zoomIn(void) {
  viewport.zoom_in();
}

void zoomOut(void) {
  viewport.zoom_out();
}

void reCenter(const point p) {
  viewport.center_on(p);
}

bool Changed(void) {
  return viewport.is_moved();
}

void moveDone(void) {
  viewport.not_moved();
}

void getViewportPosition(point* pos) {
  *pos = viewport.get_rectangle().p;
}

void getViewport(ViewPort* v) {
  *v = viewport;
}


float bound(float x, float a, float b) {
  // Return x if a <= x <= b.
  // Return a if x < a, return b if x > b.
  if (x < a) {
    return a;
  }
  if (x > b) {
    return b;
  }
  return x;
}

void ViewPort::move_to(const point v) {
  float to_x = v.x;
  float to_y = v.y;
  view.p.x = bound(to_x, 0.0, map.p.x + map.l - view.l);
  view.p.y = bound(to_y, 0.0, map.p.y + map.w - view.w);
  moved = true;
}

void ViewPort::pan_left(void) {
  move_to({view.p.x - h_step, view.p.y});
}

void ViewPort::pan_right(void) {
  move_to({view.p.x + h_step, view.p.y});
}

void ViewPort::pan_up(void) {
  move_to({view.p.x, view.p.y - v_step});
}

void ViewPort::pan_down(void) {
  move_to({view.p.x, view.p.y + v_step});
}

void ViewPort::zoom_in(void) {
  point c = get_center();

  if (scale < 1.0) {
    scale *= 2.0;
    view.l /= 2.0;
    view.w /= 2.0;
    h_step /= 2.0;
    v_step /= 2.0;
    moved = true;
  }
  center_on(c);
}

void ViewPort::zoom_out(void) {
  point c = get_center();

  if (scale > MinZoom) {
    scale /= 2.0;
    view.l *= 2.0;
    if (view.l > kBoardWidth) {
      view.l = kBoardWidth;
    }
    view.w *= 2.0;
    if (view.w > kBoardHeight) {
      view.w = kBoardHeight;
    }
    h_step = view.l / 2.0;
    v_step = view.w / 2.0;
    moved = true;
  }
  center_on(c);
}

void ViewPort::center_on(const point p) {
  // Center the viewport on the point p.
  point to_p;
  to_p.x = p.x - view.l / 2.0;
  to_p.y = p.y - view.w / 2.0;
  move_to(to_p);
}

bool ViewPort::inside(const point p) {
  return (p.x >= view.p.x) && (p.y >= view.p.y)
    && (p.x <= view.p.x + view.l) && (p.y <= view.p.y + view.w);
}

bool ViewPort::inside(const Vec& p) {
  return inside(p.as_point());
}

obstacle_t ViewPort::as_obstacle(void) {

  obstacle_t obst;
  obst.type = RECTANGLE;
  obst.item.rectangle = this->get_rectangle();
  return obst;

}

ScreenPt ViewPort::map2Screen(const Vec v) const {
  return map2Screen(v.as_point());
}

ScreenPt ViewPort::map2Screen(const point p) const {
  ScreenPt s_p = ScreenPt(scale * (p.x - view.p.x),
      scale * (p.y - view.p.y));
  return s_p;
}

point ViewPort::get_center(void) const {
  point p;
  p.x = view.p.x + view.l / 2;
  p.y = view.p.y + view.w / 2;
  return p;
}

void ViewPort::set_scale(float s) {
  scale = bound(s, MinZoom, 1.0);
}

inline void ViewPort::not_moved(void) {
  moved = false;
}

uint8_t getLine(const line_t* line, LinePoints* s_line) {

  LineVector l = LineVector(*line);
  obstacle_t vpObst = viewport.as_obstacle();
  uint8_t n; // Number of intersection points

  Vec l1 = l.p + l.v; // Endpoint of line
  bool inside0 = viewport.inside(line->p);
  bool inside1 = viewport.inside(l1);

  // Is line contained in Viewport?
  if (inside0 && inside1) {
    s_line->l0 = viewport.map2Screen(line->p);
    s_line->l1 = viewport.map2Screen(l1);
    return 1;
  }

  // Find intersections of line with Viewport.
  n = intersects(&l, &vpObst);
  if (n == 0) { // Neither inside nor intersection: nothing to draw
    return 0;
  }
  s_line->l0 = viewport.map2Screen(intersect_point(0).p);

  if (n == 2) { // Line completely cuts through the viewport
    s_line->l1 = viewport.map2Screen(intersect_point(1).p);
    return 1;
  }

  // The other endpoint of the screen line is an endpoint of the line
  // Find which one is inside the viewport
  if (inside0) {
    s_line->l1 = viewport.map2Screen(line->p);
  } else { // Must be inside1
    s_line->l1 = viewport.map2Screen(l1);
  }

  return true;
}

bool is_in_circle(const Vec& p, const circle_t* circle) {
  return (p.distance(Vec(circle->p)) < circle->r);
}

uint8_t getCircle(const circle_t* circle, ScreenCircle* s_circle) {

  // Just scale the circle to the viewport for now, after checking whether
  // it is completely outside the viewport
  rectangle_t vrect = viewport.get_rectangle();
  // Completely outside the viewport?
  if (is_in_circle(Vec(vrect.p), circle) &&
      is_in_circle(Vec(vrect.p.x + vrect.l, vrect.p.y), circle) &&
      is_in_circle(Vec(vrect.p.x, vrect.p.y + vrect.w), circle) &&
      is_in_circle(Vec(vrect.p.x + vrect.l, vrect.p.y + vrect.w), circle)) {
    return 0;
  }

  s_circle->p = viewport.map2Screen(circle->p);
  s_circle->r = viewport.scaled(circle->r);
  return 1;
}

// Transform a rectangle from shapes. Return the number of segments
// in the viewport. Return the endpoints of the segments in `lines`.
uint8_t getRectangle(const rectangle_t* rect, LinePoints* lines) {
  uint8_t n=0; // Maybe no intersection at all. Must initialize to zero.
  uint8_t i;
  RectVector rv = RectVector(*rect);
  LinePoints line;
  // We have up to 4 segments if the rectangle intersects
  // the viewport diagonally
  //
  // Left
  point p = rect->p; // Corners of the rectangle
  line_t segment = {p, rect->l, rect->rho, 1};
  if (i=getLine(&segment, &line)) {
    lines[n++] = line;
  }

  // Front. Direction towards right
  p = (rv.p + rv.v).as_point();
  segment = {p, rect->w, (int16_t)(rect->rho + 90), 1};
  if (i=getLine(&segment, &line)) {
    lines[n++] = line;
  }

  // Right. Direction towards front
  p = (rv.p + rv.front).as_point();
  segment = {p, rect->l, rect->rho, 1};
  if (i=getLine(&segment, &line)) {
    lines[n++] = line;
  }

  // Rear. Direction towards right
  p = rect->p;
  segment = {p, rect->w, (int16_t)(rect->rho + 90), 1};
  if (i=getLine(&segment, &line)) {
    lines[n++] = line;
  }

  return n;
}
// vim: ft=cpp:ts=2:sts=2:expandtab
