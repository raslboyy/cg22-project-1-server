#include "scaling/nearest_neighbor.h"

#include <iostream>

namespace server::core::scaling {

PNM<ColorSpace::RGB> Scaling<Algorithm::NearestNeighbor>::operator()(
    const PNM<ColorSpace::RGB>& image) const {
  auto height = height_;
  auto width = width_;
  if (height == 0)
    height = (uint32_t)std::round(image.height() * width / image.width());
  assert(height > 0);

  PNM<ColorSpace::RGB> scaled(width, height);

  for (int i = 0; i < (int)height; i++)
    for (int j = 0; j < (int)width; j++) {
      int x = std::min((int)std::round(i * (double)image.height() / height),
                       (int)image.height() - 1);
      int y = std::min((int)std::round(j * (double)image.width() / width),
                       (int)image.width() - 1);
      scaled[i][j] = image[x][y];
    }

  return shift_(scaled);
}

}  // namespace server::core::scaling