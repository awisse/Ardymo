// Print value of trigonometric functions
//
#include <iostream>
#include "unistd.h"
#include "../../rotate.h"
#include "../../objects.h"

rotn get_trig(int16_t angle) {

  while (angle < 0) {
    angle += 360;
  }

  while (angle >= 360) {
    angle -= 360;
  }

  return get_rotn((int16_t)angle);
}

void rotation(point* rotated, point* v, uint16_t angle) {
  // Rotate v clockwise around p by angle.
  rotn trig = get_trig(angle);
  rotated->x = trig.cos * v->x - trig.sin * v->y; 
  rotated->y = trig.sin * v->x + trig.cos * v->y; 
}

void print_help() {
  std::cout << "Usage: trig [options] [px py vx vy]\n";
  exit(0);
}

void parse_vectors(char* argv[], point* p, point* v) {
  float f;
  char* end;

  p->x = std::strtof(argv[optind++], &end);
  p->y = std::strtof(argv[optind++], &end);
  v->x = std::strtof(argv[optind++], &end);
  v->y = std::strtof(argv[optind++], &end);
}

int main(int argc, char* argv[]) {

  int angle = 0;
  rotn trig;
  bool show_trig = false, rotate = false;
  int c;
  point p, v, rotated;

  if (argc == 1) {
    print_help();
  }

  while ((c = getopt(argc, argv, "hta:r")) != -1) {
    switch (c) {

      case 'h':
        print_help();
        break;

      case 'a':
        angle = std::atoi(optarg);

      case 't':
        show_trig = true;
        break;

      case 'r':
        rotate = true;
        break;
    }
  }


  if (show_trig) {
    trig = get_trig((int16_t)angle);
    std::printf("cos(%1$hd) = %2$f\nsin(%1$hd) = %3$f\n", 
        angle, trig.cos, trig.sin);
  }

  if (rotate) {
    // There must be four elements left for px, py, vx, vy
    if (argc - optind != 4) {
      std::cerr << "Wrong number of elements px, py, vx, vy\n";
      print_help();
    }
    parse_vectors(argv, &p, &v);
    rotation(&rotated, &v, angle);

    std::printf("p + v =      (% 10.8f, % 10.8f)\n", p.x + v.x, p.y + v.y);
    std::printf("rot(v) =     (% 10.8f, % 10.8f)\n", rotated.x, rotated.y);
    std::printf("p + rot(v) = (% 10.8f, % 10.8f)\n", p.x + rotated.x, 
        p.y + rotated.y);
  }

  return 0;
}
