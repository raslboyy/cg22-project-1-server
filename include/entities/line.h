#ifndef SERVICE_TEMPLATE_LINE_H
#define SERVICE_TEMPLATE_LINE_H

#include "canvas.h"
#include "core/pixel.h"
#include "core/pnm.h"
#include "point.h"

#include <cmath>
#include <unordered_map>

namespace server::entities {
using namespace server::core::pnm::color_space;
template <ColorSpace Space>
using Pnm = server::core::pnm::PNM<Space>;

template <ColorSpace Space>
class Line {
 public:
  Line(Point, Point, int thickness, Pixel<Space>, double alpha);
  Pnm<Space> operator()(const Pnm<Space>&);

 private:
  Point p1_;
  Point p2_;
  double thickness_;
  double alpha_;
  Pixel<Space> color_;

  std::unordered_map<Point, double> SingleLine(int x0, int x1, int y0, int y1);
};

template <ColorSpace Space>
Line<Space>::Line(Point p1, Point p2, int thickness, Pixel<Space> color,
                  double alpha)
    : p1_(p1), p2_(p2), thickness_(thickness), alpha_(alpha), color_(color) {}

template <ColorSpace Space>
Pnm<Space> Line<Space>::operator()(const Pnm<Space>& origin) {
  Pnm<Space> image(origin);
  Canvas<Space> canvas(image);

  std::unordered_map<Point, double> alpha;

  int m = thickness_ / 2;
  for (int i = -m; i <= m; i++)
    for (int j = -m; j <= m; j++) {
      if (std::abs(i) + std::abs(j) > m) continue;
      auto p1 = p1_ + Point{i, j};
      auto p2 = p2_ + Point{i, j};
      for (const auto& [p, a] : SingleLine(p1.x(), p2.x(), p1.y(), p2.y()))
        alpha[p] = std::max(alpha[p], a);
    }

  for (const auto& [p, a] : alpha) canvas.Plot(p, color_, a * alpha_);

  return image;
}

template <ColorSpace Space>
std::unordered_map<Point, double> Line<Space>::SingleLine(int x0, int x1,
                                                          int y0, int y1) {
  auto ipart = [](float x) -> int { return int(std::floor(x)); };
  auto round = [](float x) -> float { return std::round(x); };
  auto fpart = [](float x) -> float { return x - std::floor(x); };
  auto rfpart = [=](float x) -> float { return 1 - fpart(x); };

  std::unordered_map<Point, double> brightness;

  bool steep = abs(y1 - y0) - abs(x1 - x0);

  if (steep) {
    std::swap(x0, y0);
    std::swap(x1, y1);
  }
  if (x0 > x1) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }

  const float dx = (float)x1 - (float)x0;
  const float dy = (float)y1 - (float)y0;
  const float gradient = (dx == 0) ? 1.f : dy / dx;

  int xpx11;
  float intery;
  {
    const float xend = round(x0);
    const float yend = y0 + gradient * (xend - x0);
    const float xgap = rfpart(x0 + 0.5);
    xpx11 = int(xend);
    const int ypx11 = ipart(yend);
    if (steep) {
      brightness.emplace(Point{ypx11, xpx11}, rfpart(yend) * xgap);
      brightness.emplace(Point{ypx11 + 1, xpx11}, fpart(yend) * xgap);
    } else {
      brightness.emplace(Point{xpx11, ypx11}, rfpart(yend) * xgap);
      brightness.emplace(Point{xpx11, ypx11 + 1}, fpart(yend) * xgap);
    }
    intery = yend + gradient;
  }

  int xpx12;
  {
    const float xend = round(x1);
    const float yend = y1 + gradient * (xend - x1);
    const float xgap = rfpart(x1 + 0.5);
    xpx12 = int(xend);
    const int ypx12 = ipart(yend);
    if (steep) {
      brightness.emplace(Point{ypx12, xpx12}, rfpart(yend) * xgap);
      brightness.emplace(Point{ypx12 + 1, xpx12}, fpart(yend) * xgap);
    } else {
      brightness.emplace(Point{xpx12, ypx12}, rfpart(yend) * xgap);
      brightness.emplace(Point{xpx12, ypx12 + 1}, fpart(yend) * xgap);
    }
  }

  if (steep) {
    for (int x = xpx11 + 1; x < xpx12; x++) {
      brightness.emplace(Point{ipart(intery), x}, rfpart(intery));
      brightness.emplace(Point{ipart(intery) + 1, x}, fpart(intery));
      intery += gradient;
    }
  } else {
    for (int x = xpx11 + 1; x < xpx12; x++) {
      brightness.emplace(Point{x, ipart(intery)}, rfpart(intery));
      brightness.emplace(Point{x, ipart(intery) + 1}, fpart(intery));
      intery += gradient;
    }
  }

  return brightness;
}

}  // namespace server::entities

#endif  // SERVICE_TEMPLATE_LINE_H
