#include "entities/point.h"

namespace server::entities {

Point& Point::operator+=(const server::entities::Point& rhs) {
  x_ += rhs.x_;
  y_ += rhs.y_;
  return *this;
}

Point operator+(Point lhs, const Point& rhs) {
  lhs += rhs;
  return lhs;
}

Point& Point::operator-=(const server::entities::Point& rhs) {
  x_ -= rhs.x_;
  y_ -= rhs.y_;
  return *this;
}

Point operator-(Point lhs, const Point& rhs) {
  lhs -= rhs;
  return lhs;
}

Point& Point::operator*=(int c) {
  x_ *= c;
  y_ *= c;
  return *this;
}

Point operator*(Point lhs, int c) {
  lhs *= c;
  return lhs;
}

Point& Point::operator/=(int c) {
  x_ /= c;
  y_ /= c;
  return *this;
}

Point operator/(Point lhs, int c) {
  lhs /= c;
  return lhs;
}

bool operator==(const Point& lhs, const Point& rhs) {
  return lhs.x() == rhs.x() && lhs.y() == rhs.y();
}

}  // namespace server::entities
