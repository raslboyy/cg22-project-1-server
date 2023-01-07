#ifndef SERVICE_TEMPLATE_BI_LINEAR_H
#define SERVICE_TEMPLATE_BI_LINEAR_H

#include "shifting.h"
#include "type.h"

namespace server::core::scaling {

using color_space::ColorSpace;

template <>
class Scaling<Algorithm::BiLinear> {
 public:
  explicit Scaling(uint32_t width, uint32_t height = 0, Shift shift = Shift())
      : width_(width), height_(height), shift_(shift) {
    assert(width > 0);
  }

  PNM<ColorSpace::RGB> operator()(const PNM<ColorSpace::RGB>&) const;

 private:
  uint32_t width_;
  uint32_t height_;
  Shift shift_;
};

}  // namespace server::core::scaling

#endif  // SERVICE_TEMPLATE_BI_LINEAR_H
