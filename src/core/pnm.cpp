#include <cctype>
#include <cwctype>

#include "pnm.h"
#include "userver/logging/log.hpp"

namespace server::core::pnm {
using namespace color_space;
Header::Header(bytes type, uint32_t width, uint32_t height,
               uint32_t max_color_value) {
  if (type.size() != 2) throw std::exception();
  if (type[0] != 'P') throw std::exception();
  if (type[1] == '5') {
    this->type = PnmType::P5;
  } else if (type[1] == '6') {
    this->type = PnmType::P6;
  } else {
    throw std::exception();
  }

  if (max_color_value != 255) throw std::exception();
  this->max_color_value = max_color_value;
  this->width = width;
  this->height = height;
}

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
    pixels.emplace_back(ConstructPixel<colorSpace>(buffer[i], buffer[i + 1], buffer[i + 2]));
  }
}

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

template struct Body<color_space::ColorSpace::NONE>;
template struct Body<color_space::ColorSpace::RGB>;
template struct Body<color_space::ColorSpace::HSL>;

template <color_space::ColorSpace colorSpace>
PNM<colorSpace>::PNM(bytes&& buffer) {
  LOG_DEBUG() << buffer;
  bytes type_ = {buffer[0], buffer[1]};
  size_t cursor = 2;
  cursor_skip_whitespaces(cursor, buffer);
  uint32_t width = read_int(cursor, buffer);
  uint32_t height = read_int(cursor, buffer);
  uint32_t max_color_value = read_int(cursor, buffer);
  header_ = Header(type_, width, height, max_color_value);
  cursor_skip_whitespaces(cursor, buffer);

  if (width == 0 || height == 0) return;

  // TODO: move, dont copy
  auto body = bytes{buffer.begin() + cursor, buffer.end()};
  body_ = Body<colorSpace>{std::move(body), width,
                           height};
}
template <color_space::ColorSpace colorSpace>
int32_t PNM<colorSpace>::read_int(size_t& cursor, const bytes& buffer) {
  std::string to_int;
  while (std::isdigit(buffer[cursor])) to_int += buffer[cursor++];
  cursor_skip_whitespaces(cursor, buffer);
  return std::stoi(to_int);
}
template <color_space::ColorSpace colorSpace>
void PNM<colorSpace>::cursor_skip_whitespaces(size_t& cursor,
                                              const bytes& buffer) {
  while (std::iswspace(buffer[cursor])) cursor++;
}
template <color_space::ColorSpace colorSpace>
PNM<colorSpace>::PNM(const Header& header, const Body<colorSpace>& body)
    : header_(header), body_(body) {}

template <color_space::ColorSpace From, color_space::ColorSpace To>
PNM<To> ColorSpaceConversion(PNM<From> from) {
  PNM<To> to;
  to.header_ = from.header_;
  to.body_ = ColorSpaceConversion<From, To>(from.body_);
  return to;
}

template class PNM<color_space::ColorSpace::NONE>;
template class PNM<color_space::ColorSpace::RGB>;
template class PNM<color_space::ColorSpace::HSL>;
template PNM<ColorSpace::RGB> ColorSpaceConversion(PNM<ColorSpace::HSL>);
template PNM<ColorSpace::HSL> ColorSpaceConversion(PNM<ColorSpace::RGB>);

}  // namespace server::core::pnm