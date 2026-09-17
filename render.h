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

const size_t NUM_CHARS = 18;
const char CHAR_SET[] = "!\"#$%&'*+,-.:;=?@~";
// raw top and bottom values of character. values are from counting the black
// pixels of each character and normalizing to [0.0, 1.0]
// https://www.desmos.com/calculator/xu6d55wtjz
const double CHAR_UPPER[]{0.6335037096645826,  0.46274417905432286,
                          0.2620000353722487,  0.050279440764747885,
                          0.18714130329050338, 0.28713643960630686,
                          0.7070779869653263,  0.43275735521696446,
                          0.6693800129108707,  1,
                          0.8748441410291556,  1,
                          0.8104091684868636,  0.7957075776199782,
                          0.6740624143328352,  0.4213299081205839,
                          0.08053597799846124, 0.66622746124528};
const double CHAR_LOWER[]{0.7600368755692721,
                          1,
                          0.35377996692694746,
                          0.19188781691324064,
                          0.22789676609216236,
                          0.10033117267847502,
                          1,
                          0.6401492708895236,
                          0.6535818823342147,
                          0.7070779869653263,
                          0.8827631032073786,
                          0.7972042658931935,
                          0.8104091684868636,
                          0.7069320764394293,
                          0.6740624143328352,
                          0.7400294473970446,
                          0,
                          0.8105484467161288};

// find the character from CHAR_SET that most closely approximates the
// brightness profile of the given light intensities.
char nearest_char(double upper, double lower);
