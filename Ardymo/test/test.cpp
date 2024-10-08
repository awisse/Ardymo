#include <iostream>
#define PROGMEM
#include "../objects.h"
#include "../intersection.h"

void print(const char* name, Vec v) {
  std::cout << ((name==NULL) ? "" : name) <<\
                      " (" << v.x << ", " << v.y << ")\n";
}

void print(const char* name, const obstacle object) {
  if (name) std::cout << name << ": ";
  switch (object.type) {
    case LINE:
      std::cout << (object.line.seg ? "segment" : "line") << "\n";
      print("p", Vec(object.line.p));
      print("v", Vec(object.line.v));
      break;
    case CIRCLE:
      std::cout << "circle\n";
      print ("p", Vec(object.circle.p));
      std::cout << "r = " << object.circle.r << "\n";
      break;
    case RECTANGLE:
      std::cout << "rectangle\n";
      print("p", Vec(object.rectangle.p));
      print("v", Vec(object.rectangle.v));
      break;
    default:
      std::cout << "Not Implemented\n";
  }
}

void test_Vec() {

  Vec a(39.0, 233.0), b(3.0, 4.0), c(-20.0, 3.0);
  point p = {10.0, 15.001};
  float v;
  Vec d(p);

  std::cout << "Tests of Vec class" << "\n";
  std::cout << "==================" << "\n";

  print("a", a);
  print("b", b);
  print("c", c);
  print("d", d);
  print("a+b", a+b);
  a += b;
  std::cout << "a+=b: ";
  print("a", a);
  print("-c", -c);

  print("a+(-c)", a+(-c));
  print("a-c", a-c);

  d = a + b + c;
  std::cout << "d = a+b+c: ";
  print("d", d);
  print("a * 3", a * 3);
  std::cout << "b.sq() = " << b.sq() << "\n";

  std::cout << "b * c = " << b * c << "\n";
  std::cout << "a * b = " << a * b << "\n";

  d = b.rotate(130);
  std::cout << "d = rotate(b, 130). ";
  print("d", d);
  std::cout << "d.sq() = " << d.sq() << "\n";

  std::cout << "a==b = " << (a==b ? "true" : "false") << "\n";
  std::cout << "b==(3, 4) = " << (b==Vec(3, 4) ? "true" : "false")\
    << "\n";
  // As point
  p = d.as_point();
  std::cout << "p=d.as_point(). p.x = " << p.x << ", p.y = " << p.y\
    << "\n";

  std::cout << "\n";
}

void print_intersection(uint8_t n) {
  uint8_t i;
  for (i=0; i<n; i++) {
    std::printf("X[%hhu] = ", i);
    print("", intersect_point(i));
  }
}
void test_intersects(void) {
  line_t sensor = {5.0, 5.0, 3.0, 4.0, 0};
  obstacle seg0 = {LINE, .line={6.0, 5.0, 2.0, -2.0, 1}};
  obstacle seg1 = {LINE, .line={5.0, 6.0, 2.0, -2.0, 1}};
  obstacle seg2 = {LINE, .line={2.0, 3.0, 1.0, 2.0, 1}};
  obstacle seg3 = {LINE, .line={5.0, 4.0, 1.0, 2.0, 1}};
  obstacle seg4 = {LINE, .line={2.25, 3.5, 0.5, 1.0, 1}};
  obstacle seg5 = {LINE, .line={2.0, 1.0, 1.5, 2.0, 1}};
  obstacle circ = {CIRCLE, {3.0, 4.0, 2.0}};
  obstacle rect1 = {RECTANGLE,.rectangle={3.0, 7.0, 4.0, 0.0, 0.5}};
  obstacle rect2 = {RECTANGLE,.rectangle={5.0, 6.0, 3.4841, -2.0, 0.5}};

  std::cout << "\nIntersection and Distance\nsensor:\n";
  print("p", sensor.p);
  print("v", sensor.v);

  std::cout << "Lines and Segments\n";
  uint8_t n;
  n = intersects(sensor, seg1);
  std::cout << "============\nsensor with seg1: " << (int)n << "\n";
  print_intersection(n);
  print("seg1", seg1);

  n = intersects(sensor, seg2);
  std::cout << "============\nsensor with seg2: " << (int)n << "\n";
  print_intersection(n);
  print("seg2", seg2);

  n = intersects(sensor, seg3);
  std::cout << "============\nsensor with seg3: " << (int)n << "\n";
  print_intersection(n);
  print("seg3", seg3);

  n = intersects(sensor, seg5);
  std::cout << "============\nsensor with seg5: " << (int)n << "\n";
  print_intersection(n);
  print("seg5", seg5);

  n = intersects(seg0.line, seg1);
  std::cout << "============\nseg0 with seg1: " << (int)n << "\n";
  print_intersection(n);
  print("seg0", seg0);

  n = intersects(seg1.line, seg2);
  std::cout << "============\nseg1 with seg2: " << (int)n << "\n";
  print_intersection(n);

  n = intersects(seg2.line, seg3);
  std::cout << "============\nseg2 with seg3: " << (int)n << "\n";
  print_intersection(n);

  n = intersects(seg1.line, seg3);
  std::cout << "============\nseg1 with seg3: " << (int)n << "\n";
  print_intersection(n);

  n = intersects(seg2.line, seg4);
  std::cout << "============\nseg2 with seg4: " << (int)n << "\n";
  print_intersection(n);
  print("seg4", seg4);

  n = intersects(seg4.line, seg2);
  std::cout << "============\nseg4 with seg2: " << (int)n << "\n";
  print_intersection(n);

  n = intersects(sensor, circ);
  std::cout << "============\nsensor with circ: " << (int)n << "\n";
  print_intersection(n);
  print("circ", circ);

  n = intersects(sensor, rect1);
  std::cout << "============\nsensor with rect1: " << (int)n << "\n";
  print_intersection(n);
  print("rect1", rect1);

  n = intersects(sensor, rect2);
  std::cout << "============\nsensor with rect2: " << (int)n << "\n";
  print_intersection(n);
}


int main() {

  test_Vec();
  test_intersects();

  return 0;

}

