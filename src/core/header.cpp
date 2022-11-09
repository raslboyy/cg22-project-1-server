#include "header.h"
#include <cstdint>

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
}  // namespace server::core::pnm