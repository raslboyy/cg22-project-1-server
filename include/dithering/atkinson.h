#ifndef SERVICE_TEMPLATE_ATKINSON_H
#define SERVICE_TEMPLATE_ATKINSON_H

#include "type.h"

namespace server::core::dithering {

using color_space::ColorSpace;

template <>
class Dithering<Algorithm::Atkinson, ColorSpace::NONE> {
 public:
  explicit Dithering(int bitness) : bitness_(bitness) {
    assert(1 <= bitness_ && bitness_ <= 8);
  }
  PNM<ColorSpace::NONE> operator()(PNM<ColorSpace::NONE> image);

 private:
  int bitness_;
  static const std::vector<std::pair<int, int>> neighbors;

  [[nodiscard]] byte FindClosest(byte old) const;
  static byte ApplyQuantError(byte pixel, double quant_error, double k) {
    int scale = (int)std::round((double)pixel + quant_error * k);
    scale = std::max(scale, 0);
    scale = std::min(scale, (int)std::numeric_limits<byte>::max());
    return scale;
  }
};

}  // namespace server::core::dithering

#endif  // SERVICE_TEMPLATE_ATKINSON_H
