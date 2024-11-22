#pragma once
#include <math.h>
#include "src/defines.h"
#include "src/objects.h"
#include "src/structs.h"

/* The default view is the screen size. It **must** be a `rectangle_t` with
 * `rho = 0`. */
constexpr rectangle_t default_view = {kScreenWidth, 0.0, kScreenHeight, 0,
  kScreenWidth};

// Point to be drawn on the screen
constexpr int16_t int16(float x) {
  return (int16_t)lrintf(x);
}

// All sorts of constructors to get a point on the screen.
struct ScreenPt {
  int16_t x, y;
  constexpr ScreenPt(int16_t x, int16_t y) : x(x), y(y) {}
  constexpr ScreenPt(int32_t x, int32_t y) : x((int16_t)x), y((int16_t)y) {}
  constexpr ScreenPt(float x, float y) : x(int16(x)), y(int16(y)) {}
  constexpr ScreenPt(const point p) : ScreenPt(p.x, p.y) {}
  constexpr ScreenPt(const Vec& v) : ScreenPt(v.x, v.y) {}
  ScreenPt() = default;
};

// Endpoints of segments to draw.
struct LinePoints {
  ScreenPt l0, l1;
  LinePoints() = default;
  constexpr LinePoints(ScreenPt l0, ScreenPt l1) : l0(l0), l1(l1) {}
};

// Circle structure with screen coordinates
struct ScreenCircle {
  ScreenPt p;
  float r;
  ScreenCircle() = default;
  ScreenCircle(point p, float r) : p(ScreenPt(p)), r(r) {}
  ScreenCircle(circle_t c) : ScreenCircle(c.p, c.r) {}
};

void initViewport(rectangle_t* map);
void panRight(void);
void panLeft(void);
void panDown(void);
void panUp(void);
void moveTo(const point p);
void zoomIn(void);
void zoomOut(void);
void reCenter(const point p); // Re-center on point p
bool Changed(void); // True if something in the viewport has changed
void moveDone(void);
void getViewportPosition(point*);

class ViewPort;
void getViewport(ViewPort*);

// Get the endpoints of the line in the ViewPort for display
// on the screen.
uint8_t getLine(const line_t* line, LinePoints* s_line);
// Get the screen parameters of the circle in the viewport from the shape.
uint8_t getCircle(const circle_t* circle, ScreenCircle* s_circle);
// Get the four line segments in the viewport from the rectangle shape. 
// Return zero if not in viewport.
// Returns the number of line segments in the viewport.
// The array lines contains the endpoints of the lines to draw.
uint8_t getRectangle(const rectangle_t* rect, LinePoints* lines);

class ViewPort {

  public:
    // Constructors
    ViewPort() = default;
    ViewPort(rectangle_t map, rectangle_t view);

    // Methods
    void pan_left(void);
    void pan_right(void);
    void pan_up(void);
    void pan_down(void);
    void move_to(const point v); // Move view in direction of v
    void zoom_in(void);
    void zoom_out(void);
    void center_on(const point p);
    // Check whether map point is in viewport
    bool inside(const point p);
    bool inside(const Vec& p);
    // Transform ViewPort into an obstacle
    obstacle_t as_obstacle(void);
    // Transform a Map point into a screen point
    ScreenPt map2Screen(const Vec p) const;
    ScreenPt map2Screen(const point p) const;
    float scaled(float x) {return x * scale;} // Scale x to viewport length

    // Getters
    bool is_moved(void) const {return moved;}
    float get_scale(void) const {return scale;}
    point get_center(void) const;
    rectangle_t get_rectangle(void) const {return view;}

    // Setters
    void set_scale(float s);
    void not_moved(void);

  private:
    rectangle_t map; // The rectangle defining the map.
    rectangle_t view; // The rectangle defining the viewport.
    float min_zoom; // Min zoom = max zoom out
    float scale; // Precompute scale at each zoom.
    float h_step, v_step;
    bool moved; // True if the viewport has moved or is rescaled
};
// vim: ft=cpp:ts=2:sts=2:expandtab
