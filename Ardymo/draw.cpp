#include "globals.h"
#include "objects.h"
#include "draw.h"
#include "platform.h"
#include "shapes.h"
#include "sprites.h"

static Vec center = {(kScreenWidth >> 1) - 1, (kScreenHeight >> 1) - 1};
// Helper functions
void DrawBackground(void);


// Draw elapsed time and distance to target in top left corner
void DrawScore(void) {
}

// Redraw compass arrow with new coordinates
void DrawCompass(int16_t vehicle_direction, int16_t target_direction) {
  Vec arrow;

  // Erase first
  Platform::eraseRectRow((kScreenWidth >> 1) - 2 - kCompassRadius, 8,
      kCompassRadius << 1, kCompassRadius << 1);
  // 1. Circle in the center
  Platform::drawCircle((kScreenWidth >> 1) - 1, kScreenHeight >> 1, kCompassRadius);
  // 2. Arrow
  arrow = center - Vec(0, kCompassRadius - 2).rotate(-vehicle_direction);
  Platform::drawLine(center.x, center.y, arrow.x, arrow.y);
  // 3. Target direction bullet
  arrow = center - Vec(0, kCompassRadius - 3).rotate(-target_direction);
  Platform::fillCircle(arrow.x, arrow.y, 2);
}

void DrawSensors(void) {
  // Draw main features of background:
  // 2. rectangle in upper left corner.
  Platform::drawFastHLine(0, kStatusY, kStatusX);
  Platform::drawFastVLine(kStatusX, 0, kStatusY);
  // 3. Triangles for directions
  Platform::drawBitmap((kScreenWidth >> 1) - 3, 0, arrows + UP_ARROW, 5, 8);
  Platform::drawBitmap((kScreenWidth >> 1) - 3, kScreenHeight - 5,
      arrows + DOWN_ARROW, 5, 8);
  Platform::drawBitmap(0, (kScreenHeight >> 1) - 3, arrows + LEFT_ARROW, 5, 8);
  Platform::drawBitmap(kScreenWidth - 5, (kScreenHeight >> 1) - 3,
      arrows + RIGHT_ARROW, 5, 8);

}
// vim:fdm=syntax
