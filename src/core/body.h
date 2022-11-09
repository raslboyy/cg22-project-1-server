#ifndef SERVICE_TEMPLATE_SRC_CORE_BODY_H_
#define SERVICE_TEMPLATE_SRC_CORE_BODY_H_

#include <vector>

#include "bytes.h"
#include "pixel.h"

namespace server::core::pnm {

template <color_space::ColorSpace colorSpace>
struct Body {
  explicit Body(bytes&& buffer, uint32_t width, uint32_t height);
  Body() = default;
  uint32_t width{};
  uint32_t height{};
  std::vector<color_space::Pixel<colorSpace>> pixels{};
  friend bool operator==(const Body<colorSpace>& a, const Body<colorSpace>& b) {
    return a.pixels == b.pixels;
  };
};

template <color_space::ColorSpace From, color_space::ColorSpace To>
Body<To> ColorSpaceConversion(Body<From> from) {
  Body<To> to;
  to.width = from.width;
  to.height = from.height;
  to.pixels = std::vector<color_space::Pixel<To>>();
  for (size_t i = 0; i != from.pixels.size(); i++)
    to.pixels.template emplace_back(
        color_space::ColorSpaceConversion<From, To>(from.pixels[i]));
  return to;
}
}  // namespace server::core::pnm

#endif  // SERVICE_TEMPLATE_SRC_CORE_BODY_H_
