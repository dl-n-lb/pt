#include "cam.h"

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
  v3 p_scr = v3_add(cam.llc, v3_add(v3_muls(cam.u, u), v3_muls(cam.v, v)));
  return (ray_t) {
    .o = cam.pos,
    .d = v3_norm(v3_sub(p_scr, cam.pos))
  };
}
