#include "dithering/floyd_steinberg.h"

namespace server::core::dithering {

PNM<ColorSpace::NONE>
Dithering<Algorithm::FloydSteinberg, ColorSpace::NONE>::operator()(
    PNM<ColorSpace::NONE> image) {
  int n = (int)image.height();
  int m = (int)image.width();
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) {
      auto old_scale = image[i][j].scale;
      auto new_scale = FindClosest(old_scale);
      image[i][j].scale = new_scale;
      double quant_error = old_scale - new_scale;

      if (j + 1 < m)
        image[i][j + 1].scale =
            ApplyQuantError(image[i][j + 1].scale, quant_error, 7. / 16);

      if (i + 1 < n && j - 1 >= 0)
        image[i + 1][j - 1].scale =
            ApplyQuantError(image[i + 1][j - 1].scale, quant_error, 3. / 16);

      if (i + 1 < n)
        image[i + 1][j].scale =
            ApplyQuantError(image[i + 1][j].scale, quant_error, 5. / 16);

      if (i + 1 < n && j + 1 < m)
        image[i + 1][j + 1].scale =
            ApplyQuantError(image[i + 1][j + 1].scale, quant_error, 1. / 16);
    }

  return image;
}

byte Dithering<Algorithm::FloydSteinberg, ColorSpace::NONE>::FindClosest(
    byte old) const {
  static const auto max = std::numeric_limits<byte>::max();
  int k = (1 << bitness_);
  std::vector<int> borders;
  for (int i = 0; i < k; i++)
    borders.emplace_back(std::round((double)max / (k - 1) * i));

  size_t i =
      std::lower_bound(borders.begin(), borders.end(), old) - borders.begin();
  if ((int)i == k - 1) return max;

  if (std::abs(borders[i] - old) <= std::abs(borders[i + 1] - old))
    return borders[i];
  return borders[i + 1];
}

}  // namespace server::core::dithering