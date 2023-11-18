#include "types.h"

// forced 3 channels because PPM doesnt support Alpha
typedef struct {
  u32 width, height;
  u8  *data;
} char_image_t;

// uses calloc
// must be freed using char_image_free
char_image_t char_image_new(u32 width, u32 height);
void char_image_free(char_image_t img);

// save pixel data to ppm
void save_as_ppm(FILE *out_file, char_image_t i);

// 3 channel float image (temp?)
typedef struct {
  u32 width, height;
  float  *data;
} float3_image_t;

float3_image_t float3_image_new(u32 width, u32 height);
void float3_image_free(float3_image_t img);

// creates a char image which must be freed using char_image_free
char_image_t float3_to_char_image(float3_image_t img);
