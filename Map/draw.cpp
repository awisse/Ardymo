// All the drawing takes place here.
#include "defines.h"
#include "globals.h"
#include "platform.h"
#include "objects.h"
#include "viewport.h"
#include "shapes.h"
#include "draw.h"

// Center of screen
static Vec centre = {(float)((kScreenWidth>>1)-1), 
                     (float)((kScreenHeight>>1)-1)};

// Helper functions
void DrawLine(line_t* line);
void DrawCircle(circle_t* circle);
void DrawRectangle(rectangle_t* rect);

void DrawBackground() {
  // Draw main features of background:
  // 1a. rectangle in upper left corner.
  Platform::drawFastHLine(0, kStatusY, kStatusX);
  Platform::drawFastVLine(kStatusX, 0, kStatusY  + 1);
  // 1b. rectangle in lower left corner.
  Platform::drawFastHLine(0, kScreenHeight-kStatusY-1, kStatusX);
  Platform::drawFastVLine(kStatusX, kScreenHeight-kStatusY-1, kStatusY);
}

void Draw(rectangle_t* vehicle) {
  // Draw the viewport. The vehicle is treated as the argument.
}

void DrawStatus(float speed, float distance) {
  // Draw elapsed time and distance to target in top left corner
  Platform::eraseRectRow(0, 0, kStatusX - 1, 15);
  Platform::setCursor(1, 1);
  Platform::print(speed, 1);
  Platform::print("/s");
  Platform::setCursor(1, 9);
  Platform::print(distance, 0);
  Platform::print('m');
}

void DrawPosition(point* vehicle_position) {

  Platform::eraseRectRow(0, kScreenHeight - 16, kStatusX - 1, 15);
  Platform::setCursor(1, kScreenHeight - kStatusY + 1);
  Platform::print("x ");
  Platform::print((int16_t)vehicle_position->x);
  Platform::setCursor(1, kScreenHeight - kStatusY + 9);
  Platform::print("y ");
  Platform::print((int16_t)vehicle_position->y);
}

// Helper functions
void DrawLine(line_t* line) {
  LinePoints lpts; 
  // Find the line segments which are actually in the viewport
  if (GetLine(line, &lpts)) {
    Platform::drawLine(lpts.l0.x, lpts.l0.y, lpts.l1.x, lpts.l1.y);
  }
}

void DrawCircle(circle_t* circle) {
  // Draw the bloody circle as is
  circle_t screen_circle;
  if (GetCircle(circle, &screen_circle)) {
  }
}

void DrawRectangle(rectangle_t* rect) {
  // Draw the four line segments
}
// vim:fdm=syntax
