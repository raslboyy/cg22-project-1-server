#include "scaling/bi_linear.h"

#include "utils/useful.h"

#include <iostream>

namespace server::core::scaling {

using namespace color_space;

PNM<ColorSpace::RGB> Scaling<Algorithm::BiLinear>::operator()(
    const PNM<ColorSpace::RGB>& image) const {
  auto height = height_;
  auto width = width_;
  if (height == 0)
    height = (uint32_t)std::round(image.height() * width / image.width());
  assert(height > 0);

  PNM<ColorSpace::RGB> scaled(width, height);

  for (int i = 0; i < (int)height; i++)
    for (int j = 0; j < (int)width; j++) {
      double y = i * (double)image.height() / height;
      double x = j * (double)image.width() / width;

      auto x1 = std::min((int)std::floor(x), (int)image.width() - 1);
      auto x2 = std::min((int)std::ceil(x), (int)image.width() - 1);
      auto y1 = std::min((int)std::floor(y), (int)image.height() - 1);
      auto y2 = std::min((int)std::ceil(y), (int)image.height() - 1);

      auto q11 = image[y1][x1];
      auto q12 = image[y1][x2];
      auto q21 = image[y2][x1];
      auto q22 = image[y2][x2];

      auto p1 = (x2 - x) * q11 + (x - x1) * q12;
      auto p2 = (x2 - x) * q21 + (x - x1) * q22;

      if (utils::DoubleCompare(x1, x2)) {
        p1 = q11;
        p2 = q22;
      }

      auto p = (y2 - y) * p1 + (y - y1) * p2;

      if (utils::DoubleCompare(y1, y2)) p = p1;

      p.red = std::min(1., p.red);
      p.green = std::min(1., p.green);
      p.blue = std::min(1., p.blue);

      scaled[i][j] = p;
    }

  return shift_(scaled);
}

}  // namespace server::core::scaling
