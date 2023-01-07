#ifndef SERVICE_TEMPLATE_USEFUL_H
#define SERVICE_TEMPLATE_USEFUL_H

#include <cmath>

namespace server::core::utils {
inline bool DoubleCompare(double a, double b) {
  static const double eps = 1e-6;
  return std::abs(a - b) < eps;
}
}  // namespace server::core::utils

#endif  // SERVICE_TEMPLATE_USEFUL_H
