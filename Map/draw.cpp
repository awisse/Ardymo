// All the drawing takes place here.
#include "src/defines.h"
#include "src/globals.h"
#include "src/platform.h"
#include "src/objects.h"
#include "viewport.h"
#include "src/shapes.h"
#include "src/utils.h"
#include "draw.h"

// Center of screen
static Vec centre = {(float)((kScreenWidth>>1)-1),
                     (float)((kScreenHeight>>1)-1)};
// ViewPort

// Helper functions
void DrawLine(const line_t* line);
void DrawCircle(const circle_t* circle);
void DrawRectangle(const rectangle_t* rect);

void Draw(rectangle_t* vehicle) {
  // Draw the viewport. The vehicle is treated as the argument.
  obstacle_t obst;
  uint8_t i;
  constexpr size_t n = sizeof(obstacles) / sizeof(obstacle_t);

  // Start with the vehicle
  DrawRectangle(vehicle);

  // All obstacles
  for (i=0; i<n; i++) {
    get_obstacle(&obst, i);
    switch (obst.type) {
      case LINE:
        DrawLine(&obst.item.line);
        break;
      case CIRCLE:
        DrawCircle(&obst.item.circle);
        break;
      case RECTANGLE:
        DrawRectangle(&obst.item.rectangle);
        break;
    }
  }
}

void DrawPosition(point* vehicle_position) {

  Platform::eraseRectRow(0, kScreenHeight - 16, kStatusX - 1, 15);
  Platform::setCursor(1, kScreenHeight - kStatusY + 2);
  Platform::print("x ");
  Platform::print((int16_t)vehicle_position->x);
  Platform::setCursor(1, kScreenHeight - kStatusY + 10);
  Platform::print("y ");
  Platform::print((int16_t)vehicle_position->y);
}

// Helper functions
void DrawLine(const line_t* line) {
  LinePoints lpts;
  // Find the line segments which are actually in the viewport
  if (GetLine(line, &lpts)) {
    Platform::drawLine(lpts.l0.x, lpts.l0.y, lpts.l1.x, lpts.l1.y);
  }
}

void DrawCircle(const circle_t* circle) {
  // Draw the bloody circle as is
  ScreenCircle screen_circle;
  if (GetCircle(circle, &screen_circle)) {
    Platform::drawCircle(screen_circle.p.x, screen_circle.p.y, screen_circle.r);
  }
}

void DrawRectangle(const rectangle_t* rect) {
  // Draw the (up to) four line segments
  LinePoints segments[4];
  uint8_t i, n;

  n = GetRectangle(rect, segments);
  for (i=0; i<n; i++) {
    Platform::drawLine(segments[i].l0.x, segments[i].l0.y,
        segments[i].l1.x, segments[i].l1.y);
  }
}
// vim:fdm=syntax
