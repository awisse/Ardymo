// Print value of trigonometric functions
//
#include <iostream>
#include <cmath>
#include "unistd.h"
#include "../../rotate.h"
#include "../../objects.h"

enum args {
  TRIANGLE,
  VECTORS
};

class Pt {

  public:

    float x, y;

    Pt() : x(0.0), y(0.0) {}
    Pt(float x, float y) : x(x), y(y) {}
    Pt(point p) : x(p.x), y(p.y) {}

    Pt operator+(const Pt& other) {
      Pt newpt = other;
      newpt.x += this->x;
      newpt.y += this->y;
      return newpt;
    }

    Pt operator-(const Pt& other) {
      Pt newpt = other;
      newpt.x -= this->x;
      newpt.y -= this->y;
      return newpt;
    }

    float len(void) {
      float len = std::sqrt(this->x * this->x + this->y * this->y);
      return len;
    }

    Pt rotate(float angle) {
      Pt rotated = *this;
      rotn trig = get_rotn(angle);
      rotated.x = trig.cos * this->x - trig.sin * this->y;
      rotated.y = trig.sin * this->x + trig.cos * this->y;
      return rotated;
    }
};

void print_help() {
  std::cout << "Usage: vecalg [options] ([x y]+|px py vx vy])\n";
  std::cout << \
    "  -h  Print this help message\n"\
    "  -d  Compute the distance between (px,py) and (vx, vy)\n"\
    "  -l  Compute the hypothenuse of (x,y)\n"\
    "  -a  <angle> The angle for clockwise rotation in degrees\n"\
    "  -t  Print cosinus and sinus of <angle>\n"\
    "  -r  Print the point defined of a clockwise rotation of (vx,vy) "\
    "around (px,py)\n";
  exit(0);
}

void parse_args(char* argv[], Pt* p, Pt* v, args argt) {
  float f;
  char* end;

  switch (argt) {
    case VECTORS:
      p->x = std::strtof(argv[optind++], &end);
      p->y = std::strtof(argv[optind++], &end);
    case TRIANGLE:
      v->x = std::strtof(argv[optind++], &end);
      v->y = std::strtof(argv[optind++], &end);
  }
}

int main(int argc, char* argv[]) {

  int i = 0, angle = 0;
  rotn trig;
  bool show_trig = false, distance = false, rotate = false;
  bool hypothenuse = false;
  int c;
  Pt p, v, w, rotated;
  float s, len;

  if (argc == 1) {
    print_help();
  }

  while ((c = getopt(argc, argv, "a:dhlrt")) != -1) {
    switch (c) {

      case 'h':
        print_help();
        break;

      case 'a':
        angle = std::atoi(optarg);

      case 't':
        show_trig = true;
        break;

      case 'd':
        distance = true;
        break;

      case 'r':
        rotate = true;
        break;

      case 'l': // Pythagoras
        hypothenuse = true;
    }
  }


  if (show_trig) {
    trig = get_rotn((int16_t)angle);
    std::printf("cos(%1$hd) = %2$f\nsin(%1$hd) = %3$f\n",
        angle, trig.cos, trig.sin);
  }

  if (rotate || distance) {
    // There must be four elements left for px, py, vx, vy
    if (argc - optind != 4) {
      std::cerr << "Wrong number of elements px, py, vx, vy\n";
      print_help();
    }

    parse_args(argv, &p, &v, VECTORS);

    // Show result of rotation
    if (rotate) {
      rotated = v.rotate(angle);
      std::printf("p =      (% 10.8f, % 10.8f)\n", p.x, p.y);
      std::printf("v =      (% 10.8f, % 10.8f)\n", v.x, v.y);
      std::printf("p + v =      (% 10.8f, % 10.8f)\n", (p+v).x, (p+v).y);
      std::printf("rot(v) =     (% 10.8f, % 10.8f)\n", rotated.x, rotated.y);
      std::printf("p + rot(v) = (% 10.8f, % 10.8f)\n",
          (p+rotated).x, (p+rotated).y);
    }

    if (distance) {
      s = std::sqrt((p.x - v.x) * (p.x - v.x) + (p.y - v.y) * (p.y - v.y));
      std::printf("∥p - v∥ =  %10.8f\n", s);
    }

  } else if (hypothenuse) {

    while (argc - optind >= 2) {
      parse_args(argv, &p, &v, TRIANGLE);
      len = v.len();
      std::printf("length(p%d) = %10.8f\n", i++, len);
    }
  }


  return 0;
}
