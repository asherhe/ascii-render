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

double Scene::march(Vec3 o, Vec3 d) {
  double t = 0.0;

  for (int i = 0; i < MAX_MARCH_STEPS; ++i) {
    Vec3 pos = o + d * t;
    double dist = sdf(pos);
    t += dist;

    if (dist < SURFACE_DIST) return t;
    if (t > MAX_MARCH_DIST) break;
  }
  return MARCH_MISS;
}

void Camera::update(double t) {
  dir = Vec3(t + 0.84, -0.38, 2 * t - 0.42);
  pos = cam2world_dir(Vec3(0.0, 0.0, -3.6));
}

Vec3 Camera::cam2world_dir(Vec3 v) {
  double sp = sin(dir.x), sy = sin(dir.y), sr = sin(dir.z);
  double cp = cos(dir.x), cy = cos(dir.y), cr = cos(dir.z);
  return Vec3(
      v.dot(Vec3(cr * cy, cr * sy * sp - cp * sr, sr * sp + cr * cp * sy)),
      v.dot(Vec3(cy * sr, cr * cp + sr * sy * sp, cp * sr * sy - cr * sp)),
      v.dot(Vec3(-sy, cy * sp, cy * cp)));
}
