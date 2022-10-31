#include "pixel.h"

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

static channel get_saturation(channel R, channel G, channel B) {}

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

  return {};
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

  return {};
}

// From HSL to RGB
template <>
Pixel<ColorSpace::RGB> ColorSpaceConversion<ColorSpace::HSL, ColorSpace::RGB>(
    Pixel<ColorSpace::HSL> from) {

}

template <>
bool operator==<ColorSpace::RGB>(const Pixel<ColorSpace::RGB>& p1,
                                 const Pixel<ColorSpace::RGB>& p2) {
  return compare(p1.red, p2.red) && compare(p1.green, p2.green) &&
         compare(p1.blue, p2.blue);
}

}  // namespace server::core::pnm::color_space