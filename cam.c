#include "cam.h"

#include "math.h"
#include "stdlib.h"

cam_t cam_create(cam_create_info_t cci) {
  cam_t res;
  res.pos = cci.lookfrom;
  res.aperture = cci.aperture;
  res.focus_dist = cci.focus_dist;
  // generate an onb
  v3 w = v3_norm(v3_sub(cci.lookat, cci.lookfrom)); // forward vector
  f32 vph = tan(cci.vfov / 2);
  res.u = v3_cross(cci.up, w); // right
  res.v = v3_cross(res.u, w); // up
  res.v = v3_muls(res.v, 2 * vph * cci.focus_dist);
  res.u = v3_muls(res.u, 2 * vph * cci.focus_dist * cci.aspect); // right scaled for aspect ratio
  // temps
  v3 hu = v3_divs(res.u, 2);
  v3 hv = v3_divs(res.v, 2);
  v3 censor_centre = v3_add(res.pos, v3_muls(w, cci.focus_dist));
  
  res.llc = v3_sub(censor_centre, v3_add(hu, hv)); // lower left corner
  return res;
}

// TODO: I DONT THINK THIS IS CORRECT AT ALL
ray_t cam_ray(cam_t cam, f32 u, f32 v) {
  v3 o = cam.pos;
  if (cam.aperture > 0) {
    f32 rx, ry;
    do {
      rx = (float)random()/RAND_MAX;
      ry = (float)random()/RAND_MAX;
    } while (rx * rx + ry * ry > 1);
    v3 rnd_x = v3_muls(cam.u, (rx - 0.5) * cam.aperture);
    v3 rnd_y = v3_muls(cam.v, (ry - 0.5) * cam.aperture);
    o = v3_add(cam.pos, v3_add(rnd_x, rnd_y));
  }
  v3 p_scr = v3_add(cam.llc, v3_add(v3_muls(cam.u, u), v3_muls(cam.v, v)));

  
  return (ray_t) {
    .o = o,
    .d = v3_norm(v3_sub(p_scr, o))
  };
}
