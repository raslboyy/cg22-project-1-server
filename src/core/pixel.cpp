#include "pixel.h"

#include <math.h>
#include <algorithm>

namespace server::core::pnm::color_space {

static const channel eps = 1e-6;
static bool compare(channel a, channel b) { return std::abs(a - b) < eps; }

static channel get_hue(channel R, channel G, channel B) {
  auto x_max = std::max(std::max(R, G), B);
  auto x_min = std::min(std::min(R, G), B);
  auto C = x_max - x_min;
  auto V = x_max;
  if (compare(C, 0))
    return 0;
  else if (compare(V, R))
    return 60 * ((G - B) / C);
  else if (compare(V, G))
    return 60 * (2 + (B - R) / C);
  else if (compare(V, B))
    return 60 * (4 + (R - G) / C);
  static_assert(true, "get_hue error");
  return {};
}

// From RGB to HSL
template <>
Pixel<ColorSpace::HSL> ColorSpaceConversion<ColorSpace::RGB, ColorSpace::HSL>(
    Pixel<ColorSpace::RGB> from) {
  Pixel<ColorSpace::HSL> to;
  auto x_max = std::max(std::max(from.red, from.green), from.blue);
  auto x_min = std::min(std::min(from.red, from.green), from.blue);
  to.lightness = (x_max + x_min) / 2;
  to.hue = get_hue(from.red, from.green, from.blue);

  auto L = to.lightness;
  auto V = x_max;
  if (compare(L, 0) || compare(L, 1))
    to.saturation = 0;
  else
    to.saturation = (V - L) / std::min(L, 1 - L);

  return to;
}

// From RGB to HSV
template <>
Pixel<ColorSpace::HSV> ColorSpaceConversion<ColorSpace::RGB, ColorSpace::HSV>(
    Pixel<ColorSpace::RGB> from) {
  Pixel<ColorSpace::HSV> to;
  to.value = std::max(std::max(from.red, from.green), from.blue);
  to.hue = get_hue(from.red, from.green, from.blue);

  auto x_max = std::max(std::max(from.red, from.green), from.blue);
  auto x_min = std::min(std::min(from.red, from.green), from.blue);
  auto C = x_max - x_min;
  auto V = x_max;
  to.saturation = compare(V, 0) ? 0 : C / V;

  return to;
}

// From HSL to RGB
template <>
Pixel<ColorSpace::RGB> ColorSpaceConversion<ColorSpace::HSL, ColorSpace::RGB>(
    Pixel<ColorSpace::HSL> from) {
  auto C = (1 - std::abs(2 * from.lightness - 1)) * from.saturation;
  auto H_ = from.hue / 60;
  auto X = C * (1 - std::abs(fmod(H_, 2) - 1));
  channel R1 = 0, G1 = 0, B1 = 0;
  if (H_ < 1.)
    R1 = C, G1 = X, B1 = 0;
  else if (H_ < 2)
    R1 = X, G1 = C, B1 = 0;
  else if (H_ < 3)
    R1 = 0, G1 = C, B1 = X;
  else if (H_ < 4)
    R1 = 0, G1 = X, B1 = C;
  else if (H_ < 5)
    R1 = X, G1 = 0, B1 = C;
  else if (H_ < 6. + eps)
    R1 = C, G1 = 0, B1 = X;
  else
    static_assert(true, "HSL to RGB error");
  auto m = from.lightness - C / 2;
  Pixel<ColorSpace::RGB> to;
  to.red = R1 + m, to.green = G1 + m, to.blue = B1 + m;
  return to;
}

//From RGB to CMY
template<>
Pixel<ColorSpace::CMY> ColorSpaceConversion<ColorSpace::RGB, ColorSpace::CMY>(
    Pixel<ColorSpace::RGB> from) {
  Pixel<ColorSpace::CMY> to;
  to.cian = 1 - from.red;
  to.magenta = 1 - from.green;
  to.yellow = 1 - from.blue;
  return to;
}

//From CMY to RGB
template<>
Pixel<ColorSpace::RGB> ColorSpaceConversion<ColorSpace::CMY, ColorSpace::RGB>(
    Pixel<ColorSpace::CMY> from) {
  Pixel<ColorSpace::RGB> to;
  to.red = 1 - from.cian;
  to.green = 1 - from.magenta;
  to.blue = 1 - from.yellow;
  return to;
}

//From RGB to YCbCr.601
template<>
Pixel<ColorSpace::YCbCr601> ColorSpaceConversion<ColorSpace::RGB, ColorSpace::YCbCr601>(
    Pixel<ColorSpace::RGB> from) {
  Pixel<ColorSpace::YCbCr601> to;
  to.luma = 0.299 * from.red + 0.587 * from.green + 0.114 * from.blue;
  to.blue_diff = (from.blue - to.luma) / 1.772;
  to.red_diff = (from.red - to.luma) / 1.402;
  return to;
}

//From YCbCr.601 to RGB
template<>
Pixel<ColorSpace::RGB> ColorSpaceConversion<ColorSpace::YCbCr601, ColorSpace::RGB>(
    Pixel<ColorSpace::YCbCr601> from) {
  Pixel<ColorSpace::RGB> to;
  to.red = from.luma + 1.402 * from.red_diff;
  to.green = from.luma - (0.299 * 1.402 / 0.587) * from.red_diff - (0.114 * 1.772 / 0.587) * from.blue_diff;
  to.blue = from.luma + 1.772 * from.blue_diff;
  return to;
}

//From RGB to YCbCr.709
template<>
Pixel<ColorSpace::YCbCr709> ColorSpaceConversion<ColorSpace::RGB, ColorSpace::YCbCr709>(
    Pixel<ColorSpace::RGB> from) {
  Pixel<ColorSpace::YCbCr709> to;
  to.luma = 0.2126 * from.red + 0.7152 * from.green + 0.0722 * from.blue;
  to.blue_diff = (from.blue - to.luma) / 1.8556;
  to.red_diff = (from.red - to.luma) / 1.5748;
  return to;
}

//From YCbCr.709 to RGB
template<>
Pixel<ColorSpace::RGB> ColorSpaceConversion<ColorSpace::YCbCr709, ColorSpace::RGB>(
    Pixel<ColorSpace::YCbCr709> from) {
  Pixel<ColorSpace::RGB> to;
  to.red = from.luma + 1.5748 * from.red_diff;
  to.green = from.luma - (0.2126 * 1.5748 / 0.7152) * from.red_diff - (0.0722 * 1.8556 / 0.7152) * from.blue_diff;
  to.blue = from.luma + 1.8556 * from.blue_diff;
  return to;
}

//From RGB to YCoCg
template<>
Pixel<ColorSpace::YCoCg> ColorSpaceConversion<ColorSpace::RGB, ColorSpace::YCoCg>(
    Pixel<ColorSpace::RGB> from) {
  Pixel<ColorSpace::YCoCg> to;
  to.luma = from.red / 4 + from.green / 2 + from.blue / 4;
  to.chroma_orange = from.red / 2 - from.blue / 2;
  to.chroma_green = from.green / 2 - from.red / 4 - from.blue / 4;
  return to;
}

//From YCoCg to RGB
template<>
Pixel<ColorSpace::RGB> ColorSpaceConversion<ColorSpace::YCoCg, ColorSpace::RGB>(
    Pixel<ColorSpace::YCoCg> from) {
  Pixel<ColorSpace::RGB> to;
  to.red = from.luma + from.chroma_orange - from.chroma_green;
  to.green = from.luma + from.chroma_green;
  to.blue = from.luma - from.chroma_orange - from.chroma_green;
  return to;
}

template <>
bool operator==<ColorSpace::RGB>(const Pixel<ColorSpace::RGB>& p1,
                                 const Pixel<ColorSpace::RGB>& p2) {
  return compare(p1.red, p2.red) && compare(p1.green, p2.green) &&
         compare(p1.blue, p2.blue);
}

}  // namespace server::core::pnm::color_space