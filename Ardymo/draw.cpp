// All the drawing takes place here.
#include "string.h"
#include "defines.h"
#include "objects.h"
#include "draw.h"
#include "platform.h"
#include "sprites.h"

// Centre of screen
static Vec centre = {(float)((kScreenWidth>>1)-1),
                     (float)((kScreenHeight>>1)-1)};

// Helper functions
void drawSeconds(uint16_t elapsed, int16_t x, int16_t y);
void eraseCompass(void);

void drawStartup() {
  // For now ...
  Platform::clear();
  // Draw a few rectangles
  // Top Left → Down → Right
  Platform::drawRect(0, 0, 22, 20);  // Left Top
  Platform::drawRect(0, 22, 14, 24); // Left second from top
  Platform::drawRect(0, 48, 14, 16); // Left Bottom
  Platform::drawRect(16, 22, 20, 18);
  Platform::drawRect(16, 42, 20, 22);
  Platform::drawRect(24, 0, 12, 20); // Top second from left
  // Middle bottom
  Platform::drawRect(38, 56, 20, 8);
  Platform::drawRect(60, 56, 30, 8);
  // Top
  Platform::drawRect(38, 0, 36, 8);
  Platform::drawRect(76, 0, 36, 8);
  // Right
  Platform::drawRect(92, 10, 10, 36);
  Platform::drawRect(92, 48, 18, 16);
  Platform::drawRect(104, 10, 8, 12);
  Platform::drawRect(104, 24, 14, 22);
  // Top right
  Platform::drawRect(114, 0, 14, 22);
  // Middle right
  Platform::drawRect(120, 24, 8, 22);
  // Bottom right
  Platform::drawRect(112, 48, 16, 16);
}

void drawBackground() {
  Platform::clear();
  // Draw main features of background:
  // 1a. rectangle in upper left corner.
  Platform::drawFastHLine(0, kStatusY, kStatusX);
  Platform::drawFastVLine(kStatusX, 0, kStatusY);
  // 1b. rectangle in lower left corner.
  Platform::drawFastHLine(0, kScreenHeight-kStatusY-1, kStatusX);
  Platform::drawFastVLine(kStatusX, kScreenHeight-kStatusY-1, kStatusY);
  // 2. Triangles for directions
  Platform::drawBitmap(centre.x - 2, 0, &arrows[UP_ARROW], 5, 8);
  Platform::drawBitmap(centre.x - 2, kScreenHeight - 5,
      &arrows[DOWN_ARROW], 5, 8);
  Platform::drawBitmap(0, centre.y - 2, &arrows[LEFT_ARROW], 5, 8);
  Platform::drawBitmap(kScreenWidth - 5, centre.y - 2,
      &arrows[RIGHT_ARROW], 5, 8);

}

// Draw elapsed time and distance to target in top left corner
void drawStatus(float speed, float distance) {
  Platform::eraseRectRow(0, 0, kStatusX - 1, 15);
  Platform::setCursor(1, 1);
  Platform::print(speed, 1);
  Platform::print("/s");
  Platform::setCursor(1, 9);
  Platform::print(distance, 0);
  Platform::print('m');
}

