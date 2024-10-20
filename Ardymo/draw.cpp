// All the drawing takes place here.
#include "defines.h"
#include "globals.h"
#include "objects.h"
#include "draw.h"
#include "platform.h"
#include "shapes.h"
#include "sprites.h"

// Center of screen
static Vec centre = {(float)screenCentre.x, (float)screenCentre.y};

// Helper functions

// Draw elapsed time and distance to target in top left corner
void DrawStatus(float speed, float distance) {
  Platform::eraseRectRow(0, 0, kStatusX - 1, 16);
  Platform::setCursor(1, 1);
  Platform::print(speed, 1);
  Platform::print("/s");
  Platform::setCursor(1, 9);
  Platform::print(distance, 0);
  Platform::print('m');
}

// Redraw compass arrow with new coordinates
void DrawCompass(Vec vehicle_direction, int16_t alpha,
    Vec target_direction) {
  Vec arrow;
  int16_t true_heading;
  int16_t voffset, hoffset; // position of degrees displayed
  char c;

  // Erase first
  Platform::eraseRectRow((kScreenWidth >> 1) - 2 - kCompassRadius, 8,
      kCompassRadius << 1, kCompassRadius << 1);
  // 1. Circle in the centre
  Platform::drawCircle(centre.x, centre.y, kCompassRadius);
  // 2. Arrow
  vehicle_direction.flipX();
  arrow = centre + vehicle_direction * (kCompassRadius - 2);
  Platform::drawLine(centre.x, centre.y, arrow.x, arrow.y);
  // 3. Target direction bullet with respect to north
  arrow = centre + target_direction * (kCompassRadius - 3);
  Platform::fillCircle(arrow.x, arrow.y, 2);
  // 4. Display degrees in centre
  //
#if 0
/* #ifdef _DEBUG */
  Platform::DebugPrint(alpha);
  Platform::DebugPrintln();
#endif
  if (alpha < 0) {
    c = 'E';
    true_heading = 180 + alpha;
  } else {
    c = 'W';
    true_heading = 180 - alpha;
  }
  voffset = (true_heading < 90) ? 2 : -8;
  if (true_heading < 10) {
    hoffset = -2;
  } else if (true_heading< 100) {
    hoffset = -5;
  } else {
    hoffset = -8;
  }
  Platform::setCursor(centre.x + hoffset, centre.y + voffset);
  Platform::print(true_heading);
  Platform::print(c);
}

void DrawDistances(Distances* distances) {
  // Front
  Platform::eraseRectRow(centre.x + 5, 0, 48, 8);
  Platform::setCursor(centre.x + 5, 0);
  Platform::print(distances->front);
  Platform::print('m');

}

void DrawBackground() {
  // Draw main features of background:
  // 1. rectangle in upper left corner.
  Platform::drawFastHLine(0, kStatusY, kStatusX);
  Platform::drawFastVLine(kStatusX, 0, kStatusY);
  // 2. Triangles for directions
  Platform::drawBitmap(centre.x - 2, 0, &arrows[UP_ARROW], 5, 8);
  Platform::drawBitmap(centre.x - 2, kScreenHeight - 5,
      &arrows[DOWN_ARROW], 5, 8);
  Platform::drawBitmap(0, centre.y - 2, &arrows[LEFT_ARROW], 5, 8);
  Platform::drawBitmap(kScreenWidth - 5, centre.y - 2,
      &arrows[RIGHT_ARROW], 5, 8);

}

// vim:fdm=syntax

