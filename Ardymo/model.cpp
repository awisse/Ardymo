// vim: ts=2:sts=2:expandtab

#include "model.h"
#include "platform.h"
#include "rotate.h"
#include "structs.h"
#include "shapes.h"
#include "defines.h"

void initialize() {}

void step_model(uint16_t frame) {

  static uint16_t alpha = 0;
  Vec start(-30, 0), end(30, 0), offset(64, 32);
  Vec rotated, start_rotated, end_rotated;
  int i;

  /* Platform::pollButtons(); */
  if (Platform::pressed(INPUT_RIGHT)) {
    alpha = alpha ? alpha - 3 : 357;
  } else if (Platform::pressed(INPUT_LEFT)) {
    alpha = (alpha + 3) % 360;
  } else if (rotated.x) { // Run at least once
    return;
  }

  Platform::clear();

  Platform::setCursor(0, 0);
  Platform::print("alpha: ");
  Platform::print(alpha);

  start_rotated = start.rotate(alpha);
  start_rotated = offset + start_rotated;
  end_rotated = end.rotate(alpha);
  end_rotated = offset + end_rotated;
  Platform::drawLine(start_rotated.x, start_rotated.y,
      end_rotated.x, end_rotated.y, COLOUR_WHITE);
}

