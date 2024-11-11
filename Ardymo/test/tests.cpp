#include <iostream>
#define PROGMEM
#include <math.h>
#include <gtest/gtest.h>
#include "../objects.h"
#include "../vehicle.h"
#include "../rotate.h"
#include "../intersection.h"

typedef enum {
    FORWARD_LEFT,
    FORWARD_RIGHT,
    RIGHT_FRONT,
    RIGHT_REAR,
    REARWARD_RIGHT,
    REARWARD_LEFT,
    LEFT_REAR,
    LEFT_FRONT
} ray_t;

// Predicates
// Equality of Vec objects
bool eq(Vec v, Vec w) {
  return (abs(v.x - w.x) < epsilon) && (abs(v.y - w.y) < epsilon);
}
// Collision
testing::AssertionResult AssertCollision(
    const char* s_sensor,
    const char* s_obst,
    const char* s_ray,
    const LineVector sensor,
    const obstacle_t obst,
    const int i) {
  uint8_t intersections = intersects(&sensor, &obst);
  uint8_t n;
  intersection_t ix;

  if (intersections == 0) {
    return testing::AssertionFailure() << "No intersection between ray "
      << s_sensor << "[" << i << "] and " << s_obst;
  }

  for (n=0; n<intersections; n++) {
    ix = intersect_point(n);
    if ((ix.nu < 0.0) && (ix.nu > -1.0)) {
      return testing::AssertionSuccess();
    }
  }

  return testing::AssertionFailure() <<
    "Intersection but no collision between ray " << s_sensor
    << "[" << i << "] and obstacle " << s_obst;
}

testing::AssertionResult AssertNoCollision(
    const char* s_sensor,
    const char* s_obst,
    const char* ray,
    const LineVector sensor,
    const obstacle_t obst,
    const int i) {
  uint8_t intersections = intersects(&sensor, &obst);
  uint8_t n;
  intersection_t ix;

  if (intersections == 0) {
    return testing::AssertionSuccess();
  }

  for (n=0; n<intersections; n++) {
    ix = intersect_point(n);
    if ((ix.nu < 0.0) && (ix.nu > -1.0)) {
      return testing::AssertionFailure() <<
        "Collision between ray " << s_sensor << " and " << s_obst;
    }
  }

  return testing::AssertionSuccess();
}

obstacle_t mkObst(geometry type, progmem_t data) {

  obstacle_t obstacle;
  obstacle.type = type;
  memcpy(&obstacle.item, &data, sizeof(obstacle.item));
  // Fix alignment problem on x86_64 for line_t
  memcpy(&obstacle.item.line.seg, &(data.bytes),
      sizeof(obstacle.item.line.seg));

  return obstacle;
}

// Check the values of the sines and cosines in rotate.h
TEST (OmegaTest, Values) {
  int16_t i;
  for (i=0; i<360; i++) {
    EXPECT_NEAR(get_rotn(i).cos, cosf(i / 180.0 * M_PI), epsilon);
    EXPECT_NEAR(get_rotn(i).sin, sinf(i / 180.0 * M_PI), epsilon);
  }
}

// Tests for Vec class
class VecTest : public testing::Test {
  protected:
    void SetUp() override {

      // class Vec
      a_ = Vec(39.0, 233.0);
      b_ = Vec(3.0, 4.0);
      c_ = Vec(-20.0, 3.0);

      // struct point
      p_ = {10.0, 15.001};
    }

    Vec a_, b_, c_, d_;
    point p_;

};

TEST_F (VecTest, Operators) {
  // Test all the operators of the Vec class
  EXPECT_PRED2(eq, (a_ + b_), Vec(42.0, 237.0));
  a_ += b_;
  EXPECT_PRED2(eq, a_, Vec(42.0, 237.0));
  EXPECT_PRED2(eq, -c_, Vec(20.0, -3.0));
  EXPECT_PRED2(eq, a_ - c_, Vec(62.0, 234.0));
  d_ = Vec(p_);
  EXPECT_PRED2(eq, d_, Vec(10.0, 15.001));
  EXPECT_PRED2(eq, a_ + b_ + c_ + d_, Vec(35.0, 259.001));
  EXPECT_PRED2(eq, a_ * 3.0, Vec(126.0, 711.0));
  EXPECT_FLOAT_EQ(b_ * c_, -48.0);
  EXPECT_FLOAT_EQ(b_ * a_, 1074.0);
}

