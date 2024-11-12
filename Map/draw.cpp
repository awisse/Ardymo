// All the drawing takes place here.
#include "src/defines.h"
#include "src/platform.h"
#include "src/objects.h"
#include "viewport.h"
#include "src/shapes.h"
#include "src/sprites.h"
#include "src/utils.h"
#include "draw.h"
#include "help.h"

// Center of screen
static Vec centre = {(float)((kScreenWidth>>1)-1),
                     (float)((kScreenHeight>>1)-1)};
// ViewPort

// Helper functions
void drawLine(const line_t* line);
void drawCircle(const circle_t* circle);
void drawRectangle(const rectangle_t* rect);

void draw(vehicle_t* vehicle) {
  // Draw the viewport. The vehicle is treated as the argument.
  obstacle_t obst;
  uint8_t i;
  uint16_t n = obst_count[vehicle->level];

  // Start with the vehicle
  drawRectangle(&vehicle->rect);

  // All obstacles
  for (i=0; i<n; i++) {
    get_obstacle(&obst, vehicle->level, i);
    switch (obst.type) {
      case LINE:
        drawLine(&obst.item.line);
        break;
      case TARGET:
      case CIRCLE:
        drawCircle(&obst.item.circle);
        break;
      case RECTANGLE:
        drawRectangle(&obst.item.rectangle);
        break;
    }
  }
}

void drawPosition(point* vehicle_position) {

  Platform::eraseRectRow(0, kScreenHeight - 16, kStatusX - 1, 15);
  Platform::setCursor(1, kScreenHeight - kStatusY + 2);
  Platform::print("x ");
  Platform::print((int16_t)vehicle_position->x);
  Platform::setCursor(1, kScreenHeight - kStatusY + 10);
  Platform::print("y ");
  Platform::print((int16_t)vehicle_position->y);
}

// Helper functions
void drawLine(const line_t* line) {
  LinePoints lpts;
  // Find the line segments which are actually in the viewport
  if (getLine(line, &lpts)) {
    Platform::drawLine(lpts.l0.x, lpts.l0.y, lpts.l1.x, lpts.l1.y);
  }
}

void drawCircle(const circle_t* circle) {
  // draw the bloody circle as is
  ScreenCircle screen_circle;
  if (getCircle(circle, &screen_circle)) {
    Platform::drawCircle(screen_circle.p.x, screen_circle.p.y, screen_circle.r);
  }
}

void drawRectangle(const rectangle_t* rect) {
  // draw the (up to) four line segments
  LinePoints segments[4];
  uint8_t i, n;

  n = getRectangle(rect, segments);
  for (i=0; i<n; i++) {
    Platform::drawLine(segments[i].l0.x, segments[i].l0.y,
        segments[i].l1.x, segments[i].l1.y);
  }
}

// draw menu in the middle
void drawMenu(uint8_t selected, uint8_t numItems, const char* item[]) {
  // level is the top item in the menu.
  // For now, there is just one more item
  uint8_t i;

  /* Platform::eraseRectRow(kMenuLeft, kMenuTop, kMenuWidth, numItems * 10); */
  Platform::clear();

  // Font height is 8. Rectangles must be 10.
  for (i=0; i<numItems; i++) {
    Platform::setCursor(kMenuLeft + 2, kMenuTop + 2 + i * 10);
    Platform::print(item[i]);
  }
  // Rectangle around selected item
  Platform::drawRect(kMenuLeft, kMenuTop + 10 * selected,
      kMenuWidth + 11, 11);
}

// draw Help Screen.
void drawHelp(uint8_t page) {
  uint8_t titleLength;
  uint8_t charPos {0}; // Position in helpText
  uint8_t line {0};
  Platform::clear();

  // Check page
  if (page == 1) {
    while ((uint8_t)pgm_read_byte(&helpText[charPos++]) != 0xFF);
  }

  titleLength = charPos;
  // Center first line
  // Find length of title
  while ((uint8_t)pgm_read_byte(&helpText[titleLength++]));
  titleLength = (titleLength - charPos)
    * Platform::getFullCharWidth(); // In pixels
  Platform::setCursor((kScreenWidth - titleLength) / 2, 0);

  while (line < 8) { // 8 lines on screen
    charPos += Platform::print_P((char*)&helpText[charPos]);
    line++;
    Platform::setCursor(16, line * Platform::getLineHeight());
  }

  // Draw Arrows
  // Left Arrow
  Platform::drawBitmap(0, centre.y - 2, &arrows[LEFT_ARROW], 5, 8);
  Platform::drawBitmap(kScreenWidth - 5, centre.y - 2, &arrows[RIGHT_ARROW],
      5, 8);

}
// vim:fdm=syntax
