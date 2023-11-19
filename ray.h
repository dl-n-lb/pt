#ifndef RAY_H_
#define RAY_H_

#include "types.h"
#include "vec.h"

// assumes the direction is normalized
typedef struct {
  v3 o, d;
} ray_t;

// assumes the direction is normalized
v3 ray_at(ray_t r, f32 t);

#endif // RAY_H_
