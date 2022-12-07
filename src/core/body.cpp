#include "body.h"
#include "pixel.h"

namespace server::core::pnm {

template <>
Body<color_space::ColorSpace::NONE>::Body(bytes&& buffer, uint32_t width,
                                          uint32_t height)
    : width(width), height(height) {
  pixels.reserve(buffer.size());
  for (size_t i = 0; i < buffer.size(); i++)
    pixels.emplace_back(
        color_space::Pixel<color_space::ColorSpace::NONE>{buffer[i]});
}

template <color_space::ColorSpace colorSpace>
Body<colorSpace>::Body(bytes&& buffer, uint32_t width,
                       uint32_t height)
    : width(width), height(height) {
  if (buffer.size() % 3 != 0)
    throw std::exception();
  pixels.reserve(buffer.size() / 3);
  for (size_t i = 0; i < buffer.size(); i += 3) {
    pixels.emplace_back(color_space::ConstructPixel<colorSpace>(buffer[i], buffer[i + 1], buffer[i + 2]));
  }
}

template struct Body<color_space::ColorSpace::NONE>;
template struct Body<color_space::ColorSpace::RGB>;
template struct Body<color_space::ColorSpace::HSL>;
template struct Body<color_space::ColorSpace::HSV>;
template struct Body<color_space::ColorSpace::CMY>;
template struct Body<color_space::ColorSpace::YCbCr601>;
template struct Body<color_space::ColorSpace::YCbCr709>;
template struct Body<color_space::ColorSpace::YCoCg>;
}  // namespace server::core::pnm
