#ifndef VEC_H_
#define VEC_H_

#include "types.h"

// print formatting
#define V3_fs "{%f %f %f}"
#define V3_f(v) (v).x, (v).y, (v).z 

typedef union {
  struct {
    f32 x, y, z;
  };
  struct {
    f32 u, v, w;
  };
  f32 e[3];
} v3;

// TODO: TEST THESE FUNCTIONS LOL
v3 v3_add(v3 a, v3 b);
v3 v3_sub(v3 a, v3 b);
v3 v3_mul(v3 a, v3 b);

v3 v3_muls(v3 a, f32 t);
v3 v3_divs(v3 a, f32 t);

v3 v3_cross(v3 a, v3 b);
f32 v3_dot(v3 a, v3 b);

f32 v3_lensq(v3 a);
f32 v3_len(v3 a);
v3 v3_norm(v3 a);

v3 rand_unit_v3(void);

#endif // VEC_H_
