#include <cctype>
#include <cwctype>

#include "pnm.h"
#include "userver/logging/log.hpp"

namespace server::core::pnm {
using namespace color_space;

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
template class PNM<color_space::ColorSpace::HSV>;
template class PNM<color_space::ColorSpace::CMY>;
template class PNM<color_space::ColorSpace::YCbCr601>;
template class PNM<color_space::ColorSpace::YCbCr709>;
template class PNM<color_space::ColorSpace::YCoCg>;
template PNM<ColorSpace::RGB> ColorSpaceConversion(PNM<ColorSpace::YCoCg>);
template PNM<ColorSpace::RGB> ColorSpaceConversion(PNM<ColorSpace::YCbCr709>);
template PNM<ColorSpace::RGB> ColorSpaceConversion(PNM<ColorSpace::YCbCr601>);
template PNM<ColorSpace::RGB> ColorSpaceConversion(PNM<ColorSpace::CMY>);
template PNM<ColorSpace::RGB> ColorSpaceConversion(PNM<ColorSpace::HSV>);
template PNM<ColorSpace::RGB> ColorSpaceConversion(PNM<ColorSpace::HSL>);
template PNM<ColorSpace::HSL> ColorSpaceConversion(PNM<ColorSpace::RGB>);
template PNM<ColorSpace::HSV> ColorSpaceConversion(PNM<ColorSpace::RGB>);
template PNM<ColorSpace::CMY> ColorSpaceConversion(PNM<ColorSpace::RGB>);
template PNM<ColorSpace::YCoCg> ColorSpaceConversion(PNM<ColorSpace::RGB>);
template PNM<ColorSpace::YCbCr601> ColorSpaceConversion(PNM<ColorSpace::RGB>);
template PNM<ColorSpace::YCbCr709> ColorSpaceConversion(PNM<ColorSpace::RGB>);

}  // namespace server::core::pnm