#include "dithering/ordered.h"

namespace server::core::dithering {

PNM<ColorSpace::NONE>
Dithering<Algorithm::Ordered, ColorSpace::NONE>::operator()(
    PNM<ColorSpace::NONE> image) {
  auto bayer = GetBayer();
  int bayer_n = (int)bayer.size();
  int n = (int)image.height();
  int m = (int)image.width();
  int max = std::numeric_limits<byte>::max();
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      image[i][j].scale =
          (double)image[i][j].scale / max <= bayer[i % bayer_n][j % bayer_n]
              ? 0
              : 255;

  return image;
}

std::vector<std::vector<double>>
Dithering<Algorithm::Ordered, ColorSpace::NONE>::GetBayer() const {
  static const std::vector<std::vector<double>> bayer2{{0., 0.5}, {0.75, 0.25}};

  auto bayer = bayer2;
  for (int ii = 1; ii <= level_; ii++) {
    int n = 1 << (ii + 1);
    int m = 1 << (ii);
    int nn = n * n;
    std::vector<std::vector<double>> new_bayer(n, std::vector<double>(n));
    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++) {
        int offset = 0;
        if (i / m == 1 && j / m == 1) offset = 1;
        if (i / m == 1 && j / m == 0) offset = 3;
        if (i / m == 0 && j / m == 1) offset = 2;
        new_bayer[i][j] = (bayer[i % m][j % m] * nn + offset) / nn;
      }
    bayer = new_bayer;
  }

  return bayer;
}

}  // namespace server::core::dithering