TEST_F (VecTest, Functions) {
  // Test all member functions of the Vec class
  EXPECT_TRUE( isnan(a_.div(b_))); // Divison of non collinear vectors
  EXPECT_FLOAT_EQ(Vec(20.0, 30.0).div(Vec(10.0, 15.0000001)), 2.0);

  EXPECT_FLOAT_EQ(c_.square(), 409.0);
  EXPECT_FLOAT_EQ(b_.length(), 5.0);

  EXPECT_PRED2(eq, b_.rotate(130), Vec(-4.99254036, -0.27301693));
  EXPECT_FLOAT_EQ(b_.det(c_), 89.0); // Determinant
  EXPECT_FLOAT_EQ(b_.distance(c_), 23.021728866);
}

// Test values for intersections
class TestIntersections : public testing::Test {

  protected:

    void SetUp(void) override {
      Vec temp; // Auxiliary variable
                //
      segment = LineVector(Vec(1.8, 0.0), 2.0, -30, 1);
      sensor = segment;
      sensor.seg = 0;   // Unbound line
      seg0 = mkObst(LINE, {6.0, 5.0, 2 * M_SQRT2, 225, 0x01});
      seg1 = mkObst(LINE, {5.0, 6.0, 2 * M_SQRT2, 225, 0x01});
      seg2 = mkObst(LINE, {2.0, 3.0, 3.0, 330, 0x01});
      seg3 = mkObst(LINE, {5.0, 4.0, 2.0, 330, 0x01});
      seg4 = mkObst(LINE, {2.375, 3.64951905, 1.5, 330, 0x01});
      // Special case: seg7 intersects v1 and v3 of the rectangle
      seg7 = mkObst(LINE, {0.0, 8.0, 9.0, -90, 0x01});
      // seg5 must be on the sensor ray
      temp = segment.p + segment.v * 2.0;
      seg5 = mkObst(LINE, {temp.x, temp.y, 1.0, -30, 0x01});
      // Segment with circle.
      seg6 = mkObst(LINE, {2.8, 1.73205081, 2, -30, 0x01});
      circ = mkObst(CIRCLE, {3.0, 4.0, 2.0});
      bigcirc = mkObst(CIRCLE, {5.0, 5.0, 3.0});
      rect1 = mkObst(RECTANGLE,{2.5, 7.0, 4.0, 270, 0x40000000});
      rect2 = mkObst(RECTANGLE,{5.0, 6.0, 4.0, 240, 0x40000000});
      // All Segments as LineVectors
      seg0_lv = LineVector(seg0.item.line);
      seg1_lv = LineVector(seg1.item.line);
      seg2_lv = LineVector(seg2.item.line);
      seg3_lv = LineVector(seg3.item.line);
      seg4_lv = LineVector(seg4.item.line);
      seg7_lv = LineVector(seg7.item.line);
      seg5_lv = LineVector(seg5.item.line);
      seg6_lv = LineVector(seg6.item.line);
    }

    LineVector sensor;
    LineVector segment;
    obstacle_t seg0;
    obstacle_t seg1;
    obstacle_t seg2;
    obstacle_t seg3;
    obstacle_t seg4;
    obstacle_t seg7;
    obstacle_t seg5;
    obstacle_t seg6;
    LineVector seg0_lv;
    LineVector seg1_lv;
    LineVector seg2_lv;
    LineVector seg3_lv;
    LineVector seg4_lv;
    LineVector seg7_lv;
    LineVector seg5_lv;
    LineVector seg6_lv;
    obstacle_t circ;
    obstacle_t bigcirc;
    obstacle_t rect1;
    obstacle_t rect2;
};

