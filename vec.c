#include "vec.h"
#include <math.h>

// TODO: TEST THESE FUNCTIONS LOL
v3 v3_add(v3 a, v3 b) {
  return (v3) { a.x + b.x, a.y + b.y, a.z + b.z };
}

v3 v3_sub(v3 a, v3 b) {
  return (v3) { a.x - b.x, a.y - b.y, a.z - b.z };
}

v3 v3_muls(v3 a, f32 t) {
  return (v3) { a.x * t, a.y * t, a.z * t };
}

v3 v3_divs(v3 a, f32 t) {
  return (v3) { a.x / t, a.y / t, a.z / t };
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
