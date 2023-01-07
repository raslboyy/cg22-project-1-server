#ifndef SERVICE_TEMPLATE_RANDOM_H
#define SERVICE_TEMPLATE_RANDOM_H

#include "type.h"

#include <userver/utils/rand.hpp>
#include <vector>

namespace server::core::dithering {

using color_space::ColorSpace;

template <>
class Dithering<Algorithm::Random, ColorSpace::NONE> {
 public:
  explicit Dithering(int bitness) : bitness_(bitness) {
    assert(1 <= bitness && bitness <= 8);
  }
  PNM<ColorSpace::NONE> operator()(PNM<ColorSpace::NONE> image) const;

 private:
  int bitness_ = 2;
};

}  // namespace server::core::dithering

#endif  // SERVICE_TEMPLATE_RANDOM_H
