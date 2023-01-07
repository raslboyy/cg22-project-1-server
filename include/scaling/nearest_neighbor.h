#ifndef SERVICE_TEMPLATE_NEAREST_NEIGHBOR_H
#define SERVICE_TEMPLATE_NEAREST_NEIGHBOR_H

#include "shifting.h"
#include "type.h"

namespace server::core::scaling {

using color_space::ColorSpace;

template <>
class Scaling<Algorithm::NearestNeighbor> {
 public:
  explicit Scaling(uint32_t width, uint32_t height = 0, Shift shift = Shift())
      : width_(width), height_(height), shift_(shift) {
    assert(width > 0);
  }

  PNM<ColorSpace::RGB> operator()(const PNM<ColorSpace::RGB>& image) const;

 private:
  uint32_t width_;
  uint32_t height_;
  Shift shift_;
};

}  // namespace server::core::scaling

#endif  // SERVICE_TEMPLATE_NEAREST_NEIGHBOR_H
