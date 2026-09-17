#include "scene.h"

#include <cmath>

double Scene::sdf(Vec3 p) {
  // distance from major radius along XY plane
  double rad_plane = sqrt(p.x * p.x + p.z * p.z) - r_a;
  // distance to major radius ring
  double rad_ring = sqrt(rad_plane * rad_plane + p.y * p.y);
  return rad_ring - r_b;
}

Vec3 Scene::sdf_grad(Vec3 p) {
  double rad_plane = sqrt(p.x * p.x + p.z * p.z);
  return p.hadamard(Vec3(rad_plane - r_a, rad_plane, rad_plane - r_a));
}

double march(Scene scene, Vec3 o, Vec3 d) {
  double t = 0.0;

  for (int i = 0; i < MAX_MARCH_STEPS; ++i) {
    Vec3 pos = o + d * t;
    double dist = scene.sdf(pos);
    t += dist;

    if (t > MAX_MARCH_DIST || dist < SURFACE_DIST) break;
  }

  return t;
}
