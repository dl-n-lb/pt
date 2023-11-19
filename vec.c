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

v3 rand_unit_v3(void) {
  // simplest (wrong) method is uniform sample + normalize
  // simplest (maybe correct method) is to pick angles then work out the vector
  // google told me to use a gaussian distribution then normalize
  // this is just ripped off of raytracing.github.io because im lazy
  v3 res = {0};
  f32 theta = (f32)rand() / RAND_MAX * 2 * M_PI;
  f32 rho = (f32)rand() / RAND_MAX * 2 * M_PI;
  res.x = cosf(theta) * 2 * sqrt(rho * (1-rho));
  res.y = sinf(theta) * 2 * sqrt(rho * (1-rho));
  res.z = 1 - 2 * rho;

  return res;
}
