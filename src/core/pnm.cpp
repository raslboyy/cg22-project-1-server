#include <cwctype>
#include <cctype>

#include "pnm.h"

Header::Header(bytes type, uint32_t width, uint32_t height, uint32_t max_color_value) {
  if (type.size() != 2)
    throw std::exception();
  if (type[0] != 'P')
    throw std::exception();
  if (type[1] == '5') {
    this->type = PnmType::P5;
  } else if (type[1] == '6') {
    this->type = PnmType::P6;
  } else {
    throw std::exception();
  }

  if (max_color_value != 255)
    throw std::exception();
  this->max_color_value = max_color_value;
  this->width = width;
  this->height = height;
}

template<PnmType type>
PNM<type>::PNM(bytes &&buffer) {
  bytes type_ = {buffer[0], buffer[1]};
  size_t cursor = 2;
  cursor_skip_whitespaces(cursor, buffer);
  uint32_t width = read_int(cursor, buffer);
  uint32_t height = read_int(cursor, buffer);
  uint32_t max_color_value = read_int(cursor, buffer);
  header_ = {type_, width, height, max_color_value};

  if (width == 0 || height == 0)
    return;
  // TODO: move, dont copy
  body_ = Body<type>{bytes{buffer.begin() + cursor + 1, buffer.end()}, width, height};
}
template<PnmType type>
int32_t PNM<type>::read_int(size_t &cursor, const bytes &buffer) {
  std::string to_int;
  while (std::isdigit(buffer[cursor]))
    to_int += buffer[cursor++];
  cursor_skip_whitespaces(cursor, buffer);
  return std::stoi(to_int);
}
template<PnmType type>
void PNM<type>::cursor_skip_whitespaces(size_t &cursor, const bytes &buffer) {
  while (std::iswspace(buffer[cursor]))
    cursor++;
}

//TODO: optimize pixels init
template<>
Body<PnmType::P5>::Body(bytes &&buffer, uint32_t width, uint32_t height)
    : width(width), height(height) {
  pixels.reserve(buffer.size());
  for (size_t i = 0; i != buffer.size(); i++)
    pixels.emplace_back(Pixel<PnmType::P5>{buffer[i]});
}
template<>
Body<PnmType::P6>::Body(bytes &&buffer, uint32_t width, uint32_t height)
    : width(width), height(height) {
  pixels.reserve(buffer.size() / 3);
  for (size_t i = 0; i != buffer.size(); i += 3)
    pixels.emplace_back(Pixel<PnmType::P6>{buffer[i], buffer[i + 1], buffer[i + 2]});
}

template
    struct Body<PnmType::P5>;
template
    struct Body<PnmType::P6>;
template
    class PNM<PnmType::P5>;
template
    class PNM<PnmType::P6>;