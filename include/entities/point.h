#ifndef SERVICE_TEMPLATE_POINT_H
#define SERVICE_TEMPLATE_POINT_H

#include <cstddef>
#include <functional>

namespace server::entities {
class Point {
 public:
  Point() = default;
  Point(int x, int y) : x_(x), y_(y) {}
  Point(const Point&) = default;
  auto& x() { return x_; }
  auto& y() { return y_; }
  [[nodiscard]] auto x() const { return x_; }
  [[nodiscard]] auto y() const { return y_; }

  Point& operator+=(const Point& rhs);
  friend Point operator+(Point lhs, const Point& rhs);
  Point& operator-=(const Point& rhs);
  friend Point operator-(Point lhs, const Point& rhs);
  Point& operator*=(int c);
  friend Point operator*(Point lhs, int c);
  Point& operator/=(int c);
  friend Point operator/(Point lhs, int c);
  friend bool operator==(const Point& lhs, const Point& rhs);

 private:
  int x_{};
  int y_{};
};
}  // namespace server::entities

namespace std {
template <>
struct hash<server::entities::Point> {
  size_t operator()(const server::entities::Point& p) const {
    return std::hash<decltype(p.x())>()(p.x()) ^
           std::hash<decltype(p.y())>()(p.y());
  }
};
}  // namespace std

#endif  // SERVICE_TEMPLATE_POINT_H