// Test of line intersections
TEST_F (TestIntersections, Lines) {

  // Sensor with seg1
  EXPECT_EQ(intersects(&sensor, &seg1), 1);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(5.16743371, 5.83256629));

  EXPECT_EQ(intersects(&sensor, &seg3), 0);

  EXPECT_EQ(intersects(&sensor, &seg5), 2);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(3.8, 3.46410162));
  EXPECT_PRED2(eq, intersect_point(1).p, Vec(4.3, 4.33012703));

  EXPECT_EQ(intersects(&seg0_lv, &seg1), 2);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(6.0, 5.0));
  EXPECT_PRED2(eq, intersect_point(1).p, Vec(7.0, 4.0));

  EXPECT_EQ(intersects(&seg1_lv, &seg2), 0);
  EXPECT_EQ(intersects(&seg2_lv, &seg3), 0);

  EXPECT_EQ(intersects(&seg1_lv, &seg3), 1);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(5.73205081, 5.26794919));

  EXPECT_EQ(intersects(&seg2_lv, &seg4), 2);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(2.375, 3.64951905));
  EXPECT_PRED2(eq, intersect_point(1).p, Vec(3.125, 4.94855716));

  EXPECT_EQ(intersects(&seg4_lv, &seg2), 2);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(2.375, 3.64951905));
  EXPECT_PRED2(eq, intersect_point(1).p, Vec(3.125, 4.94855716));
}

// Tests of circle intersections
TEST_F (TestIntersections, Circles) {

  EXPECT_EQ(intersects(&sensor, &circ), 2);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(4.70910866, 5.03872401));
  EXPECT_PRED2(eq, intersect_point(1).p, Vec(2.95499295, 2.00050647));

  EXPECT_EQ(intersects(&seg6_lv, &circ), 1);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(2.95499295, 2.00050647));

  EXPECT_EQ(intersects(&seg0_lv, &bigcirc), 1);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(7.56155282, 3.43844718));

  EXPECT_EQ(intersects(&seg4_lv, &bigcirc), 0);
}

// Tests of rectangle intersections
TEST_F (TestIntersections, Rectangle) {

  EXPECT_EQ(intersects(&sensor, &rect1), 2);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(6.5, 8.1406388));
  EXPECT_PRED2(eq, intersect_point(1).p, Vec(5.84145188, 7.0));

  EXPECT_EQ(intersects(&sensor, &rect2), 2);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(6.19807621, 7.61769146));
  EXPECT_PRED2(eq, intersect_point(1).p, Vec(5.19807621, 5.88564065));

  EXPECT_EQ(intersects(&seg7_lv, &rect1), 2);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(2.5, 8.0));
  EXPECT_PRED2(eq, intersect_point(1).p, Vec(6.5, 8.0));
}

// Tests of collision of Vehicle with obstacle
class TestCollisions : public testing::Test {
  protected:

    void SetUp(void) {
      vehicle = Vehicle({3.0, 6.0, 4.0, 240, 2.0});
      // Define the 8 vehicle sensor rays
      rays[FORWARD_LEFT] = LineVector(vehicle.p() + vehicle.v(),
          vehicle.v(), 0);
      rays[FORWARD_RIGHT] = LineVector(vehicle.p() + vehicle.v()
          + vehicle.front(), vehicle.v(), 0);
      rays[RIGHT_FRONT] = LineVector(vehicle.p() + vehicle.v()
          + vehicle.front(), vehicle.front(), 0);
      rays[RIGHT_REAR] = LineVector(vehicle.p() + vehicle.front(),
          vehicle.front(), 0);
      rays[REARWARD_RIGHT] = LineVector(vehicle.p() + vehicle.front(),
          -vehicle.v(), 0);
      rays[REARWARD_LEFT] = LineVector(vehicle.p(), -vehicle.v(), 0);
      rays[LEFT_REAR] = LineVector(vehicle.p(), -vehicle.front(), 0);
      rays[LEFT_FRONT] = LineVector(vehicle.p() + vehicle.v(),
          -vehicle.front(), 0);
      // Segments
      segment[NONE] = mkObst(LINE, {8.8, 3.45, 2.0, 60, 1});
      segment[LEFT] = mkObst(LINE, {4.0, 4.0, 1.4, -45, 1});
      segment[FRONT] = mkObst(LINE, {8.0, 4.1, 2.0, 90, 1});
      segment[RIGHT] = mkObst(LINE, {8.0, 7.74, 2.8, 135, 1});
      segment[REAR] = mkObst(LINE, {2.0, 8.0, 2.0, 240, 1});
      // Circles
      circle[NONE] = mkObst(CIRCLE, {8.0, 4.0, 1.0});
      circle[LEFT] = mkObst(CIRCLE, {3.0, 3.9, 2.0});
      circle[FRONT] = mkObst(CIRCLE, {7.0, 2.25, 2.0});
      circle[RIGHT] = mkObst(CIRCLE, {7.5, 8.0, 2.0});
      circle[REAR] = mkObst(CIRCLE, {4.9, 8.1, 1.0});
      // Rectangles
      rectangle[NONE] = mkObst(RECTANGLE, {2.5, 4.0, 2.0, 0, 0x3F800000});
      rectangle[LEFT] = mkObst(RECTANGLE, {3.0, 3.75, 2.0, -30, 0x3F800000});
      rectangle[FRONT] = mkObst(RECTANGLE, {9.4, 6.0, 2.0, 90, 0x3F800000});
      rectangle[RIGHT] = mkObst(RECTANGLE, {7.2, 8.0, 2.0, 135, 0x3F800000});
      rectangle[REAR] = mkObst(RECTANGLE, {3.2, 8.5, 1.4, 225, 0x3F800000});
    }

