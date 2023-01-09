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

TEST(PnmConversion, RGBtoCMY) {
  std::string buffer = "P6 2 2 255 aaabbbcccddd";
  PNM<ColorSpace::RGB> rgb;
  EXPECT_NO_THROW(
      rgb = PNM<ColorSpace::RGB>(bytes{buffer.begin(), buffer.end()}););
  auto cmy = ColorSpaceConversion<ColorSpace::RGB, ColorSpace::CMY>(rgb);
  auto rgb_ = ColorSpaceConversion<ColorSpace::CMY, ColorSpace::RGB>(cmy);
  ASSERT_EQ(rgb, rgb_);
}

TEST(PnmConversion, RGBtoYCbCr601) {
  std::string buffer = "P6 2 2 255 aaabbbcccddd";
  PNM<ColorSpace::RGB> rgb;
  EXPECT_NO_THROW(
      rgb = PNM<ColorSpace::RGB>(bytes{buffer.begin(), buffer.end()}););
  auto y601 = ColorSpaceConversion<ColorSpace::RGB, ColorSpace::YCbCr601>(rgb);
  auto rgb_ = ColorSpaceConversion<ColorSpace::YCbCr601, ColorSpace::RGB>(y601);
  ASSERT_EQ(rgb, rgb_);
}

TEST(PnmConversion, RGBtoYCbCr709) {
  std::string buffer = "P6 2 2 255 aaabbbcccddd";
  PNM<ColorSpace::RGB> rgb;
  EXPECT_NO_THROW(
      rgb = PNM<ColorSpace::RGB>(bytes{buffer.begin(), buffer.end()}););
  auto y709 = ColorSpaceConversion<ColorSpace::RGB, ColorSpace::YCbCr709>(rgb);
  auto rgb_ = ColorSpaceConversion<ColorSpace::YCbCr709, ColorSpace::RGB>(y709);
  ASSERT_EQ(rgb, rgb_);
}

TEST(PnmConversion, RGBtoYCoCg) {
  std::string buffer = "P6 2 2 255 aaabbbcccddd";
  PNM<ColorSpace::RGB> rgb;
  EXPECT_NO_THROW(
      rgb = PNM<ColorSpace::RGB>(bytes{buffer.begin(), buffer.end()}););
  auto ycocg = ColorSpaceConversion<ColorSpace::RGB, ColorSpace::YCoCg>(rgb);
  auto rgb_ = ColorSpaceConversion<ColorSpace::YCoCg, ColorSpace::RGB>(ycocg);
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