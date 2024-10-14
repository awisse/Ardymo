#include "globals.h"
#include "draw.h"
#include "platform.h"
#include "shapes.h"
#include "sprites.h"

// Helper functions
void DrawBackground(void);


// Redraw all
void DrawScreen(void) {
  DrawBackground();
}

// Draw elapsed time and distance to target in top left corner
void DrawScore(void) {
}

// Redraw compass arrow with new coordinates
void DrawCompass(void) {
}

void DrawBackground(void) {
  // Draw main features of background:
  // 1. Circle in the center
  // 2. rectangle in upper left corner.
  // 3. Triangles for directions
  // corner.
  Platform::drawBitmap((kScreenWidth >> 1) - 3, 0, arrows + UP_ARROW, 5, 8);
  Platform::drawBitmap((kScreenWidth >> 1) - 3, kScreenHeight - 5,
      arrows + DOWN_ARROW, 5, 8);
  Platform::drawBitmap(0, (kScreenHeight >> 1) - 3, arrows + LEFT_ARROW, 5, 8);
  Platform::drawBitmap(kScreenWidth - 5, (kScreenHeight >> 1) - 3,
      arrows + RIGHT_ARROW, 5, 8);

}
// vim:fdm=syntax
