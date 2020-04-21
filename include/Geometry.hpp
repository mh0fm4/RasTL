
#ifndef RTL_GEOMETRY_HPP
#define RTL_GEOMETRY_HPP

#include <vector>

namespace rastl {

template<typename T>
struct Point
{
  T x, y;

  Point():x(), y() {}
  Point(T _x, T _y):x(_x), y(_y) {}
};


template<typename T>
struct Line
{
  Point<T> p0, p1;

  Line():p0(), p1() {}
  Line(const Point<T>& _p0, const Point<T>& _p1):p0(_p0), p1(_p1) {}
};


template<typename T>
using List = std::vector<T>;


template<typename T>
struct Polygon: public std::vector<Point<T>>
{
  using PointType = Point<T>;

  void shift(T dx, T dy)
  {
    for (typename std::vector<Point<T>>::iterator point = this->begin(); point != this->end(); ++point)
    {
      point->x += dx;
      point->y += dy;
    }
  }
};

} // namespace rastl

#endif // RTL_GEOMETRY_HPP
