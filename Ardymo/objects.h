/* vim: ft=cpp:ts=2:sts=2:expandtab
 *
 * Struct and classes used in program
 */
#pragma once
#include <stdint.h>

// We need the `point` type because the Arduino compiler doesn't accept the
// `Vec` class in PROGMEM: "a variable with dynamic initialization
// cannot be put in program memory area".
constexpr float epsilon = 5E-6; // Test for zero with floats

typedef struct {
  float x, y;
} point;

class Vec {
  // A "Vector" with a few useful operators

  public:
    // I leave these public: less hassle
    float x, y;

    Vec() : x(0), y(0) {}
    Vec(float x, float y) : x(x), y(y) {}
    Vec(const Vec& p) : x(p.x), y(p.y) {}
    Vec(const point p) : x(p.x), y(p.y) {}

    Vec operator+=(const Vec&);
    Vec operator+(const Vec&);
    Vec operator+(const point p);
    Vec operator-(void); // Inverse sign
    Vec operator-(const Vec&);
    Vec operator*(const float a); // Scalar product
    Vec operator/(const float a); // Division by scalar
    float operator*(const Vec&); // Inner product
    bool operator==(const Vec&);

    float div(const Vec&); // Division of collinear vectors.
    float det(const Vec&); // Determinant of the matrix (this, other)
    float sq(void);     // ∥v∥^2
    float length(void); // ∥v∥
    float distance(const Vec&); // ∥v-other∥
    Vec normalized(void); // Vector normalized to ∥v∥=1

    void flipX(void); // Flip along x-axis in place
    Vec rotate(const int16_t alpha); // Rotate clockwise by alpha

    point as_point(void) const; // Transform into a struct `point`
};

