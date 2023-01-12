#pragma once

#include <vector>

namespace server::core::jpeg {
class Idct {
 public:
  Idct();

  std::vector<int> base;

  std::vector<std::vector<int>> RearrangeUsingZigzag();
  void PerformIdct();

 private:
  std::vector<std::vector<int>> zig_zag;
  std::vector<std::vector<double>> idct_table;
  int idct_precision;
};
}  // namespace server::core::jpeg
