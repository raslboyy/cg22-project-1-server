#include "dithering/atkinson.h"

namespace server::core::dithering {
const std::vector<std::pair<int, int>>
    Dithering<Algorithm::Atkinson, ColorSpace::NONE>::neighbors{
        {0, 1}, {0, 2}, {1, 1}, {1, 0}, {2, 0}, {-1, -1}};

PNM<ColorSpace::NONE>
Dithering<Algorithm::Atkinson, ColorSpace::NONE>::operator()(
    PNM<ColorSpace::NONE> image) {
  int n = (int)image.height();
  int m = (int)image.width();
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) {
      auto old_scale = image[i][j].scale;
      auto new_scale = FindClosest(old_scale);
      image[i][j].scale = new_scale;
      double quant_error = old_scale - new_scale;

      for (auto [ii, jj] : neighbors)
        if (0 <= i + ii && i + ii < n && 0 <= j + jj && j + jj < m)
          image[i + ii][j + jj].scale =
              ApplyQuantError(image[i + ii][j + jj].scale, quant_error, 1. / 8);
    }

  return image;
}

byte Dithering<Algorithm::Atkinson, ColorSpace::NONE>::FindClosest(
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
