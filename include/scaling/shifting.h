#ifndef SERVICE_TEMPLATE_SHIFTING_H
#define SERVICE_TEMPLATE_SHIFTING_H

#include "core/pnm.h"
#include "type.h"

namespace server::core::scaling {

using color_space::ColorSpace;

class Shift {
 public:
  Shift() = default;
  Shift(int x, int y) : x_(x), y_(y) {}
  template <ColorSpace Space>
  PNM<Space> operator()(const PNM<Space>&) const;

 private:
  int x_{};
  int y_{};
};

template <ColorSpace Space>
PNM<Space> Shift::operator()(const PNM<Space>& image) const {
  PNM<Space> shifted(image.width(), image.height(),
                     color_space::Pixel<Space>::white);

  int n = image.height();
  int m = image.width();
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) {
      int x = i + x_;
      int y = j + y_;
      if (x < 0 || x >= n) continue;
      if (y < 0 || y >= m) continue;
      shifted[x][y] = image[i][j];
    }

  return shifted;
}

}  // namespace server::core::scaling

#endif  // SERVICE_TEMPLATE_SHIFTING_H
