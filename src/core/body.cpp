#include "body.h"
#include "pixel.h"

#include <stdexcept>

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
Body<colorSpace>::Body(bytes&& buffer, uint32_t width, uint32_t height)
    : width(width), height(height) {
  if (buffer.size() % 3 != 0) throw std::exception();
  pixels.reserve(buffer.size() / 3);
  for (size_t i = 0; i < buffer.size(); i += 3) {
    pixels.emplace_back(color_space::ConstructPixel<colorSpace>(
        buffer[i], buffer[i + 1], buffer[i + 2]));
  }
}

template <color_space::ColorSpace colorSpace>
bytes Body<colorSpace>::GetRaw() const {
  uint32_t n = width * height * 3;
  bytes raw(n);
  for (int i = 0; i < static_cast<int>(n); i += 3) {
    auto raw_pixel = color_space::GetRaw<colorSpace>(pixels[i / 3]);
    raw[i] = std::get<0>(raw_pixel);
    raw[i + 1] = std::get<1>(raw_pixel);
    raw[i + 2] = std::get<2>(raw_pixel);
  }
  return raw;
}

template <>
bytes Body<color_space::ColorSpace::NONE>::GetRaw() const {
  uint32_t n = width * height;
  bytes raw(n);
  for (int i = 0; i < static_cast<int>(n); i++) raw[i] = pixels[i].scale;
  return raw;
}

template struct Body<color_space::ColorSpace::NONE>;
template struct Body<color_space::ColorSpace::RGB>;
template struct Body<color_space::ColorSpace::HSL>;
}  // namespace server::core::pnm
