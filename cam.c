#include "cam.h"

#include "stdlib.h"

cam_t cam_create(cam_create_info_t cci) {
  cam_t res;
  res.pos = cci.lookfrom;
  res.aperture = cci.aperture;
  res.focus_dist = v3_len(v3_sub(cci.lookat, cci.lookfrom));
  // generate an onb
  v3 w = v3_norm(v3_sub(cci.lookat, cci.lookfrom)); // forward vector
  res.u = v3_cross(cci.up, w); // right
  res.v = v3_cross(res.u, w); // up
  res.u = v3_muls(res.u, cci.aspect); // right scaled for aspect ratio
  // temps
  v3 hu = v3_divs(res.u, 2);
  v3 hv = v3_divs(res.v, 2);
  v3 censor_centre = v3_add(res.pos, v3_muls(w, cci.lens_len));
  
  res.llc = v3_sub(censor_centre, v3_add(hu, hv)); // lower left corner
  return res;
}

// TODO: I DONT THINK THIS IS CORRECT AT ALL
ray_t cam_ray(cam_t cam, f32 u, f32 v) {
  f32 rx, ry;
  do {
    rx = (float)random()/RAND_MAX;
    ry = (float)random()/RAND_MAX;
  } while (rx * rx + ry * ry > 1);
  v3 rnd_x = v3_muls(cam.u, (rx - 0.5) * cam.aperture);
  v3 rnd_y = v3_muls(cam.v, (ry - 0.5) * cam.aperture);

  v3 p_scr = v3_add(cam.llc, v3_add(v3_muls(cam.u, u), v3_muls(cam.v, v)));
  // get focal point
  // TODO: THIS IS SO SLOW
  v3 fP = v3_add(cam.pos, v3_muls(v3_norm(v3_sub(p_scr, cam.pos)), cam.focus_dist));

  v3 o = v3_add(cam.pos, v3_add(rnd_x, rnd_y));
  return (ray_t) {
    .o = o,
    .d = v3_norm(v3_sub(fP, o))
  };
}
