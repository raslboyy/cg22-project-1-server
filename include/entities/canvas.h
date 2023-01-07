#ifndef SERVICE_TEMPLATE_CANVAS_H
#define SERVICE_TEMPLATE_CANVAS_H

#include "core/pnm.h"
#include "point.h"

namespace server::entities {
using namespace server::core::pnm::color_space;
template <ColorSpace Space>
using Pnm = server::core::pnm::PNM<Space>;

template <ColorSpace Space>
class Canvas {
 public:
  // copy?
  explicit Canvas(Pnm<Space>& base) : base_(base){};
  void Plot(const Point&, const Pixel<Space>& color, double brightness = 1);

 private:
  Pnm<Space>& base_;

  bool Validate(const Point&);
};

template <ColorSpace Space>
void Canvas<Space>::Plot(const Point& point, const Pixel<Space>& color,
                         double alpha) {
  if (!Validate(point)) return;
  base_[point.x()][point.y()] =
      AlphaBlending<Space>(base_[point.x()][point.y()], color, alpha);
}

template <ColorSpace Space>
bool Canvas<Space>::Validate(const Point& point) {
  if (point.x() < 0 || point.x() >= (int)base_.height()) return false;
  if (point.y() < 0 || point.y() >= (int)base_.width()) return false;
  return true;
}

}  // namespace server::entities

#endif  // SERVICE_TEMPLATE_CANVAS_H
