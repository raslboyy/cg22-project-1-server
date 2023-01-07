#include <userver/utest/utest.hpp>

#include "default_vars.h"
#include "dithering/atkinson.h"
#include "dithering/floyd_steinberg.h"
#include "dithering/ordered.h"
#include "dithering/random.h"
#include "dithering/type.h"
#include "utils/file.h"

namespace {
using namespace server::core::dithering;
using server::core::pnm::color_space::ColorSpace;
using namespace server::core::utils;
}  // namespace

TEST(Dithering, Random) {
  GTEST_SKIP() << "skip random";
  auto gradient = GenerateGradient<ColorSpace::NONE>(100, 1000);
  for (int i = 1; i <= 8; i++) {
    Dithering<Algorithm::Random, ColorSpace::NONE> dithering(i);
    save(dithering(gradient).GetRaw(),
         "dithering/random_" + std::to_string(i) + ".pgm");
  }
}

TEST(Dithering, Ordered) {
  GTEST_SKIP() << "skip ordered";
  auto gradient = GenerateGradient<ColorSpace::NONE>(100, 1000);
  for (int i = 0; i <= 6; i++) {
    Dithering<Algorithm::Ordered, ColorSpace::NONE> dithering(i);
    save(dithering(gradient).GetRaw(),
         "dithering/ordered_" + std::to_string(i) + ".pgm");
  }
}

TEST(Dithering, FloydSteinberg) {
  GTEST_SKIP() << "skip floyd steinberg";
  auto gradient = GenerateGradient<ColorSpace::NONE>(100, 1000);
  for (int i = 1; i <= 8; i++) {
    Dithering<Algorithm::FloydSteinberg, ColorSpace::NONE> dithering(i);
    save(dithering(gradient).GetRaw(),
         "dithering/floyd_steinberg_" + std::to_string(i) + ".pgm");
  }
}

TEST(Dithering, Atkinson) {
  GTEST_SKIP() << "skip atkinson";
  auto gradient = GenerateGradient<ColorSpace::NONE>(100, 1000);
  for (int i = 1; i <= 8; i++) {
    Dithering<Algorithm::Atkinson, ColorSpace::NONE> dithering(i);
    save(dithering(gradient).GetRaw(),
         "dithering/atkinson_" + std::to_string(i) + ".pgm");
  }
}
