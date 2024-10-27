// Print value of trigonometric functions
//
#include <iostream>
#include <cmath>
#include "unistd.h"
#include "../../rotate.h"
#include "../../objects.h"

enum args {
  TWO,
  FOUR
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
      newpt.x -= x;
      newpt.y -= y;
      return newpt;
    }

    float len(void) {
      float len = std::sqrt(x * x + y * y);
      return len;
    }

    Pt rotate(float phi) {
      Pt rotated = *this;
      rotn trig = get_rotn(phi);
      rotated.x = trig.cos * x - trig.sin * y;
      rotated.y = trig.sin * x + trig.cos * y;
      return rotated;
    }

    float angle(void) {
      // Get the angle of the vector, clockwise rotation
      float a;
      if ((x == 0) && (y == 0))
        return INFINITY;
      if ((x == 0) && (y > 0))
        return 0;
      if ((x == 0) && (y < 0))
        return 180;

      // Now that we have the edge cases, get the inverse tangent
      a = std::atan2(-x, y);
      return a / M_PI * 180;

    }
};

void print_help() {
  std::cout << "Usage: vecalg [options] ([x y]+|px py vx vy])\n";
  std::cout << \
    "  -h  Print this help message\n"\
    "  -d  Compute the distance between (px,py) and (vx, vy)\n"\
    "  -l  Compute the hypothenuse of (x,y)\n"\
    "  -p  <phi> The angle for clockwise rotation in degrees\n"\
    "  -t  Print cosinus and sinus of <phi>\n"\
    "  -a  Print the angle of (vx,vy) rotated around (px, py) in degrees\n"\
    "  -r  Print the point defined of a clockwise rotation of (vx,vy) "\
    "around (px,py)\n";
  exit(0);
}

void parse_args(char* argv[], Pt* p, Pt* v, args argt) {
  float f;
  char* end;

  switch (argt) {
    case FOUR:
      p->x = std::strtof(argv[optind++], &end);
      p->y = std::strtof(argv[optind++], &end);
    case TWO:
      v->x = std::strtof(argv[optind++], &end);
      v->y = std::strtof(argv[optind++], &end);
  }
}

int main(int argc, char* argv[]) {

  int i = 0, phi = 0;
  rotn trig;
  bool show_trig = false, distance = false, rotate = false;
  bool get_angle = false;
  bool hypothenuse = false;
  int c;
  Pt p, v, w, rotated;
  float s, len, angle;

  if (argc == 1) {
    print_help();
  }

  while ((c = getopt(argc, argv, "p:adhlrt")) != -1) {
    switch (c) {

      case 'h':
        print_help();
        break;

      case 'p':
        phi = std::atoi(optarg);

      case 't':
        show_trig = true;
        break;

      case 'd':
        distance = true;
        break;

      case 'r':
        rotate = true;
        break;

      case 'a':
        get_angle = true;
        break;

      case 'l': // Pythagoras
        hypothenuse = true;
    }
  }


  if (show_trig) {
    trig = get_rotn((int16_t)phi);
    std::printf("cos(%1$hd) = %2$f\nsin(%1$hd) = %3$f\n",
        phi, trig.cos, trig.sin);
  }

  if (rotate || distance) {
    // There must be four elements left for px, py, vx, vy
    if (argc - optind != 4) {
      std::cerr << "Wrong number of elements px, py, vx, vy\n";
      print_help();
    }

    parse_args(argv, &p, &v, FOUR);

    // Show result of rotation
    if (rotate) {
      rotated = v.rotate(phi);
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

  } else if (hypothenuse || get_angle) {

    while (argc - optind >= 2) {
      parse_args(argv, &p, &v, TWO);
      if (hypothenuse) {
        len = v.len();
        std::printf("length(p%d) = %10.8f\n", i++, len);
      }
      if (get_angle) {
        angle = v.angle();
        std::printf("angle(p%d)  = %10.8f\n", i++, angle);
      }

    }
  }


  return 0;
}
