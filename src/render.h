#pragma once

#include <random>

#include "scene.h"
#include "vec3.h"

struct RenderConfig {
  Vec3 sun_dir = Vec3(-0.85, 0.44, -0.27).normalized();
  double sky_color = 0.0;
};

class Renderer {
 private:
  Scene* scene_;
  Camera* camera_;

  std::random_device rd;
  std::mt19937 gen;
  std::uniform_real_distribution<double> real_dist;

  RenderConfig config_;

  // character display settings
  double aspect_ = 2.0;

  double scale_ = 0.66;

  size_t n_rows_, n_cols_;    // text size
  double mid_row_, mid_col_;  // text midpoint

  double win_half_size_;  // half size of shortest edge of window, in columns

 public:
  Renderer(Scene& scene, Camera& camera)
      : scene_(&scene), camera_(&camera), gen(rd()), real_dist(0.0, 1.0) {}

  // notify
  void resize(size_t rows, size_t cols);

  // cast a view ray in a direction and calculate light intensity
  double render(Vec3 o, Vec3 d);

  // render light intensity from character space
  // inputs are doubles because of subcharacter antialiasing
  double render(double row, double col);

  // determine character to be placed at row, col
  char rendered_char(size_t row, size_t col);
};

// find the character from CHAR_SET that most closely approximates the
// brightness profile of the given light intensities.
char nearest_char(double upper, double lower);
