#include <stdio.h>
#include <stdlib.h>

#include "image.h"

char_image_t char_image_new(u32 width, u32 height) {
  char_image_t image = { .width = width, .height = height };
  image.data = calloc(width * height * 3, sizeof(unsigned char));

  return image;
}

void char_image_free(char_image_t img) {
  free(img.data);
}

void save_as_ppm(FILE *out_file, char_image_t i) {
  fprintf(out_file, "P3\n");
  fprintf(out_file, "%u %u\n", i.width, i.height);
  fprintf(out_file, "255\n");
  for (size_t p = 0; p < i.width * i.height * 3; p += 3) {
    fprintf(out_file, "%uc %uc %uc\n", i.data[p], i.data[p+1], i.data[p+2]);
  }
}

float3_image_t float3_image_new(u32 width, u32 height) {
  float3_image_t image = { .width = width, .height = height };
  image.data = calloc(width * height * 3, sizeof(float));

  return image;
}

void float3_image_free(float3_image_t img) {
  free(img.data);
}

char_image_t float3_to_char_image(float3_image_t img) {
  char_image_t res = char_image_new(img.width, img.height);
  for (size_t i = 0; i < img.width * img.height * 3; ++i) {
    // assume 0-1. -> 0-255
    
    float curr = img.data[i];
    // clamp
    curr = (curr < 0) ? 0 : curr;
    curr = (curr > 1) ? 1 : curr;
    curr *= 255.;
    res.data[i] = (unsigned char)curr;
  }
  return res;
}
