
#ifndef RTL_RASTER_LINE_HPP
#define RTL_RASTER_LINE_HPP

#include <cmath>

#include "Geometry.hpp"
#include "RasterBase.hpp"

namespace rastl {

template<typename CanvasType>
class RasterLineBase: RasterBase<CanvasType>
{
public:
  using Coordinate_t = typename CanvasType::Coordinate_t;
  using Entry_t = typename CanvasType::Entry_t;
  using Index_t = typename CanvasType::Index_t;

  using Line_t = Line<Coordinate_t>;
  using LineList_t = List<Line_t>;
};


template<typename CanvasType>
class RasterLineBresenham: RasterLineBase<CanvasType>
{
public:
  using Coordinate_t = typename CanvasType::Coordinate_t;
  using Entry_t = typename CanvasType::Entry_t;
  using Index_t = typename CanvasType::Index_t;

  using Line_t = typename RasterLineBase<CanvasType>::Line_t;
  using LineList_t = typename RasterLineBase<CanvasType>::LineList_t;

  static void draw(CanvasType& canvas, const Line_t& line, const Entry_t& value)
  {
    Index_t point0_x, point0_y, point1_x, point1_y;
    canvas.convertPointCoordinateToIndex(line.p0, point0_x, point0_y);
    canvas.convertPointCoordinateToIndex(line.p1, point1_x, point1_y);

    Index_t diffX = std::abs(point1_x - point0_x);
    Index_t diffY = std::abs(point1_y - point0_y);
    int stepX = (point0_x < point1_x)?1:-1;
    int stepY = (point0_y < point1_y)?1:-1;
    long long err = diffX - diffY;
    long long e2;

    Index_t point_x = point0_x;
    Index_t point_y = point0_y;

    while (1)
    {
      canvas.setEntry(point_x, point_y, value);
      if (point_x == point1_x && point_y == point1_y) break;
      e2 = 2 * err;
      if (-e2 < diffY) { err -= diffY; point_x += stepX; }
      if (e2 < diffX) { err += diffX; point_y += stepY; }
    }
  }

  static void draw(CanvasType& canvas, const LineList_t& lines, const Entry_t& value)
  {
    for (typename LineList_t::const_iterator line = lines.begin(); line != lines.end(); ++line)
    {
      draw(canvas, *line, value);
    }
  }
};


template<typename CanvasType>
using RasterLine = RasterLineBresenham<CanvasType>;

} // namespace rastl

#endif // RTL_RASTER_LINE_HPP
