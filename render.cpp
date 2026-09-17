#include "render.h"

#include <algorithm>

void Renderer::resize(size_t rows, size_t cols) {
  n_rows_ = rows;
  n_cols_ = cols;

  mid_row_ = n_rows_ / 2.0;
  mid_col_ = n_cols_ / 2.0;

  win_half_size_ = 0.5 * std::min(n_rows_ * aspect_, (double)n_cols_);
}

double Renderer::render(Vec3 o, Vec3 d) {
  double t = scene_->march(o, d);

  if (t == MARCH_MISS) return config_.sky_color;

  Vec3 hit_pos = o + d * t;
  Vec3 normal = scene_->sdf_grad(hit_pos).normalized();

  double lambert = 0.4 + 0.6 * normal.dot(config_.sun_dir);
  if (lambert < 0.0) lambert = 0.0;
  if (lambert > 1.0) lambert = 1.0;

  double noise = real_dist(gen);

  return lambert + 0.2 * noise;
}

double Renderer::render(double row, double col) {
  // undo text stretching
  double x = col - mid_col_;
  double y = aspect_ * (mid_row_ - row);

  // scale smallest dimension to [-1,1]
  x /= win_half_size_;
  y /= win_half_size_;

  // camera space view direction
  Vec3 dir = camera_->cam2world_dir(Vec3(scale_ * x, scale_ * y, 1.0));

  return render(camera_->pos, dir);
}

char Renderer::rendered_char(size_t row, size_t col) {
  double val_upper = render(row - 0.3, col - 0.2);
  double val_lower = render(row + 0.3, col + 0.2);

  return nearest_char(val_upper, val_lower);
}

char nearest_char(double upper, double lower) {
  char best_char = ' ';
  double best_dist = INFINITY;

  // invert intensity because 0.0 is white
  upper = 1.0 - upper;
  lower = 1.0 - lower;

  for (int i = 0; i < NUM_CHARS; ++i) {
    double dup = CHAR_UPPER[i] - upper, dlo = CHAR_LOWER[i] - lower;
    double dist = dup * dup + dlo * dlo;
    if (dist < best_dist) {
      best_dist = dist;
      best_char = CHAR_SET[i];
    }
  }

  return best_char;
}
