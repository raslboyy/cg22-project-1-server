#include "body.h"

namespace server::core::pnm {
//template <>
//Body<color_space::ColorSpace::NONE>::Body(bytes&& buffer, uint32_t width,
//                                          uint32_t height)
//    : width(width), height(height) {
//  pixels.reserve(buffer.size());
//  for (size_t i = 0; i != buffer.size(); i++)
//    pixels.emplace_back(
//        color_space::Pixel<color_space::ColorSpace::NONE>{buffer[i]});
//}
//template <>
//Body<color_space::ColorSpace::RGB>::Body(bytes&& buffer, uint32_t width,
//                                         uint32_t height)
//    : width(width), height(height) {
//  pixels.reserve(buffer.size() / 3);
//  for (size_t i = 0; i != buffer.size(); i += 3)
//    pixels.emplace_back(color_space::Pixel<color_space::ColorSpace::RGB>(
//        buffer[i], buffer[i + 1], buffer[i + 2]));
//}
//
//template <color_space::ColorSpace From, color_space::ColorSpace To>
//Body<To> ColorSpaceConversion(Body<From> from) {
//  Body<To> to;
//  to.width = from.width;
//  to.height = from.height;
//  to.pixels = std::vector<color_space::Pixel<To>>();
//  for (size_t i = 0; i != from.pixels.size(); i++)
//    to.pixels.template emplace_back(
//        color_space::ColorSpaceConversion<From, To>(from.pixels[i]));
//}
//
//template struct Body<color_space::ColorSpace::NONE>;
//template struct Body<color_space::ColorSpace::RGB>;
}  // namespace server::core::pnm
