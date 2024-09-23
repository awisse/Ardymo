/* vim: ft=cpp:ts=2:sts=2:expandtab
 *
 * Struct and classes used in program
 */
#include <stdint.h>

class Pt {

  public:
    // I leave these public: less hassle
    int16_t x, y;

    Pt() : x(0), y(0) {}
    Pt(int16_t x, int16_t y) : x(x), y(y) {}
    Pt(const Pt& p) : x(p.x), y(p.y) {}

    Pt operator+=(const Pt&);
    Pt operator+(const Pt&);
    Pt operator-(void);
    Pt operator-(const Pt&);
    Pt operator*(float a);
    bool operator==(const Pt&);
};

typedef struct {
  int16_t x, y;
} point;

