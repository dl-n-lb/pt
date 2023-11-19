#include "ray.h"

v3 ray_at(ray_t r, f32 t) {
  return v3_add(r.o, v3_muls(r.d, t));
}
