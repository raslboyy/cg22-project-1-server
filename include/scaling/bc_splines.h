#ifndef SERVICE_TEMPLATE_BC_SPLINES_H
#define SERVICE_TEMPLATE_BC_SPLINES_H

#include "shifting.h"
#include "type.h"

#include <vector>

namespace server::core::scaling {

using color_space::ColorSpace;

template <>
class Scaling<Algorithm::BcSplines> {
 public:
  explicit Scaling(uint32_t width, uint32_t height = 0, Shift shift = Shift(),
                   double B = 0., double C = 0.5)
      : width_(width), height_(height), shift_(shift), B(B), C(C) {
    assert(width > 0);
  }

  PNM<ColorSpace::RGB> operator()(const PNM<ColorSpace::RGB>&) const;

 private:
  uint32_t width_;
  uint32_t height_;
  Shift shift_;
  const double B;
  const double C;

  color_space::Pixel<ColorSpace::RGB> Interpolate(
      std::vector<color_space::Pixel<ColorSpace::RGB>>&, double d) const;
};

}  // namespace server::core::scaling

#endif  // SERVICE_TEMPLATE_BC_SPLINES_H
