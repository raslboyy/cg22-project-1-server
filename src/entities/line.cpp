// #include "line.h"
//
// namespace server::entities {
//
// template <ColorSpace Space>
// Line<Space>::Line(Point p1, Point p2, int thickness, Pixel<Space> color)
//     : p1_(p1), p2_(p2), thickness_(thickness), color_(color) {}
//
// template <ColorSpace Space>
// Pnm<Space> Line<Space>::operator()(const Pnm<Space>& image) {
//   int scale = std::max(image.width(), image.height());
//   Canvas canvas(image);
//   SingleLine(canvas, p1_, p2_, scale);
// }
//
// template <ColorSpace Space>
// void Line<Space>::SingleLine(Canvas<Space>& canvas, Point start, Point
// finish,
//                              int scale) {
//   Point d(finish - start);
//   auto point = start.NewScale(scale) + Point{scale / 2, scale / 2};
//   for (int i = 0; i < scale; i++) {
//     canvas.Plot(point.NewScale(1. / scale), color_);
//     point += d;
//   }
// }
//
// }  // namespace server::entities
