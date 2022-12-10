#include "header.h"
#include <cstdint>
#include <string>

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

static std::string ToString(const PnmType& type) {
  switch (type) {
    case PnmType::P5:
      return "P5";
    case PnmType::P6:
      return "P6";
  }
}

bytes Header::GetRaw() const {
  std::string raw;
  raw += ToString(type) + " ";
  raw += std::to_string(width) + " ";
  raw += std::to_string(height) + " ";
  raw += "255 ";
  return {raw.begin(), raw.end()};
}

}  // namespace server::core::pnm