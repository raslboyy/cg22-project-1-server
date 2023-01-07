#include "scaling/bc_splines.h"

#include "utils/useful.h"

#include <cmath>
#include <iostream>
#include <vector>

namespace server::core::scaling {

using namespace color_space;

PNM<ColorSpace::RGB> Scaling<Algorithm::BcSplines>::operator()(
    const PNM<ColorSpace::RGB>& image) const {
  auto height = height_;
  auto width = width_;
  if (height == 0)
    height = (uint32_t)std::round(image.height() * width / image.width());
  assert(height > 0);

  PNM<ColorSpace::RGB> scaled(width, height);
  auto max = std::numeric_limits<byte>::max();

  for (int i = 0; i < (int)height; i++)
    for (int j = 0; j < (int)width; j++) {
      double x = j * (double)image.width() / width;
      double y = i * (double)image.height() / height;
      int x_ = (int)std::floor(x);
      int y_ = (int)std::floor(y);

      std::vector<Pixel<ColorSpace::RGB>> P;
      for (int dx = -1; dx <= 2; dx++) {
        if (x_ + dx < 0) {
          P.emplace_back(max, max, max);
          continue;
        }
        if (x_ + dx >= (int)image.width()) {
          P.emplace_back(max, max, max);
          continue;
        }
        std::vector<Pixel<ColorSpace::RGB>> p;
        for (int dy = -1; dy <= 2; dy++) {
          if (y_ + dy < 0) {
            p.emplace_back(max, max, max);
            continue;
          }
          if (y_ + dy >= (int)image.height()) {
            p.emplace_back(max, max, max);
            continue;
          }
          p.emplace_back(image[y_ + dy][x_ + dx]);
        }
        P.emplace_back(Interpolate(p, (y - y_)));
      }

      scaled[i][j] = Interpolate(P, (x - x_));

      scaled[i][j].red = std::max(0., std::min(1., scaled[i][j].red));
      scaled[i][j].green = std::max(0., std::min(1., scaled[i][j].green));
      scaled[i][j].blue = std::max(0., std::min(1., scaled[i][j].blue));
    }

  return shift_(scaled);
}

Pixel<ColorSpace::RGB> Scaling<Algorithm::BcSplines>::Interpolate(
    std::vector<Pixel<ColorSpace::RGB>>& p, double d) const {
  assert(p.size() == 4);
  auto a1 = (-B / 6. - C) * p[0] + (-3 * B / 2 - C + 2) * p[1] +
            (3 * B / 2 + C - 2) * p[2] + (B / 6 + C) * p[3];
  auto a2 = (B / 2. + 2 * C) * p[0] + (2 * B + C - 3) * p[1] +
            (-5. * B / 2. - 2 * C + 3) * p[2] + (-C) * p[3];
  auto a3 = (-B / 2. - C) * p[0] + (B / 2 + C) * p[2];
  auto a4 = B / 6. * p[0] + (-B / 3. + 1) * p[1] + B / 6. * p[2];
  return std::pow(d, 3) * a1 + std::pow(d, 2) * a2 + std::pow(d, 1) * a3 + a4;
}

}  // namespace server::core::scaling