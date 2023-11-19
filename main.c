#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define CLAP_IMPLEMENTATION
#include "clap.h"

#include "types.h"
#include "vec.h"
#include "image.h"
#include "ray.h"
#include "cam.h"

typedef struct {
  v3 c;
  f32 r;
} sphere_t;

/// ? basically an optional type for now
// might later return more info? idk prolly not
typedef struct {
  bool exists;
  float t;
} intersection_t;

intersection_t intersect(ray_t r, sphere_t sph) {
  v3 oc = v3_sub(r.o, sph.c);
  f32 c = v3_dot(oc, oc) - sph.r * sph.r;
  f32 hb = v3_dot(oc, r.d);
  f32 a = v3_dot(r.d, r.d);

  f32 disc = hb * hb - a * c;
  if (disc < 0) {
    return (intersection_t) {false};
  }
  float t = (-hb - sqrt(disc))/a;
  if (t > 0) {
    return (intersection_t) {true, t};
  }
  t = (-hb + sqrt(disc))/a;
  if (t > 0) {
    return (intersection_t) {true, t};
  }
  return (intersection_t) {false};
}

#define WIDTH 100
#define HEIGHT 100

int main(int argc, const char **argv) {
  (void)argc;
  (void)argv;
  const char *outfile_path = "out.ppm";

  float3_image_t render = float3_image_new(WIDTH, HEIGHT);

  cam_t cam = cam_create((cam_create_info_t) {
      .up = (v3) {{0, 1, 0}},
      .lookfrom = (v3) {{0, 0, -1}},
      .lookat = (v3) {{0}},
      .lens_len = 1.f,
      .aperture = 1.f,
      .aspect = 1.f,
    });

  sphere_t sph = {
    .c = (v3) {{ 0, 0, 2 }},
    .r = 0.8f
  };

  for (size_t i = 0; i < HEIGHT; ++i) {
    for (size_t j = 0; j < WIDTH; ++j) {      
      ray_t r = cam_ray(cam, (float)j/WIDTH, (float)i/HEIGHT);
      intersection_t is = intersect(r, sph);
      if (is.exists) {
	render.data[(i*WIDTH + j) * 3] = 1;
      }
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
