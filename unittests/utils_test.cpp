#include <userver/utest/utest.hpp>

#include "utils/file.h"

namespace {
using namespace server::core::utils;
using server::core::pnm::color_space::ColorSpace;
}  // namespace

TEST(Utils, GrayscaleGradient) {
  auto pnm = GenerateGradient<ColorSpace::NONE>(100, 1000);
  save(pnm.GetRaw(), "utils_test_grayscale_gradient.pgm");
}
