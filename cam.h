#ifndef CAM_H_
#define CAM_H_

#include "types.h"
#include "vec.h"
#include "ray.h"

typedef struct {
  v3 up, lookfrom, lookat;
  f32 lens_len;
  f32 aperture; // unused for now
  f32 aspect;
} cam_create_info_t;

typedef struct {
  v3 pos;
  v3 llc; // bottom left of image
  v3 u, v;
  f32 aperture; // unused
} cam_t;

cam_t cam_create(cam_create_info_t cci);
ray_t cam_ray(cam_t cam, f32 u, f32 v);

#endif // CAM_H_
