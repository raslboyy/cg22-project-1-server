#include "dithering/random.h"

namespace server::core::dithering {

PNM<ColorSpace::NONE>
Dithering<Algorithm::Random, ColorSpace::NONE>::operator()(
    PNM<ColorSpace::NONE> image) const {
  int k = 1 << bitness_;
  int n = (int)image.height();
  int m = (int)image.width();
  int max = std::numeric_limits<byte>::max();
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) {
      std::vector<byte> borders(k - 1);
      for (int ii = 0; ii < k - 1; ii++)
        borders[ii] = userver::utils::RandRange(0, max + 1);
      borders.emplace_back(255);
      std::sort(borders.begin(), borders.end());

      size_t ii =
          std::lower_bound(borders.begin(), borders.end(), image[i][j].scale) -
          borders.begin();

      if ((int)ii == 0)
        ii = 0;
      else
        ii = borders[ii];

      assert(0 <= ii && (int)ii <= max);
      image[i][j].scale = ii;
    }

  return image;
}

}  // namespace server::core::dithering