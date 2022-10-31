#ifndef SERVICE_TEMPLATE_SRC_CORE_PIXEL_H_
#define SERVICE_TEMPLATE_SRC_CORE_PIXEL_H_

#include <cstdint>
#include "pnm_type.h"
#include "color_space.h"

namespace server::core::pnm {
template <ColorSpace>
struct Pixel;
template <>
struct Pixel<ColorSpace::NONE> {
  int32_t scale{};
};
template <>
struct Pixel<ColorSpace::RGB> {
  int32_t r{};
  int32_t g{};
  int32_t b{};
};
}  // namespace server::core::pnm

#endif  // SERVICE_TEMPLATE_SRC_CORE_PIXEL_H_
