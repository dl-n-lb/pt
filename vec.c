#include "vec.h"
#include <stdlib.h>
#include <math.h>

// TODO: TEST THESE FUNCTIONS LOL
v3 v3_add(v3 a, v3 b) {
  return (v3) {{ a.x + b.x, a.y + b.y, a.z + b.z }};
}

v3 v3_sub(v3 a, v3 b) {
  return (v3) {{ a.x - b.x, a.y - b.y, a.z - b.z }};
}

v3 v3_mul(v3 a, v3 b) {
  return (v3) {{ a.x * b.x, a.y * b.y, a.z * b.z }};
}

v3 v3_muls(v3 a, f32 t) {
  return (v3) {{ a.x * t, a.y * t, a.z * t }};
}

v3 v3_divs(v3 a, f32 t) {
  return (v3) {{ a.x / t, a.y / t, a.z / t }};
}

v3 v3_cross(v3 a, v3 b) {
  return (v3) {
    .x = a.y * b.z - a.z * b.y,
    .y = a.z * b.x - a.x * b.z,
    .z = a.x * b.y - a.y * b.x
  };
}

f32 v3_dot(v3 a, v3 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

f32 v3_lensq(v3 a) {
  return v3_dot(a, a);
}

f32 v3_len(v3 a) {
  return sqrtf(v3_lensq(a));
}

v3 v3_norm(v3 a) {
  return v3_divs(a, v3_len(a));
}

// (should be) a uniformly randomly distributed vector
// forgot that X, Y werent necessarily normalised (oops)
/*v3 rand_unit_v3(void) {
  float U = (float)random() / RAND_MAX;
  float V = (float)random() / RAND_MAX;

  float X = sqrtf(-2 * logf(U)) * cosf(2 * M_PI * V);
  float Y = sqrtf(-2 * logf(U)) * sinf(2 * M_PI * V);
  float Z = 1 - X*X - Y*Y;
  return (v3) {{X, Y, Z}};
  }*/

v3 rand_unit_v3(void) {
  float theta = (float)random() / RAND_MAX * 2 * M_PI;
  float z = ((float)random() / RAND_MAX) * 2 - 1;
  return (v3) {{
    sqrtf(1 - z*z) * cos(theta),
    sqrtf(1 - z*z) * sin(theta),
    z
    }};
}
