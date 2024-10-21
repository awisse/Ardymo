#include <iostream>
#define PROGMEM
#include <math.h>
#include <gtest/gtest.h>
#include "../objects.h"
#include "../vehicle.h"
#include "../rotate.h"
#include "../intersection.h"

bool eq(Vec v, Vec w) {
  return (abs(v.x - w.x) < epsilon) && (abs(v.y - w.y) < epsilon);
}

obstacle_t mkObst(geometry type, progmem_t data) {

  obstacle_t obstacle;
  obstacle.type = type;
  memcpy(&obstacle.item, &data, sizeof(obstacle.item));
  // Fix alignment problem for line_t
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

  EXPECT_FLOAT_EQ(c_.sq(), 409.0);
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
      // seg5 must be on the sensor ray
      temp = segment.p + segment.v * 2.0;
      seg5 = mkObst(LINE, {temp.x, temp.y, 1.0, -30, 0x01});
      // Segment with circle.
      seg6 = mkObst(LINE, {2.8, 1.73205081, 2, -30, 0x01});
      circ = mkObst(CIRCLE, {3.0, 4.0, 2.0});
      bigcirc = mkObst(CIRCLE, {5.0, 5.0, 3.0});
      rect1 = mkObst(RECTANGLE,{2.5, 7.0, 4.0, 270, 0x3F000000});
      rect2 = mkObst(RECTANGLE,{5.0, 6.0, 4.0, 240, 0x3F000000});
    }

    LineVector sensor;
    LineVector segment;
    obstacle_t seg0;
    obstacle_t seg1;
    obstacle_t seg2;
    obstacle_t seg3;
    obstacle_t seg4;
    obstacle_t seg5;
    obstacle_t seg6;
    obstacle_t circ;
    obstacle_t bigcirc;
    obstacle_t rect1;
    obstacle_t rect2;
};

// Test of line intersections
TEST_F (TestIntersections, Lines) {

  // Sensor with seg1
  EXPECT_EQ(intersects(sensor, seg1), 1);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(5.16743371, 5.83256629));

  EXPECT_EQ(intersects(sensor, seg3), 0);

  EXPECT_EQ(intersects(sensor, seg5), 2);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(3.8, 3.46410162));
  EXPECT_PRED2(eq, intersect_point(1).p, Vec(4.3, 4.33012703));

  EXPECT_EQ(intersects(seg0.item.line, seg1), 2);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(6.0, 5.0));
  EXPECT_PRED2(eq, intersect_point(1).p, Vec(7.0, 4.0));

  EXPECT_EQ(intersects(seg1.item.line, seg2), 0);
  EXPECT_EQ(intersects(seg2.item.line, seg3), 0);

  EXPECT_EQ(intersects(seg1.item.line, seg3), 1);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(5.73205081, 5.26794919));

  EXPECT_EQ(intersects(seg2.item.line, seg4), 2);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(2.375, 3.64951905));
  EXPECT_PRED2(eq, intersect_point(1).p, Vec(3.125, 4.94855716));

  EXPECT_EQ(intersects(seg4.item.line, seg2), 2);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(2.375, 3.64951905));
  EXPECT_PRED2(eq, intersect_point(1).p, Vec(3.125, 4.94855716));
}

// Tests of circle intersections
TEST_F (TestIntersections, Circles) {

  EXPECT_EQ(intersects(sensor, circ), 2);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(4.70910866, 5.03872401));
  EXPECT_PRED2(eq, intersect_point(1).p, Vec(2.95499295, 2.00050647));

  EXPECT_EQ(intersects(seg6.item.line, circ), 1);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(2.95499295, 2.00050647));

  EXPECT_EQ(intersects(seg0.item.line, bigcirc), 1);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(7.56155282, 3.43844718));

  EXPECT_EQ(intersects(seg4.item.line, bigcirc), 0);
}

// Tests of rectangle intersections
TEST_F (TestIntersections, Rectangle) {

  EXPECT_EQ(intersects(sensor, rect1), 2);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(6.5, 8.1406388));
  EXPECT_PRED2(eq, intersect_point(1).p, Vec(5.84145188, 7.0));

  EXPECT_EQ(intersects(sensor, rect2), 2);
  EXPECT_PRED2(eq, intersect_point(0).p, Vec(6.19807621, 7.61769146));
  EXPECT_PRED2(eq, intersect_point(1).p, Vec(5.19807621, 5.88564065));

}

