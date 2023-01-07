#ifndef SERVICE_TEMPLATE_ORDERED_H
#define SERVICE_TEMPLATE_ORDERED_H

#include "type.h"

#include <iostream>
#include <vector>

namespace server::core::dithering {

using color_space::ColorSpace;

template <>
class Dithering<Algorithm::Ordered, ColorSpace::NONE> {
 public:
  explicit Dithering(int level) : level_(level) { assert(level >= 0); }
  PNM<ColorSpace::NONE> operator()(PNM<ColorSpace::NONE> image);

 private:
  int level_;

  [[nodiscard]] std::vector<std::vector<double>> GetBayer() const;
};

}  // namespace server::core::dithering

#endif  // SERVICE_TEMPLATE_ORDERED_H
