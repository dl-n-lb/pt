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

#define SKY_COLOR (v3) {{0.1, 0.2, 0.4}}

typedef struct {
  bool is_light;
  union {
    v3 color;
    v3 emission;
  };
} mat_t;

// NOTE: align this?
typedef struct {
  v3 c;
  f32 r;
  mat_t mat;
} sphere_t;

typedef struct {
  v3 v0, v1, v2;
  mat_t mat;
} tri_t;

typedef struct {
  sphere_t *spheres;
  size_t spheres_cnt;
  tri_t *tris;
  size_t tris_cnt;
} scene_t;

// NOTE: basically an optional type for now
// might later return more info? idk prolly not
typedef struct {
  bool exists;
  mat_t mat;
  f32 t;
  v3 p;
  v3 n;
} intersection_t;

intersection_t intersect_tri(ray_t r, tri_t tri, f32 tmin, f32 tmax) {
  // first check if the ray direction
  // is parallel to the plane
  // the triangle lies on
  v3 e0 = v3_sub(tri.v1, tri.v0);
  v3 e1 = v3_sub(tri.v2, tri.v0);
  // triple product for determinant
  v3 p = v3_cross(r.d, e1);
  f32 det = v3_dot(e0, p);

  if (fabs(det) < 0.001) return (intersection_t){false};

  f32 idet = 1/det;
  // construct barycentric coords
  // while return
  v3 tv = v3_sub(r.o, tri.v0);
  f32 u = v3_dot(tv, p) * idet;
  if (u < 0 || u > 1) return (intersection_t){false};

  v3 qv = v3_cross(tv, e0);
  f32 v = v3_dot(r.d, qv) * idet;
  if (v < 0 || u + v > 1) return (intersection_t){false};

  f32 t = v3_dot(e1, qv) * idet;
  if (t < tmin || t > tmax) return (intersection_t){false};
  return (intersection_t) {
    .exists=true,
    .mat=tri.mat,
    .t = t,
    .p=ray_at(r, t),
    .n=v3_cross(e0, e1)
  };
}

intersection_t intersect_sphere(ray_t r, sphere_t sph, f32 tmin, f32 tmax) {
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
  f32 tmin = 0.0001f, tmax = 1e5; // small number, large number (inf)
  intersection_t closest = {false}, res;
  for (size_t i = 0; i < scene.spheres_cnt; ++i) {
    res = intersect_sphere(r, scene.spheres[i], tmin, tmax);
    if (res.exists) {
      tmax = res.t;
      closest = res;
    }
  }
  for (size_t i = 0; i < scene.tris_cnt; ++i) {
    res = intersect_tri(r, scene.tris[i], tmin, tmax);
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
      return v3_mul(acc, SKY_COLOR);
    }
    if (i.mat.is_light) {
      return v3_mul(acc, i.mat.emission);
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
  if (i.mat.is_light) {
    return i.mat.emission;
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

#define WIDTH 1280
#define HEIGHT 720
#define SPP 500

int main(int argc, const char **argv) {
  (void)argc;
  (void)argv;
  const char *outfile_path = "out.ppm";

  float3_image_t render = float3_image_new(WIDTH, HEIGHT);

  cam_t cam = cam_create((cam_create_info_t) {
      .up = (v3) {{0, 1, 0}},
      .lookfrom = (v3) {{0, 0, -1}},
      .lookat = (v3) {{0, 0, 2}},
      .lens_len = 1.f,
      .aperture = 0.15f,
      .aspect = (float)WIDTH/HEIGHT,
    });

  sphere_t sph = {
    .c = (v3) {{ 3, 1.2, 1.3 }},
    .r = 1.f,
    .mat.is_light = true,
    .mat.emission = (v3) {{2.2, 2.5, 3.5}}
  };
  sphere_t sph2 = {
    .c = (v3) {{ 0, -10 - 0.8, 2 }},
    .r = 10.f,
    .mat.color = (v3) {{0.9, 0.9, 0.8}}
  };
  sphere_t sph3 = {
    .c = (v3) {{ 0, 0, 2 }},
    .r = 0.8f,
    .mat.color = (v3) {{0.8, 0.4, 0.1}}
  };

  tri_t tri = {
    .v0 = (v3) {{-2, 1, 2.5}},
    .v1 = (v3) {{-2.4, 0.8, 1.6}},
    .v2 = (v3) {{-1.6, 0, 2}},
    .mat.is_light = true,
    .mat.emission = (v3) {{3, 3, 3}},
  };

  // TODO: TEMP
  sphere_t spheres[3] = {sph, sph2, sph3};
  tri_t tris[1] = {tri};
  scene_t sc = {spheres, 3, tris, 1};

  // TODO: WHY IS THIS SO SLOW
  ray_t r;
  v3 col;
  //#pragma omp parallel for shared(render, sc, cam)
  for (size_t i = 0; i < HEIGHT; ++i) {
    for (size_t j = 0; j < WIDTH; ++j) {
      col = (v3){0};
      for (size_t s = 0; s < SPP; ++s) {	
	float jx = j + (float)random()/RAND_MAX;
	float jy = i + (float)random()/RAND_MAX;
	r = cam_ray(cam, jx/WIDTH, jy/HEIGHT);
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
