#include <stdio.h>
#include <stdlib.h>

#define CLAP_IMPLEMENTATION
#include "clap.h"

typedef unsigned int  u32;
typedef unsigned char u8;

// forced 3 channels because PPM doesnt support Alpha
typedef struct {
  u32 width, height;
  u8  *data;
} char_image_t;

// uses calloc
// must be freed using char_image_free
char_image_t char_image_new(u32 width, u32 height) {
  char_image_t image = { .width = width, .height = height };
  image.data = calloc(width * height * 3, sizeof(unsigned char));

  return image;
}

void char_image_free(char_image_t img) {
  free(img.data);
}

// save pixel data to ppm
void save_as_ppm(FILE *out_file, char_image_t i) {
  fprintf(out_file, "P3\n");
  fprintf(out_file, "%u %u\n", i.width, i.height);
  fprintf(out_file, "255\n");
  for (size_t p = 0; p < i.width * i.height * 3; p += 3) {
    fprintf(out_file, "%uc %uc %uc\n", i.data[p], i.data[p+1], i.data[p+2]);
  }
}

typedef union {
  struct {
    float x, y, z;
  };
  float e[3];
} v3;

v3 v3_add(v3 a, v3 b) {
  return (v3) { a.x + b.x, a.y + b.y, a.z + b.z };
}

v3 v3_muls(v3 a, float t) {
  return (v3) { a.x * t, a.y * t, a.z * t };
}

typedef struct {
  v3 o, d;
} ray_t;

v3 ray_at(ray_t r, float t) {
  return v3_add(r.o, v3_muls(r.d, t));
}

int main(int argc, const char **argv) {
  const char *outfile_path = "out.ppm";

  char_image_t image = char_image_new(100, 100);
  

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