    Vehicle vehicle;
    // 8 sensor rays
    LineVector rays[8];

    obstacle_t segment[NUM];
    obstacle_t circle[NUM];
    obstacle_t rectangle[NUM];
};

TEST_F (TestCollisions, SegmentNONE) {

  uint8_t i;
  LineVector ray;

  for (i=0; i<8; i++) {
    ray = rays[i];
    EXPECT_PRED_FORMAT3(AssertNoCollision, ray, segment[NONE], i);
  }
}

TEST_F (TestCollisions, SegmentLEFT) {
  uint8_t i;
  LineVector ray;

  for (i=0; i<8; i++) {
    ray = rays[i];
    if ((i == FORWARD_LEFT) || (i == REARWARD_LEFT)) {
      EXPECT_PRED_FORMAT3(AssertCollision, ray, segment[LEFT], i);
    } else {
      EXPECT_PRED_FORMAT3(AssertNoCollision, ray, segment[LEFT], i);
    }
  }
}

TEST_F (TestCollisions, SegmentFRONT) {
  uint8_t i;
  LineVector ray;

  for (i=0; i<8; i++) {
    ray = rays[i];
    if ((i == FORWARD_LEFT) || (i == REARWARD_LEFT) ||
        (i == LEFT_FRONT) || (i == RIGHT_FRONT)) {
      EXPECT_PRED_FORMAT3(AssertCollision, ray, segment[FRONT], i);
    } else {
      EXPECT_PRED_FORMAT3(AssertNoCollision, ray, segment[FRONT], i);
    }
  }
}

TEST_F (TestCollisions, SegmentRIGHT) {
  uint8_t i;
  LineVector ray;

  for (i=0; i<8; i++) {
    ray = rays[i];
    if ((i == FORWARD_RIGHT) || (i == REARWARD_RIGHT)) {
      EXPECT_PRED_FORMAT3(AssertCollision, ray, segment[RIGHT], i);
    } else {
      EXPECT_PRED_FORMAT3(AssertNoCollision, ray, segment[RIGHT], i);
    }
  }
}

TEST_F (TestCollisions, SegmentREAR) {
  int i;
  LineVector ray;

  for (i=0; i<8; i++) {
    ray = rays[i];
    if ((i == LEFT_REAR) || (i == RIGHT_REAR)) {
      EXPECT_PRED_FORMAT3(AssertCollision, ray, segment[REAR], i);
    } else {
      EXPECT_PRED_FORMAT3(AssertNoCollision, ray, segment[REAR], i);
    }
  }
}

TEST_F (TestCollisions, CircleNONE) {

  int i;
  LineVector ray;

  for (i=0; i<8; i++) {
    ray = rays[i];
    EXPECT_PRED_FORMAT3(AssertNoCollision, ray, circle[NONE], i);
  }
}

TEST_F (TestCollisions, CircleLEFT) {
  uint8_t i;
  LineVector ray;

  for (i=0; i<8; i++) {
    ray = rays[i];
    if ((i == FORWARD_LEFT) || (i == REARWARD_LEFT)) {
      EXPECT_PRED_FORMAT3(AssertCollision, ray, circle[LEFT], i);
    } else {
      EXPECT_PRED_FORMAT3(AssertNoCollision, ray, circle[LEFT], i);
    }
  }
}

