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

#define SKY_COLOR (v3) {{0.65, 0.85, 0.9}}

typedef struct {
  v3 color;
} mat_t;

// NOTE: align this?
typedef struct {
  v3 c;
  f32 r;
  mat_t mat;
} sphere_t;

typedef struct {
  sphere_t *spheres;
  size_t cnt;
} scene_t;

// NOTE: basically an optional type for now
// might later return more info? idk prolly not
typedef struct {
  bool exists;
  mat_t mat;
  float t;
  v3 p;
  v3 n;
} intersection_t;

intersection_t intersect_sphere(ray_t r, sphere_t sph, float tmin, float tmax) {
  v3 oc = v3_sub(r.o, sph.c);
  f32 c = v3_dot(oc, oc) - sph.r * sph.r;
  f32 hb = v3_dot(oc, r.d);
  f32 a = v3_dot(r.d, r.d);

  f32 disc = hb * hb - a * c;
  if (disc < 0) {
    return (intersection_t) {false};
  }
  float t = (-hb - sqrt(disc))/a;
  if (t > tmin && t < tmax) {
    v3 p = ray_at(r, t);
    v3 n = v3_norm(v3_sub(p, sph.c));
    return (intersection_t) {true, sph.mat, t, p, n};
  }
  t = (-hb + sqrt(disc))/a;
  if (t > tmin && t < tmax) {
    v3 p = ray_at(r, t);
    v3 n = v3_norm(v3_sub(p, sph.c));
    return (intersection_t) {true, sph.mat, t, p, n};
  }
  return (intersection_t) {false};
}

intersection_t intersect_scene(ray_t r, const scene_t scene) {
  float tmin = 0.0001f, tmax = 1e5; // small number, large number (inf)
  intersection_t closest = {false};
  for (size_t i = 0; i < scene.cnt; ++i) {
    intersection_t res = intersect_sphere(r, scene.spheres[i], tmin, tmax);
    if (res.exists) {
      tmax = res.t;
      closest = res;
    }
  }
  return closest;
}

v3 ray_color_it(ray_t r, const scene_t sc, int depth) {
  v3 acc = {{1, 1, 1}};
  intersection_t i;
  for (int x = 0; x < depth; ++x) {
    i = intersect_scene(r, sc);
    if (!i.exists) {
      acc = v3_mul(acc, SKY_COLOR);
      return acc;
    }
    v3 nd;
    do {
      nd = rand_unit_v3();
    } while(v3_dot(nd, i.n) < 0);
    r = (ray_t) {i.p, nd};
    acc = v3_mul(acc, i.mat.color);
  }
  return (v3) {0};
}

v3 ray_color(ray_t r, const scene_t sc, int depth) {
  if (depth == 0) {
    return (v3) {0};
  }
  intersection_t i = intersect_scene(r, sc);
  if (!i.exists) {
    return SKY_COLOR;
  }
  v3 new_d;
  // force the new direction to be in the same hemisphere as the normal
  // to satisfy that this integral is over the hemisphere
  do {
    new_d = rand_unit_v3(); // random on the sphere
  }  while (v3_dot(new_d, i.n) < 0);

  v3 new_o = i.p;
  ray_t r_new = {new_o, new_d};
  // recursive call to ray_color
  return v3_mul(i.mat.color, ray_color(r_new, sc, depth-1));
}

#define WIDTH 2500
#define HEIGHT 2500
#define SPP 100

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
    .r = 0.8f,
    .mat.color = (v3) {{0.9, 0.8, 0.9}}
  };
  sphere_t sph2 = {
    .c = (v3) {{ 0, -10 - 0.8, 2 }},
    .r = 10.f,
    .mat.color = (v3) {{0.9, 0.9, 0.8}}
  };
  // TODO: TEMP
  sphere_t spheres[2] = {sph, sph2};
  scene_t sc = {spheres, 2};

  //#pragma omp parallel for
  for (size_t i = 0; i < HEIGHT; ++i) {
    for (size_t j = 0; j < WIDTH; ++j) {
      v3 col = {0};
      ray_t r = cam_ray(cam, (float)j/WIDTH, (float)i/HEIGHT);
      for (size_t s = 0; s < SPP; ++s) {
	col = v3_add(col, ray_color_it(r, sc, 10));
      }
      col = v3_divs(col, SPP);

      render.data[(i*WIDTH + j) * 3] = col.x;
      render.data[(i*WIDTH + j) * 3 + 1] = col.y;
      render.data[(i*WIDTH + j) * 3 + 2] = col.z;
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
