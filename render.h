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

const size_t NUM_CHARS = 19;
const char CHAR_SET[] = " !\"#$%&'*+,-.:;=?@~";
// raw top and bottom values of character. values are from counting the black
// pixels of each character and normalizing to [0.0, 1.0]
// https://www.desmos.com/calculator/xu6d55wtjz
const double CHAR_UPPER[]{0,
                          0.36649629033541736,
                          0.5372558209456771,
                          0.7379999646277513,
                          0.9497205592352521,
                          0.8128586967094966,
                          0.7128635603936931,
                          0.2929220130346737,
                          0.5672426447830355,
                          0.33061998708912926,
                          0,
                          0.1251558589708444,
                          0,
                          0.18959083151313638,
                          0.20429242238002177,
                          0.3259375856671648,
                          0.5786700918794161,
                          0.9194640220015388,
                          0.33377253875472};
const double CHAR_LOWER[]{0,
                          0.2399631244307279,
                          0,
                          0.6462200330730525,
                          0.8081121830867594,
                          0.7721032339078376,
                          0.899668827321525,
                          0,
                          0.35985072911047644,
                          0.34641811766578534,
                          0.2929220130346737,
                          0.11723689679262136,
                          0.2027957341068065,
                          0.18959083151313638,
                          0.2930679235605706,
                          0.3259375856671648,
                          0.2599705526029554,
                          1,
                          0.18945155328387114};

// find the character from CHAR_SET that most closely approximates the
// brightness profile of the given light intensities.
char nearest_char(double upper, double lower);