TEST_F (TestCollisions, CircleREAR) { uint8_t i;
  LineVector ray;

  for (i=0; i<8; i++) {
    ray = rays[i];
    if ((i == FORWARD_RIGHT) || (i == REARWARD_RIGHT) ||
        (i == LEFT_REAR) || (i == RIGHT_REAR)) {
      EXPECT_PRED_FORMAT3(AssertCollision, ray, circle[REAR], i);
    } else {
      EXPECT_PRED_FORMAT3(AssertNoCollision, ray, circle[REAR], i);
    }
  }
}

TEST_F (TestCollisions, CircleRIGHT) {
  uint8_t i;
  LineVector ray;

  for (i=0; i<8; i++) {
    ray = rays[i];
    if ((i == FORWARD_RIGHT) || (i == REARWARD_RIGHT)) {
      EXPECT_PRED_FORMAT3(AssertCollision, ray, circle[RIGHT], i);
    } else {
      EXPECT_PRED_FORMAT3(AssertNoCollision, ray, circle[RIGHT], i);
    }
  }
}

TEST_F (TestCollisions, RectangleNONE) {

  int i;
  LineVector ray;

  for (i=0; i<8; i++) {
    ray = rays[i];
    EXPECT_PRED_FORMAT3(AssertNoCollision, ray, rectangle[NONE], i);
  }
}

TEST_F (TestCollisions, RectangleLEFT) {
  uint8_t i;
  LineVector ray;

  for (i=0; i<8; i++) {
    ray = rays[i];
    if ((i == FORWARD_LEFT) || (i == REARWARD_LEFT)) {
      EXPECT_PRED_FORMAT3(AssertCollision, ray, rectangle[LEFT], i);
    } else {
      EXPECT_PRED_FORMAT3(AssertNoCollision, ray, rectangle[LEFT], i);
    }
  }
}

TEST_F (TestCollisions, RectangleFRONT) { uint8_t i;
  LineVector ray;

  for (i=0; i<8; i++) {
    ray = rays[i];
    if ((i == FORWARD_RIGHT) || (i == REARWARD_RIGHT) ||
        (i == LEFT_FRONT) || (i == RIGHT_FRONT)) {
      EXPECT_PRED_FORMAT3(AssertCollision, ray, rectangle[FRONT], i);
    } else {
      EXPECT_PRED_FORMAT3(AssertNoCollision, ray, rectangle[FRONT], i);
    }
  }
}

TEST_F (TestCollisions, RectangleRIGHT) {
  uint8_t i;
  LineVector ray;

  for (i=0; i<8; i++) {
    ray = rays[i];
    if ((i == FORWARD_RIGHT) || (i == REARWARD_RIGHT)) {
      EXPECT_PRED_FORMAT3(AssertCollision, ray, rectangle[RIGHT], i);
    } else {
      EXPECT_PRED_FORMAT3(AssertNoCollision, ray, rectangle[RIGHT], i);
    }
  }
}

TEST_F (TestCollisions, RectangleREAR) { uint8_t i;
  LineVector ray;

  for (i=0; i<8; i++) {
    ray = rays[i];
    if ((i == FORWARD_RIGHT) || (i == REARWARD_RIGHT) ||
        (i == LEFT_REAR) || (i == RIGHT_REAR)) {
      EXPECT_PRED_FORMAT3(AssertCollision, ray, rectangle[REAR], i);
    } else {
      EXPECT_PRED_FORMAT3(AssertNoCollision, ray, rectangle[REAR], i);
    }
  }
}

// Tests of distance to intersection point
// Add tests for computation of distance with the same sign
TEST_F (TestIntersections, Distance) {

  uint8_t n;

  n = intersects(&sensor, &circ);
  EXPECT_FLOAT_EQ(distance(Vec(sensor.p), 0), 5.81821733);
  EXPECT_FLOAT_EQ(distance(Vec(sensor.p), 1), 2.3099859);
  n = intersects(&sensor, &rect2);
  EXPECT_FLOAT_EQ(distance(Vec(sensor.p), 0), 8.79615243);
  EXPECT_FLOAT_EQ(distance(Vec(sensor.p), 1), 6.79615243);
  // Only two intersection points exist: 0 and 1. NaN is returned as
  // a distance to an intersection point that doesn't exist.
  EXPECT_TRUE(isnan(distance(Vec(sensor.p), 2)));
  EXPECT_TRUE(isnan(distance(Vec(sensor.p), 20)));
}