// Redraw compass with new coordinates
void drawCompass(Vec vehicle_direction, int16_t alpha, Vec target_direction) {
  Vec arrow;
  int16_t true_heading;
  int16_t voffset, hoffset; // position of degrees displayed
  char c;

  // Erase first
  eraseCompass();
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

void drawI2C(bool useI2C) {
  uint8_t icon_pos = useI2C ? I2C_ON : I2C_OFF;
  // Draw I2C indicator in top right corner
  Platform::eraseRectRow(kScreenWidth - 5, 0, 5, 8);
  Platform::drawBitmap(kScreenWidth - 5, 0, &i2c_indicator[icon_pos], 5, 8);
}

void drawDistances(Distances* distances) {
  int16_t hoffset; // For right distance
  // Front
  Platform::eraseRectRow(centre.x + 6, 0, 48, 1);
  Platform::setCursor(centre.x + 6, 0);
  Platform::print(distances->front, 0);
  Platform::print('m');
  // Rear
  Platform::eraseRectRow(centre.x + 6, kScreenHeight - 7, 30, 1);
  Platform::setCursor(centre.x + 6, kScreenHeight - 7);
  Platform::print(distances->rear, 0);
  Platform::print('m');
  // Left
  Platform::eraseRectRow(6, centre.y - 4, 30, 8);
  Platform::setCursor(7, centre.y - 4);
  Platform::print(distances->left, 0);
  Platform::print('m');
  // Right
  Platform::eraseRectRow(kScreenWidth - 39, centre.y - 4, 34, 8);
  // Fix alignment depending on number
  if (distances->right < 9.5)
    hoffset = 39 - 3 * 6;
  else if (distances->right < 99.5)
    hoffset = 39 - 2 * 6;
  else if (distances->right < 999.5)
    hoffset = 39 - 6;
  else
    hoffset = 39;
  Platform::setCursor(kScreenWidth - hoffset, centre.y - 4);
  Platform::print(distances->right, 0);
  Platform::print('m');

}

void drawPosition(Vec vehicle_position) {
  // Bottom left corner
  Platform::eraseRectRow(0, kScreenHeight - 16, kStatusX - 1, kStatusX - 1);
  Platform::setCursor(1, kScreenHeight - kStatusY + 2);
  Platform::print("x ");
  Platform::print((int16_t)vehicle_position.x);
  Platform::setCursor(1, kScreenHeight - kStatusY + 10);
  Platform::print("y ");
  Platform::print((int16_t)vehicle_position.y);
}

// A simple success screen.
void drawSuccess(uint16_t elapsed) {
  Platform::clear();
  Platform::setCursor(centre.x - 45, centre.y - 8);
  Platform::print("*** SUCCESS ***");
  drawSeconds(elapsed, centre.x - 36, centre.y);
}

// A simple crash screen.
void drawCrash(Vec* position) {
  Platform::clear();
  Platform::setCursor(centre.x - 39, centre.y - 8);
  Platform::print("----CRASH----");
  Platform::setCursor(centre.x - 33, centre.y - 0);
  Platform::print("(");
  Platform::print(position->x, 0);
  Platform::print(",");
  Platform::print(position->y, 0);
  Platform::print(")");
}

// A simple game over screen.
void drawGameOver(uint16_t elapsed) {
  Platform::clear();
  Platform::setCursor(centre.x - 48, centre.y - 9);
  Platform::print("Target Destroyed");
  drawSeconds(elapsed, centre.x - 36, centre.y);
}

// Draw a message to the user in the center of the screen.
// Info, Warning, or Alert.
void drawMessage(const char* msg) {
  uint8_t msg_len = strlen(msg) * 6; // 6 pixels per 5x7 character
  uint8_t x = (kScreenWidth - msg_len) / 2;

  Platform::eraseRectRow(x, 24, msg_len + 4, 15);
  Platform::drawRect(x, 24, msg_len + 4, 15);
  Platform::setCursor(x + 3, 28);
  Platform::print(msg);
}

// Draw menu in the middle
void drawMenu(uint8_t selected, uint8_t numItems, const char* items[]) {
  // level is the top item in the menu.
  // For now, there is just one more item
  uint8_t i;

  Platform::eraseRectRow(kMenuLeft, kMenuTop, kMenuWidth, numItems * 10);

  // Font height is 8. Rectangles must be 10.
  for (i=0; i<numItems; i++) {
    Platform::setCursor(kMenuLeft + 2, kMenuTop + 2 + i * 10);
    Platform::print_P(items[i]);
  }
  // Rectangle around selected item
  Platform::drawRect(kMenuLeft, kMenuTop + 10 * selected, kMenuWidth, 11);
}

// Draw Help Screen.
void drawHelp() {
  Platform::clear();
  Platform::setCursor(0, 0);
  Platform::print("Help!!!");
}

// For end of game screens.
void drawSeconds(uint16_t elapsed, int16_t x, int16_t y) {
  Platform::setCursor(x, y);
  Platform::print(elapsed);
  Platform::print(" seconds");
}

void eraseCompass(void) {
  Platform::eraseRectRow((kScreenWidth >> 1) - 2 - kCompassRadius, 8,
      (kCompassRadius << 1) + 4, (kCompassRadius << 1)  - 1);
  // A short line at the top to erase, too
  Platform::drawFastHLine((kScreenWidth >> 1) - 5, 7, 8, COLOUR_BLACK);
}

// vim:fdm=syntax
