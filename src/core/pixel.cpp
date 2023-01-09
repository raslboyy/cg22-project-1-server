#include "core/pixel.h"

#include <algorithm>
#include <cmath>

#include <iostream>

namespace server::core::pnm::color_space {

Pixel<ColorSpace::RGB> operator*(double lhs, Pixel<ColorSpace::RGB> pixel) {
  pixel.red *= lhs;
  pixel.green *= lhs;
  pixel.blue *= lhs;
  return pixel;
}

Pixel<ColorSpace::RGB> operator+(Pixel<ColorSpace::RGB> lhs,
                                 const Pixel<ColorSpace::RGB>& rhs) {
  lhs.red += rhs.red;
  lhs.green += rhs.green;
  lhs.blue += rhs.blue;
  return lhs;
}

Pixel<ColorSpace::RGB> operator+(Pixel<ColorSpace::RGB> lhs, double rhs) {
  lhs.red += rhs;
  lhs.green += rhs;
  lhs.blue += rhs;
  return lhs;
}

Pixel<ColorSpace::RGB> Pixel<ColorSpace::RGB>::white(255, 255, 255);

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
Pixel<ColorSpace::HSL>
ColorSpaceConversion<ColorSpace::RGB, ColorSpace::HSL>::operator()(
    const Pixel<ColorSpace::RGB>& from) {
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
Pixel<ColorSpace::HSV>
ColorSpaceConversion<ColorSpace::RGB, ColorSpace::HSV>::operator()(
    const Pixel<ColorSpace::RGB>& from) {
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

// From HSV to RGB
Pixel<ColorSpace::RGB>
ColorSpaceConversion<ColorSpace::HSV, ColorSpace::RGB>::operator()(
    const Pixel<ColorSpace::HSV>& in) {
  double hh, p, q, t, ff;
  long i;
  Pixel<ColorSpace::RGB> out;
  auto value = in.value;

  if (in.saturation <= 0.0) {  // < is bogus, just shuts up warnings
    out.red = value;
    out.green = value;
    out.blue = value;
    return out;
  }
  hh = in.hue;
  if (hh >= 360.0) hh = 0.0;
  hh /= 60.0;
  i = (long)hh;
  ff = hh - i;
  p = value * (1.0 - in.saturation);
  q = value * (1.0 - (in.saturation * ff));
  t = value * (1.0 - (in.saturation * (1.0 - ff)));

  switch (i) {
    case 0:
      out.red = value;
      out.green = t;
      out.blue = p;
      break;
    case 1:
      out.red = q;
      out.green = value;
      out.blue = p;
      break;
    case 2:
      out.red = p;
      out.green = value;
      out.blue = t;
      break;

    case 3:
      out.red = p;
      out.green = q;
      out.blue = value;
      break;
    case 4:
      out.red = t;
      out.green = p;
      out.blue = value;
      break;
    case 5:
    default:
      out.red = value;
      out.green = p;
      out.blue = q;
      break;
  }
  return out;
}

// From HSL to RGB
Pixel<ColorSpace::RGB>
ColorSpaceConversion<ColorSpace::HSL, ColorSpace::RGB>::operator()(
    const Pixel<ColorSpace::HSL>& from) {
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

// From CMY to RGB
Pixel<ColorSpace::RGB>
ColorSpaceConversion<ColorSpace::CMY, ColorSpace::RGB>::operator()(
    const Pixel<ColorSpace::CMY>& from) {
  Pixel<ColorSpace::RGB> to;
  to.red = 1 - from.cian;
  to.green = 1 - from.magenta;
  to.blue = 1 - from.yellow;
  return to;
}

// From RGB to CMY
Pixel<ColorSpace::CMY>
ColorSpaceConversion<ColorSpace::RGB, ColorSpace::CMY>::operator()(
    const Pixel<ColorSpace::RGB>& from) {
  Pixel<ColorSpace::CMY> to;
  to.cian = 1 - from.red;
  to.magenta = 1 - from.green;
  to.yellow = 1 - from.blue;
  return to;
}

// From YCbCr601 to RGB
Pixel<ColorSpace::RGB>
ColorSpaceConversion<ColorSpace::YCbCr601, ColorSpace::RGB>::operator()(
    const Pixel<ColorSpace::YCbCr601>& from) {
  Pixel<ColorSpace::RGB> to;
  to.red = from.luma + 1.402 * from.red_diff;
  to.green = from.luma - (0.299 * 1.402 / 0.587) * from.red_diff -
             (0.114 * 1.772 / 0.587) * from.blue_diff;
  to.blue = from.luma + 1.772 * from.blue_diff;
  return to;
}

// From RGB to YCbCr601
Pixel<ColorSpace::YCbCr601>
ColorSpaceConversion<ColorSpace::RGB, ColorSpace::YCbCr601>::operator()(
    const Pixel<ColorSpace::RGB>& from) {
  Pixel<ColorSpace::YCbCr601> to;
  to.luma = 0.299 * from.red + 0.587 * from.green + 0.114 * from.blue;
  to.blue_diff = (from.blue - to.luma) / 1.772;
  to.red_diff = (from.red - to.luma) / 1.402;
  return to;
}

// From YCbCr709 to RGB
Pixel<ColorSpace::RGB>
ColorSpaceConversion<ColorSpace::YCbCr709, ColorSpace::RGB>::operator()(
    const Pixel<ColorSpace::YCbCr709>& from) {
  Pixel<ColorSpace::RGB> to;
  to.red = from.luma + 1.5748 * from.red_diff;
  to.green = from.luma - (0.2126 * 1.5748 / 0.7152) * from.red_diff -
             (0.0722 * 1.8556 / 0.7152) * from.blue_diff;
  to.blue = from.luma + 1.8556 * from.blue_diff;
  return to;
}

// From RGB to YCbCr709
Pixel<ColorSpace::YCbCr709>
ColorSpaceConversion<ColorSpace::RGB, ColorSpace::YCbCr709>::operator()(
    const Pixel<ColorSpace::RGB>& from) {
  Pixel<ColorSpace::YCbCr709> to;
  to.luma = 0.2126 * from.red + 0.7152 * from.green + 0.0722 * from.blue;
  to.blue_diff = (from.blue - to.luma) / 1.8556;
  to.red_diff = (from.red - to.luma) / 1.5748;
  return to;
}

// From YCoCg to RGB
Pixel<ColorSpace::RGB>
ColorSpaceConversion<ColorSpace::YCoCg, ColorSpace::RGB>::operator()(
    const Pixel<ColorSpace::YCoCg>& from) {
  Pixel<ColorSpace::RGB> to;
  to.red = from.luma + from.chroma_orange - from.chroma_green;
  to.green = from.luma + from.chroma_green;
  to.blue = from.luma - from.chroma_orange - from.chroma_green;
  return to;
}

// From RGB to YCoCg
Pixel<ColorSpace::YCoCg>
ColorSpaceConversion<ColorSpace::RGB, ColorSpace::YCoCg>::operator()(
    const Pixel<ColorSpace::RGB>& from) {
  Pixel<ColorSpace::YCoCg> to;
  to.luma = from.red / 4 + from.green / 2 + from.blue / 4;
  to.chroma_orange = from.red / 2 - from.blue / 2;
  to.chroma_green = from.green / 2 - from.red / 4 - from.blue / 4;
  return to;
}

template <>
bool operator==<ColorSpace::RGB>(const Pixel<ColorSpace::RGB>& p1,
                                 const Pixel<ColorSpace::RGB>& p2) {
  return compare(p1.red, p2.red) && compare(p1.green, p2.green) &&
         compare(p1.blue, p2.blue);
}

static byte AlphaBlending(byte back, byte front, double alpha) {
  int value =
      (int)std::round((uint32_t)back * (1. - alpha) + (uint32_t)front * alpha);
  value = std::max(value, 0);
  value = std::min(value, 255);
  return value;
}

template <>
Pixel<ColorSpace::NONE> AlphaBlending(const Pixel<ColorSpace::NONE>& background,
                                      const Pixel<ColorSpace::NONE>& foreground,
                                      double alpha) {
  return {AlphaBlending(background.scale, foreground.scale, alpha)};
}

template <>
Pixel<ColorSpace::RGB> AlphaBlending(const Pixel<ColorSpace::RGB>& background,
                                     const Pixel<ColorSpace::RGB>& foreground,
                                     double alpha) {
  return {AlphaBlending(background.red * 255, foreground.red * 255, alpha),
          AlphaBlending(background.green * 255, foreground.green * 255, alpha),
          AlphaBlending(background.blue * 255, foreground.blue * 255, alpha)};
}

}  // namespace server::core::pnm::color_space