#include <userver/utest/utest.hpp>

#include "core/pnm.h"

#include <algorithm>
#include <cctype>
#include <string>

using namespace server::core::pnm;
using ColorSpace = color_space::ColorSpace;
using Mask = color_space::Mask;

TEST(PnmConversion, RGBtoHSL) {
  std::string buffer = "P6 2 2 255 aaabbbcccddd";
  PNM<ColorSpace::RGB> rgb(bytes{buffer.begin(), buffer.end()});
  auto hsl = ColorSpaceConversion<ColorSpace::RGB, ColorSpace::HSL>(rgb);
  auto rgb_ = ColorSpaceConversion<ColorSpace::HSL, ColorSpace::RGB>(hsl);

  ASSERT_EQ(rgb, rgb_);
}

TEST(PnmConversion, RGBtoHSV) {
  std::string buffer = "P6 2 2 255 aaabbbcccddd";
  PNM<ColorSpace::RGB> rgb(bytes{buffer.begin(), buffer.end()});
  auto hsv = ColorSpaceConversion<ColorSpace::RGB, ColorSpace::HSV>(rgb);
  auto rgb_ = ColorSpaceConversion<ColorSpace::HSV, ColorSpace::RGB>(hsv);

  ASSERT_EQ(rgb, rgb_);
}

TEST(PnmConversion, RGBtoNONE) {
  std::string buffer = "P6 2 2 255 aaabbbcccddd";
  PNM<ColorSpace::RGB> rgb(bytes{buffer.begin(), buffer.end()});
  auto none =
      ColorSpaceConversion<ColorSpace::RGB, ColorSpace::NONE, Mask::SECOND>(rgb)
          .GetRaw();
  auto actual = std::string{none.begin(), none.end()};

  ASSERT_EQ("P5 2 2 255 abcd", actual);
}