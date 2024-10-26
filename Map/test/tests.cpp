#include <iostream>
#define PROGMEM
#include <math.h>
#include <gtest/gtest.h>
#include "../viewport.h"
#include "../intersection.h"

bool eqv(Vec v, Vec w) {
  return (abs(v.x - w.x) < epsilon) && (abs(v.y - w.y) < epsilon);
}

// Equality of two rectangle_t
bool eqr(rectangle_t r0, rectangle_t r) {
  return (r0.p.x == r.p.x) && (r0.p.y == r.p.y) && (r0.l == r.l) 
    && (r0.rho == r.rho) && (r0.w == r.w);
}

// Equality of two ScreenPt
bool eqsp(ScreenPt p0, ScreenPt p) {
  return (p0.x == p.x) && (p0.y == p.y);
}

// Equality of two lines given by LinePoints
bool eqlp(LinePoints lp0, LinePoints lp1) {

  bool equal = (eqsp(lp0.l0, lp1.l0) && eqsp(lp1.l1, lp1.l1))
    || (eqsp(lp0.l1, lp1.l0) || eqsp(lp0.l0, lp1.l1));

  return equal;
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

ScreenPt to_screen(const point p, const ViewPort* v) {
  ScreenPt sp;
  const rectangle_t r = v->get_rectangle();
  sp = ScreenPt((p.x - r.p.x) * v->get_scale(), 
     (p.y - r.p.y) * v->get_scale());

  return sp;
}

// Tests for Vec class
class ViewportTest : public testing::Test {
  protected:
    void SetUp() override {

      // Middle
      rect_middle = {kBoardWidth / 2.0, kBoardHeight / 2.0, 
        kScreenWidth, -90, kScreenHeight};
      // Bottom-Left
      rect_bl = {0.0, kBoardHeight - kScreenHeight, 
        kScreenWidth, -90, kScreenHeight};
      // Top-Right
      rect_tr = {kBoardWidth - kScreenWidth, 0.0, 
        kScreenWidth, -90, kScreenHeight};
      // Bottom-Right
      rect_br = {kBoardWidth - kScreenWidth, kBoardHeight - kScreenHeight,
        kScreenWidth, -90, kScreenHeight};
      // Top-Left is given by default_view;
    }

    rectangle_t rect_middle, rect_bl, rect_tr, rect_br;
};

TEST_F (ViewportTest, Movements) {

  rectangle_t r_middle = rect_middle;
  rectangle_t r_bl = rect_bl;
  rectangle_t r_tr = rect_tr;
  ViewPort v;

  v = ViewPort(default_map, default_view);
  EXPECT_PRED2(eqr, v.get_rectangle(), default_view);
  v = ViewPort(default_map, rect_middle);
  EXPECT_PRED2(eqr, v.get_rectangle(), rect_middle);
  v = ViewPort(default_map, rect_bl);
  EXPECT_PRED2(eqr, v.get_rectangle(), rect_bl);
  v = ViewPort(default_map, rect_tr);
  EXPECT_PRED2(eqr, v.get_rectangle(), rect_tr);
  v = ViewPort(default_map, rect_br);
  EXPECT_PRED2(eqr, v.get_rectangle(), rect_br);

  // Shouldn't move

  v = ViewPort(default_map, rect_bl);
  v.pan_left();
  EXPECT_PRED2(eqr, v.get_rectangle(), rect_bl);
  v.pan_down();
  EXPECT_PRED2(eqr, v.get_rectangle(), rect_bl);

  v = ViewPort(default_map, rect_tr);
  v.pan_right();
  EXPECT_PRED2(eqr, v.get_rectangle(), rect_tr);
  v.pan_up();
  EXPECT_PRED2(eqr, v.get_rectangle(), rect_tr);

  // Move 1/2 dimension
  //
  v = ViewPort(default_map, rect_middle);
  v.pan_right();
  r_middle.p.x += r_middle.l / 2;
  EXPECT_PRED2(eqr, v.get_rectangle(), r_middle);

  v.pan_down();
  r_middle.p.y += r_middle.w / 2;
  EXPECT_PRED2(eqr, v.get_rectangle(), r_middle);

  v = ViewPort(default_map, rect_bl);
  v.pan_up();
  r_bl.p.y -= r_bl.w / 2;
  EXPECT_PRED2(eqr, v.get_rectangle(), r_bl);

  v = ViewPort(default_map, rect_tr);
  v.pan_left();
  r_tr.p.x -= r_tr.l / 2;
  EXPECT_PRED2(eqr, v.get_rectangle(), r_tr);

}

TEST_F (ViewportTest, Centering) {
  // ReCenter
  rectangle_t r_middle = rect_middle;
  ViewPort v;

  v = ViewPort(default_map, r_middle);
  v.re_center({kBoardWidth / 2, kBoardHeight / 2});
  r_middle.p.x = (kBoardWidth - r_middle.l) / 2;
  r_middle.p.y = (kBoardHeight - r_middle.w) / 2;
  EXPECT_PRED2(eqr, v.get_rectangle(), r_middle);
}

TEST_F (ViewportTest, Scaling) {
  rectangle_t r;
  ViewPort v;

  r = default_view;
  v = ViewPort(default_map, r);
  v.zoom_out();
  r.l *= 2;
  r.w *= 2;
  EXPECT_PRED2(eqr, v.get_rectangle(), r);

  r = rect_br;
  v = ViewPort(default_map, r);
  v.zoom_out();
  r.p.x = r.p.x - r.l;
  r.p.y = r.p.y - r.w;
  r.l *= 2;
  r.w *= 2;
  EXPECT_PRED2(eqr, v.get_rectangle(), r);

  r = rect_tr;
  v = ViewPort(default_map, r);
  v.zoom_out();
  r.p.x = r.p.x - r.l;
  r.l *= 2;
  r.w *= 2;
  EXPECT_PRED2(eqr, v.get_rectangle(), r);
  
  r = rect_bl;
  v = ViewPort(default_map, r);
  v.zoom_out();
  r.p.y = r.p.y - r.w;
  r.l *= 2;
  r.w *= 2;
  EXPECT_PRED2(eqr, v.get_rectangle(), r);
  EXPECT_EQ(0.5, v.get_scale());

  r = rect_middle;
  v = ViewPort(default_map, r);
  r = v.get_rectangle();
  v.zoom_in();
  EXPECT_PRED2(eqr, v.get_rectangle(), r);

  r = rect_middle;
  v = ViewPort(default_map, r);
  v.zoom_out();
  EXPECT_EQ(0.5, v.get_scale());
  v.zoom_out();
  EXPECT_EQ(0.25, v.get_scale());
  v.zoom_out();
  EXPECT_EQ(0.125, v.get_scale());
  v.zoom_out();
  EXPECT_EQ(1.0 / 16.0, v.get_scale());
  EXPECT_PRED2(eqr, v.get_rectangle(), default_map);
}

TEST_F (ViewportTest, Map2Screen) {
  point p;
  ScreenPt sp;
  ViewPort v;

  // Test whether map points are correctly translated into 
  // Screen Points (can be outside of screen)
  // 1. Reduce rect_middle by a factor of four.
  v = ViewPort(default_map, rect_middle);
  
  // Define four points in the center, four quadrants and corners of 
  // rect_middle for a total of 9 points.
  // Corners
  // Top Left
  p = rect_middle.p;
  sp = v.Map2Screen(p);
  EXPECT_EQ(sp.x, 0);
  EXPECT_EQ(sp.y, 0);
  // Top Right
  p.x += rect_middle.l;
  sp = v.Map2Screen(p);
  EXPECT_EQ(sp.x, kScreenWidth);
  EXPECT_EQ(sp.y, 0);
  // Bottom Right
  p.y += rect_middle.w;
  sp = v.Map2Screen(p);
  EXPECT_EQ(sp.x, kScreenWidth);
  EXPECT_EQ(sp.y, kScreenHeight);
  // Bottom Left
  p.x = rect_middle.p.x;
  sp = v.Map2Screen(p);
  EXPECT_EQ(sp.x, 0);
  EXPECT_EQ(sp.y, kScreenHeight);
  // Center
  p.x += rect_middle.l / 2.0;
  p.y = rect_middle.p.y + rect_middle.w / 2.0;
  sp = v.Map2Screen(p);
  EXPECT_EQ(sp.x, kScreenWidth / 2);
  EXPECT_EQ(sp.y, kScreenHeight / 2);
  // Upper left
  p.x -= rect_middle.l / 4.0;
  p.y -= rect_middle.w / 4.0;
  sp = v.Map2Screen(p);
  EXPECT_EQ(sp.x, kScreenWidth / 4);
  EXPECT_EQ(sp.y, kScreenHeight / 4);
  // Upper right
  p.x += rect_middle.l / 2.0;
  sp = v.Map2Screen(p);
  EXPECT_EQ(sp.x, 3 * kScreenWidth / 4);
  EXPECT_EQ(sp.y, kScreenHeight / 4);
  // Lower right
  p.y += rect_middle.w / 2.0;
  sp = v.Map2Screen(p);
  EXPECT_EQ(sp.x, 3 * kScreenWidth / 4);
  EXPECT_EQ(sp.y, 3 * kScreenHeight / 4);
  // Lower left
  p.x = rect_middle.p.x + rect_middle.l / 4.0;
  sp = v.Map2Screen(p);
  EXPECT_EQ(sp.x, kScreenWidth / 4);
  EXPECT_EQ(sp.y, 3 * kScreenHeight / 4);

  // Outside of screen
  // Top Left
  p = {0, 0};
  sp = v.Map2Screen(p);
  EXPECT_PRED2(eqsp, v.Map2Screen(p), to_screen(p, &v));
  // Top Right
  p = {kBoardWidth, 0};
  sp = v.Map2Screen(p);
  EXPECT_PRED2(eqsp, v.Map2Screen(p), to_screen(p, &v));
  // Bottom Right
  p = {kBoardWidth, kBoardHeight};
  sp = v.Map2Screen(p);
  EXPECT_PRED2(eqsp, v.Map2Screen(p), to_screen(p, &v));
  // Bottom Left
  p = {0, kBoardHeight};
  sp = v.Map2Screen(p);
  EXPECT_PRED2(eqsp, v.Map2Screen(p), to_screen(p, &v));
  // Center
  p = {kBoardWidth / 2.0, kBoardHeight / 2.0};
  sp = v.Map2Screen(p);
  EXPECT_EQ(sp.x, 0);
  EXPECT_EQ(sp.y, 0);
}

class GetFigure : public testing::Test {

  protected:
    void SetUp() override {
      // Based on inherited rect_middle
      l0 = {300.0, 300.0, 300.0, -30, 1};
      l1 = {kBoardWidth / 2 + 16, 412.0, 150.0, 0, 1};
      l2 = {kBoardWidth / 2 + 64, kBoardHeight / 2 - 32, 150.0, 45, 1};
      lc = {kBoardWidth / 2 - 32, kBoardHeight / 2 - 32, 80.0, -45, 1}; 
      p0 = {kBoardWidth / 2, kBoardHeight / 2};
    }

    line_t l0, l1, l2, lc, l_inner;
    circle_t c0, c1, c2, c_inner;
    point p0; // Origin of test viewport
};

TEST_F (GetFigure, GetLine) {
  ViewPort viewport;
  InitViewport(&p0);
  GetViewport(&viewport);
  LinePoints lp0, lp1;
  intersection_t ix[2];
  ScreenPt s_ix[2]; // 
  uint8_t n;

  n = GetLine(&l0, &lp0);
  EXPECT_EQ(n, 0);

  // One Intersection
  n = GetLine(&l1, &lp0);
  EXPECT_EQ(n, 1);
  ix[0] = intersect_point(0);
  s_ix[0] = to_screen(ix[0].p.as_point(), &viewport);
  lp1 = LinePoints(s_ix[0], to_screen({l1.p.x, l1.p.y + l1.l}, &viewport));
  EXPECT_PRED2(eqlp, lp0, lp1);
  
  // Two Intersections
  n = GetLine(&l2, &lp0);
  EXPECT_EQ(n, 1);
  ix[0] = intersect_point(0);
  ix[1] = intersect_point(1);
  s_ix[0] = ScreenPt(0, 32);
  s_ix[1] = ScreenPt(32, 0);
  /* s_ix[0] = to_screen(ix[0].p.as_point(), &viewport); */
  /* s_ix[1] = to_screen(ix[1].p.as_point(), &viewport); */
  lp1 = LinePoints(s_ix[0], s_ix[1]);
  EXPECT_PRED2(eqlp, lp0, lp1);

  // Two intersections, one corner
  n = GetLine(&lc, &lp0);
  EXPECT_EQ(n, 1);
  s_ix[0] = ScreenPt(0, 0);
  s_ix[1] = ScreenPt(32, 32);
  /* s_ix[0] = to_screen(ix[0].p.as_point(), &viewport); */
  lp1 = LinePoints(s_ix[0], s_ix[1]);
  EXPECT_PRED2(eqlp, lp0, lp1);
}