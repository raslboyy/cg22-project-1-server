#include "scaling/lanczos.h"

#include "utils/useful.h"

#include <cmath>
#include <iostream>
#include <vector>

namespace server::core::scaling {

using namespace color_space;

PNM<ColorSpace::RGB> Scaling<Algorithm::Lanczos>::operator()(
    const PNM<ColorSpace::RGB>& image) {
  auto height = height_;
  auto width = width_;
  if (height == 0)
    height = (uint32_t)std::round(image.height() * width / image.width());
  assert(height > 0);

  PNM<ColorSpace::RGB> scaled(width, height);

  for (int i = 0; i < (int)height; i++)
    for (int j = 0; j < (int)width; j++) {
      double x = j * (double)image.width() / width;
      double y = i * (double)image.height() / height;
      int x_ = (int)std::floor(x);
      int y_ = (int)std::floor(y);

      for (int dx = -A + 1; dx <= A; dx++) {
        if (x_ + dx < 0) continue;
        if (x_ + dx >= (int)image.width()) continue;
        for (int dy = -A + 1; dy <= A; dy++) {
          if (y_ + dy < 0) continue;
          if (y_ + dy >= (int)image.height()) continue;
          scaled[i][j] = scaled[i][j] + L(x - (x_ + dx)) * L(y - (y_ + dy)) *
                                            image[y_ + dy][x_ + dx];
        }
      }
      scaled[i][j].red = std::max(0., std::min(1., scaled[i][j].red));
      scaled[i][j].green = std::max(0., std::min(1., scaled[i][j].green));
      scaled[i][j].blue = std::max(0., std::min(1., scaled[i][j].blue));
    }
  return shift_(scaled);
}

double Scaling<Algorithm::Lanczos>::L(double x) const {
  if (utils::DoubleCompare(x, 0.)) return 1;
  if (x < -A) return 0;
  if (x >= A) return 0;
  return A * std::sin(M_PI * x) * std::sin(M_PI * x / A) / (M_PI * M_PI) /
         (x * x);
}

}  // namespace server::core::scaling