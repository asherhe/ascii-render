#pragma once

#include <stddef.h>

#include "vec3.h"

//////// scene description ////////

struct Scene {
  double r_a;
  double r_b;

  // signed distance field of scene
  double sdf(Vec3 p);
  // gradient of scene(p)
  Vec3 sdf_grad(Vec3 p);
};

//////// raymarching ////////

const int MAX_MARCH_STEPS = 32;
const double MAX_MARCH_DIST = 10.0;
const double SURFACE_DIST = 0.01;

// raymarch onto scene given ray origin and direction
double march(Scene scene, Vec3 o, Vec3 d);
