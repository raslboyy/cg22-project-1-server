#include "jpeg/idct.h"

#include <cmath>

namespace {
static double NormCoeff(int n) {
  if (n == 0)
    return 1. / std::sqrt(2.);
  else
    return 1.;
}
}  // namespace

namespace server::core::jpeg {
Idct::Idct()
    : base(64),
      zig_zag({{0, 1, 5, 6, 14, 15, 27, 28},
               {2, 4, 7, 13, 16, 26, 29, 42},
               {3, 8, 12, 17, 25, 30, 41, 43},
               {9, 11, 18, 24, 31, 40, 44, 53},
               {10, 19, 23, 32, 39, 45, 52, 54},
               {20, 22, 33, 38, 46, 51, 55, 60},
               {21, 34, 37, 47, 50, 56, 59, 61},
               {35, 36, 48, 49, 57, 58, 62, 63}}),
      idct_table(8, std::vector<double>(8)),
      idct_precision(8) {
  for (int u = 0; u < idct_precision; u++)
    for (int x = 0; x < idct_precision; x++)
      idct_table[u][x] =
          NormCoeff(u) * std::cos(((2.0 * x + 1.0) * u * M_PI) / 16.0);
}

std::vector<std::vector<int>> Idct::RearrangeUsingZigzag() {
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++) zig_zag[x][y] = base[zig_zag[x][y]];
  return zig_zag;
}
void Idct::PerformIdct() {
  std::vector<std::vector<double>> out(8, std::vector<double>(8));

  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++) {
      double local_sum = 0;
      for (int u = 0; u < idct_precision; u++)
        for (int v = 0; v < idct_precision; v++)
          local_sum += (zig_zag[v][u] * idct_table[u][x] * idct_table[v][y]);
      out[x][y] = int(std::round(local_sum)) / 4;
      base[x * 8 + y] = out[x][y];
    }
}
}  // namespace server::core::jpeg
