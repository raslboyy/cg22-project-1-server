#ifndef SERVICE_TEMPLATE_SRC_CORE_PIXEL_H_
#define SERVICE_TEMPLATE_SRC_CORE_PIXEL_H_

#include <cstdint>
#include "color_space.h"
#include "pnm_type.h"

namespace server::core::pnm::color_space {
using channel = double;

template <ColorSpace>
struct Pixel;
template <ColorSpace color>
bool operator==(const Pixel<color>& p1, const Pixel<color>& p2);
template <ColorSpace color>
Pixel<color> ConstructPixel(int32_t a, int32_t b, int32_t c){
  return {a, b, c};
}

template <>
struct Pixel<ColorSpace::NONE> {
  int16_t scale{};
};
template <>
struct Pixel<ColorSpace::RGB> {
  Pixel() = default;
  Pixel(int32_t r, int32_t g, int32_t b)
      : red(r / 255.0), green(g / 255.0), blue(b / 255.0) {}
  channel red{};
  channel green{};
  channel blue{};
};

template <>
struct Pixel<ColorSpace::HSL> {
  Pixel() = default;
  Pixel(int32_t h, int32_t s, int32_t l)
      : hue(h / 255.0 * 360.0), saturation(s / 255.0), lightness(l / 255.0) {}
  channel hue{};
  channel saturation{};
  channel lightness{};
};
template <>
struct Pixel<ColorSpace::HSV> {
  channel hue{};
  channel saturation{};
  channel value{};
};
template <>
struct Pixel<ColorSpace::CMY>{
  Pixel() = default;
  Pixel(int32_t c, int32_t m, int32_t y)
      : cian(c / 255.0), magenta(m / 255.0), yellow(y / 255.0) {}
  channel cian{};
  channel magenta{};
  channel yellow{};
};
template<>
struct Pixel<ColorSpace::YCbCr601>{
  Pixel() = default;
  Pixel(int32_t y, int32_t cb, int32_t cr)
      : luma(y / 255.0), blue_diff(cb / 255.0 - 0.5), red_diff(cr / 255.0 - 0.5) {}
  channel luma{};
  channel blue_diff{};
  channel red_diff{};
};
template<>
struct Pixel<ColorSpace::YCbCr709>{
  Pixel() = default;
  Pixel(int32_t y, int32_t cb, int32_t cr)
      : luma(y / 255.0), blue_diff(cb / 255.0 - 0.5), red_diff(cr / 255.0 - 0.5) {}
  channel luma{};
  channel blue_diff{};
  channel red_diff{};
};
template<>
struct Pixel<ColorSpace::YCoCg>{
  Pixel() = default;
  Pixel(int32_t y, int32_t co, int32_t cg)
      : luma(y / 255.0), chroma_orange(co / 255.0 - 0.5), chroma_green(cg / 255.0 - 0.5) {}
  channel luma{};
  channel chroma_orange{};
  channel chroma_green{};
};

template <ColorSpace From, ColorSpace To>
Pixel<To> ColorSpaceConversion(Pixel<From>);
}  // namespace server::core::pnm::color_space

#endif  // SERVICE_TEMPLATE_SRC_CORE_PIXEL_H_
