#include "histogram/histogram.h"

#include <vector>

// не стал даже добавлять возвращаемый тип потому что скорее всего есть более
// адекватная структура для хранения чем статический двумерный массив
namespace server::core::histogram {
std::vector<std::vector<int>> CalculateFrequencies(PNM<ColorSpace::RGB> image) {
  std::vector<std::vector<int>> freq(3, std::vector<int>(256));
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 256; j++) {
      freq[i][j] = 0;
    }
  int n = (int)image.height();
  int m = (int)image.width();
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) {
      freq[0][(int)std::round(image[i][j].red * 255)]++;
      freq[1][(int)std::round(image[i][j].green * 255)]++;
      freq[2][(int)std::round(image[i][j].blue * 255)]++;
    }
  return freq;
}

PNM<ColorSpace::RGB> ContrastCorrection(PNM<ColorSpace::RGB> image,
                                        double ignore) {
  auto freq = CalculateFrequencies(image);
  double min_coef = 256;
  for (int i = 0; i < 3; i++) {
    double coef =
        255. / (freq[i][255 - int(ignore * 255)] - freq[i][int(ignore * 255)]);
    if (coef < min_coef) min_coef = coef;
  }
  int n = (int)image.height();
  int m = (int)image.width();
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) {
      image[i][j] = min_coef * image[i][j];
    }
  return image;
}
}  // namespace server::core::histogram