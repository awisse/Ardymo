#include <iostream>
#define PROGMEM
#include "../objects.h"
#include "../intersection.h"

void print(const char* name, Vec v) {
  std::cout << name << " = (" << v.x << ", " << v.y << ")\n";
}

void print(const obstacle object) {
  switch (object.type) {
    case LINE:
      print("line p", Vec(object.line.p));
      print("line v", Vec(object.line.v));
      break;
    case CIRCLE:
      print ("circle p", Vec(object.circle.p));
      std::cout << "circle r = " << object.circle.r << "\n";
      break;
    case RECTANGLE:
      print("rectangle p", Vec(object.rectangle.p));
      print("rectangle v", Vec(object.rectangle.v));
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

void test_intersects(void) {
  line_t sensor = {{5.0, 5.0}, {3.0, 4.0}, 0};
  obstacle seg1 = {LINE, {5.0, 6.0, 2.0, -2.0, 1}};
  obstacle seg2 = {LINE, {2.0, 3.0, 1.0, 2.0, 1}};
  obstacle circ = {CIRCLE, {3.0, 4.0, 2.0}};
  obstacle rect1 = {RECTANGLE,{3.0, 7.0, 4.0, 0.0, 0.5}};
  obstacle rect2 = {RECTANGLE,{5.0, 6.0, 3.4841, -2.0, 0.5}};
  uint8_t i;

  std::cout << "Sensor: Intersection and Distance\n";
  print("p", sensor.p);
  print("v", sensor.v);
  print(seg1);
  uint8_t n = intersects(sensor, seg1);
  std::cout << "sensor with seg1: " << (int)n << "\n";
  if (n) {
    print("seg1: X[0]", intersect_point(0));
  }

  print(seg2);
  n = intersects(sensor, seg2);
  std::cout << "sensor with seg2: " << (int)n << "\n";
  if (n) {
    print("seg2: X[0]", intersect_point(0));
  }

  print(circ);
  n = intersects(sensor, circ);
  std::cout << "sensor with circ: " << (int)n << "\n";
  for (i=0; i<n; i++) {
    std::printf("circ: X[%hhu]", i);
    print("", intersect_point(i));
  }

  print(rect1);
  n = intersects(sensor, rect1);
  std::cout << "sensor with rect1: " << (int)n << "\n";
  for (i=0; i<n; i++) {
    std::printf("rect1: X[%hhu]", i);
    print("", intersect_point(i));
  }

  print(rect2);
  n = intersects(sensor, rect2);
  std::cout << "sensor with rect2: " << (int)n << "\n";
  for (i=0; i<n; i++) {
    std::printf("rect2: X[%hhu]", i);
    print("", intersect_point(i));
  }
}


int main() {

  test_Vec();
  test_intersects();

  return 0;

}

