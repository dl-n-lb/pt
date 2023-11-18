#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define CLAP_IMPLEMENTATION
#include "clap.h"

#include "types.h"
#include "vec.h"
#include "image.h"

// assumes the direction is normalized
typedef struct {
  v3 o, d;
} ray_t;

// assumes the direction is normalized
v3 ray_at(ray_t r, f32 t) {
  return v3_add(r.o, v3_muls(r.d, t));
}

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

cam_t cam_create(cam_create_info_t cci) {
  cam_t res;
  res.pos = cci.lookfrom;
  res.aperture = cci.aperture;
  // generate an onb
  v3 w = v3_norm(v3_sub(cci.lookat, cci.lookfrom)); // forward vector
  res.u = v3_cross(cci.up, w); // right
  res.v = v3_cross(w, res.u); // up
  res.u = v3_muls(res.u, cci.aspect); // right scaled for aspect ratio

  // temps
  v3 hu = v3_divs(res.u, 2);
  v3 hv = v3_divs(res.v, 2);
  v3 censor_centre = v3_add(res.pos, v3_muls(w, cci.lens_len));
  
  res.llc = v3_sub(censor_centre, v3_add(hu, hv)); // lower left corner
  return res;
}

ray_t cam_ray(cam_t cam, f32 u, f32 v) {
  return (ray_t) {
    .o = cam.pos,
    .d = v3_norm(v3_add(cam.llc, v3_add(v3_muls(cam.u, u), v3_muls(cam.v, v))))
  };
}

int main(int argc, const char **argv) {
  const char *outfile_path = "out.ppm";

  float3_image_t render = float3_image_new(100, 100);

  cam_t cam = cam_create((cam_create_info_t) {
      .up = (v3) {0, 1, 0},
      .lookfrom = (v3) {0, 0, -1},
      .lookat = (v3) {0},
      .lens_len = 1.f,
      .aperture = 1.f,
      .aspect = 1.f,
    });

  for (size_t i = 0; i < 100; ++i) {
    for (size_t j = 0; j < 100; ++j) {
      v3 d = cam_ray(cam, j/100.f, i/100.f).d;
      render.data[(100 * i + j)*3] =   fabs(d.x);
      render.data[(100 * i + j)*3+1] = fabs(d.y);
      render.data[(100 * i + j)*3+2] = fabs(d.z);
    }
  }
  
  char_image_t image = float3_to_char_image(render);
  float3_image_free(render);

  FILE *out_file = fopen(outfile_path, "w");
  if (out_file == NULL) {
    fprintf(stderr, "ERROR: Failed to open file at %s\n", outfile_path);
    return 1;
  }
  save_as_ppm(out_file, image);
  fclose(out_file);

  free(image.data);
  
  return 0;
}
