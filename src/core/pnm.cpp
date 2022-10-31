#include <cctype>
#include <cwctype>

#include "pnm.h"

namespace server::core::pnm {
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

template <ColorSpace colorSpace>
PNM<colorSpace>::PNM(bytes&& buffer) {
  bytes type_ = {buffer[0], buffer[1]};
  size_t cursor = 2;
  cursor_skip_whitespaces(cursor, buffer);
  uint32_t width = read_int(cursor, buffer);
  uint32_t height = read_int(cursor, buffer);
  uint32_t max_color_value = read_int(cursor, buffer);
  header_ = {type_, width, height, max_color_value};
  cursor_skip_whitespaces(cursor, buffer);

  if (width == 0 || height == 0) return;

  // TODO: move, dont copy
  body_ = Body<colorSpace>{bytes{buffer.begin() + cursor, buffer.end()}, width,
                           height};
}
template <ColorSpace colorSpace>
int32_t PNM<colorSpace>::read_int(size_t& cursor, const bytes& buffer) {
  std::string to_int;
  while (std::isdigit(buffer[cursor])) to_int += buffer[cursor++];
  cursor_skip_whitespaces(cursor, buffer);
  return std::stoi(to_int);
}
template <ColorSpace colorSpace>
void PNM<colorSpace>::cursor_skip_whitespaces(size_t& cursor,
                                              const bytes& buffer) {
  while (std::iswspace(buffer[cursor])) cursor++;
}
template <ColorSpace colorSpace>
PNM<colorSpace>::PNM(const Header& header, const Body<colorSpace>& body)
    : header_(header), body_(body) {}

template <>
Body<ColorSpace::NONE>::Body(bytes&& buffer, uint32_t width, uint32_t height)
    : width(width), height(height) {
  pixels.reserve(buffer.size());
  for (size_t i = 0; i != buffer.size(); i++)
    pixels.emplace_back(Pixel<ColorSpace::NONE>{buffer[i]});
}
template <>
Body<ColorSpace::RGB>::Body(bytes&& buffer, uint32_t width, uint32_t height)
    : width(width), height(height) {
  pixels.reserve(buffer.size() / 3);
  for (size_t i = 0; i != buffer.size(); i += 3)
    pixels.emplace_back(
        Pixel<ColorSpace::RGB>{buffer[i], buffer[i + 1], buffer[i + 2]});
}

template struct Body<ColorSpace::NONE>;
template struct Body<ColorSpace::RGB>;
template class PNM<ColorSpace::NONE>;
template class PNM<ColorSpace::RGB>;

}  // namespace server::core::pnm