#pragma once
#include "defines.h"
#include "objects.h"
#include "structs.h"
#include "mapstructs.h"

constexpr rectangle_t default_view = {{0.0, 0.0}, kScreenWidth, 0, kScreenHeight};
constexpr rectangle_t default_map = {{0.0, 0.0}, kBoardWidth, kBoardHeight};
constexpr float MinZoom = (float)kScreenWidth / (float)kBoardWidth;

struct LinePoints {
  point l0, l1;
};

void InitViewport(void);
void PanRight(void);
void PanLeft(void);
void PanDown(void);
void PanUp(void);
void ZoomIn(void);
void ZoomOut(void);
void ReCenter(const point p); // Re-center on point p
bool Changed(void); // True if something in the viewport has changed
void GetViewportPosition(point*);

// Transform a line from shapes. Return false if not in viewport.
bool GetLine(const line_t* line, LinePoints* screen_line); 
// Transform a circle from shapes. Return false if not in viewport.
bool GetCircle(const circle_t* circle, circle_t* screen_circle); 
// Transform a rectangle from shapes. Return false if not in viewport.
// Returns an array of lines in the viewport, null terminated
bool GetRectangle(const rectangle_t* rect, rectangle_t* lines); 

class ViewPort {

  public:
    // Constructors
    ViewPort() = default;
    ViewPort(rectangle_t map, rectangle_t view) : 
      map(map), view(view), scale(1.0), 
      h_step(view.l/2.0), v_step(view.w/2.0), moved(false) {}

    // Methods
    void pan_left(void);
    void pan_right(void);
    void pan_up(void);
    void pan_down(void);
    void move_to(const point v); // Move view in direction of v
    void zoom_in(void);
    void zoom_out(void);
    void re_center(const point p); 
    // Check whether map point is in viewport
    bool inside(const point p);
    bool inside(const Vec& p);
    // Transform ViewPort into an obstacle
    obstacle_t as_obstacle(void);
    // Transform a Map point into a screen point
    ScreenPt Map2Screen(const point p); 

    // Getters
    bool is_moved(void) {return moved;}
    float get_scale(void) {return scale;}
    rectangle_t get_rectangle(void) {return view;}

    // Setters
    
  private:
    rectangle_t map; // The rectangle defining the map.
    rectangle_t view; // The rectangle defining the viewport.
    float scale; // Precompute scale at each zoom.
    float h_step, v_step;
    bool moved; // True if the viewport has moved or is rescaled
};
// vim: ft=cpp:ts=2:sts=2:expandtab
