#include <userver/utest/utest.hpp>

#include "default_vars.h"
#include "scaling/shifting.h"
#include "utils/file.h"

namespace {
using namespace server::core::scaling;
using server::core::pnm::color_space::ColorSpace;
using namespace server::core::utils;
}  // namespace

TEST(Shifting, Origin) {
  Shift shift(0, 0);
  save(shift(rgb_sample_1200).GetRaw(), "shifting/origin.pnm");
}

TEST(Shifting, LeftTop) {
  Shift shift(-200, -200);
  save(shift(rgb_sample_1200).GetRaw(), "shifting/left_top.pnm");
}

TEST(Shifting, RightBottom) {
  Shift shift(200, 200);
  save(shift(rgb_sample_1200).GetRaw(), "shifting/right_bottom.pnm");
}
