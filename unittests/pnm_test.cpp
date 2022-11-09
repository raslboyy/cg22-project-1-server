#include <userver/utest/utest.hpp>

#include "../src/core/pnm.h"

using namespace server::core::pnm;
using namespace server::core::pnm::color_space;

TEST(PnmBasic, P5Struct) {
  std::string buffer = "P5\n2 2\n255\nfrfg";
  EXPECT_NO_THROW(
      PNM<color_space::ColorSpace::NONE>(bytes{buffer.begin(), buffer.end()}););
}

TEST(PnmBasic, P6Struct) {
  std::string buffer = "P6 2 2 255 aaabbbcccddd";
  EXPECT_NO_THROW(
      PNM<color_space::ColorSpace::RGB>(bytes{buffer.begin(), buffer.end()}););
}

TEST(PnmConversion, RGBtoHSL) {
  std::string buffer = "P6 2 2 255 aaabbbcccddd";
  PNM<ColorSpace::RGB> rgb;
  EXPECT_NO_THROW(
      rgb = PNM<ColorSpace::RGB>(bytes{buffer.begin(), buffer.end()}););
  auto hsl = ColorSpaceConversion<ColorSpace::RGB, ColorSpace::HSL>(rgb);
  auto rgb_ = ColorSpaceConversion<ColorSpace::HSL, ColorSpace::RGB>(hsl);
  ASSERT_EQ(rgb, rgb_);
}