// Tests of collision of Vehicle with obstacle
class TestCollisions : public testing::Test {
  protected:

    void SetUp(void) {
      vehicle = Vehicle(3.0, 6.0, 4.0, 240, 0.5);
      // Segments
      segment[NONE] = mkObst(LINE, {8.8, 3.45, 2.0, 60, 1});
      segment[LEFT] = mkObst(LINE, {4.0, 4.0, 1.4, -45, 1});
      segment[FRONT] = mkObst(LINE, {8.0, 4.1, 2.0, 90, 1});
      segment[RIGHT] = mkObst(LINE, {9.0, 7.74, 2.8, 135, 1});
      segment[REAR] = mkObst(LINE, {2.0, 8.0, 2.0, 240, 1});
      // Circles
      circle[NONE] = mkObst(CIRCLE, {8.0, 4.0, 1.0});
      circle[LEFT] = mkObst(CIRCLE, {3.0, 3.9, 2.0});
      circle[FRONT] = mkObst(CIRCLE, {7.0, 2.25, 2.0});
      circle[RIGHT] = mkObst(CIRCLE, {7.5, 8.0, 2.0});
      circle[REAR] = mkObst(CIRCLE, {4.9, 8.1, 1.0});
      // Rectangles
      rectangle[NONE] = mkObst(RECTANGLE, {2.5, 4.0, 2.0, 0, 0x3F000000});
      rectangle[LEFT] = mkObst(RECTANGLE, {3.0, 3.75, 2.0, -30, 0x3F000000});
      rectangle[FRONT] = mkObst(RECTANGLE, {9.4, 6.0, 2.0, 90, 0x3F000000});
      rectangle[RIGHT] = mkObst(RECTANGLE, {7.2, 8.0, 2.0, 135, 0x3F000000});
      rectangle[REAR] = mkObst(RECTANGLE, {3.2, 8.5, 1.4, 225, 0x3F36DB6E});
    }

    Vehicle vehicle;
    obstacle_t segment[NUM];
    obstacle_t circle[NUM];
    obstacle_t rectangle[NUM];
};

TEST_F (TestCollisions, Segment) {

  EXPECT_EQ(collides(vehicle, segment[NONE]), NONE);
  EXPECT_EQ(collides(vehicle, segment[LEFT]), LEFT);
  EXPECT_EQ(collides(vehicle, segment[FRONT]), FRONT);
  EXPECT_EQ(collides(vehicle, segment[RIGHT]), RIGHT);
  EXPECT_EQ(collides(vehicle, segment[REAR]), REAR);
}

TEST_F (TestCollisions, Circle) {

  EXPECT_EQ(collides(vehicle, circle[NONE]), NONE);
  EXPECT_EQ(collides(vehicle, circle[LEFT]), LEFT);
  EXPECT_EQ(collides(vehicle, circle[FRONT]), FRONT);
  EXPECT_EQ(collides(vehicle, circle[RIGHT]), RIGHT);
  EXPECT_EQ(collides(vehicle, circle[REAR]), REAR);
}

TEST_F (TestCollisions, Rectangle) {

  EXPECT_EQ(collides(vehicle, rectangle[NONE]), NONE);
  EXPECT_EQ(collides(vehicle, rectangle[LEFT]), LEFT);
  EXPECT_EQ(collides(vehicle, rectangle[FRONT]), FRONT);
  EXPECT_EQ(collides(vehicle, rectangle[RIGHT]), RIGHT);
  EXPECT_EQ(collides(vehicle, rectangle[REAR]), REAR);
}

// Tests of distance to intersection point
TEST_F (TestIntersections, Distance) {

  uint8_t n;

  n = intersects(sensor, circ);
  EXPECT_FLOAT_EQ(distance(Vec(sensor.p), n), 2.3099859);
  n = intersects(sensor, rect2);
  EXPECT_FLOAT_EQ(distance(Vec(sensor.p), n), 6.79615243);
}

