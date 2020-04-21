
#ifndef RTL_RASTER_POLYGON_BORDER_HPP
#define RTL_RASTER_POLYGON_BORDER_HPP

#include "Geometry.hpp"
#include "Canvas.hpp"
#include "RasterLine.hpp"

namespace rastl {

template<typename CanvasType>
class RasterPolygonBorder: RasterPolygonBase<CanvasType>
{
public:
  using Coordinate_t = typename CanvasType::Coordinate_t;
  using Entry_t = typename CanvasType::Entry_t;
  using Index_t = typename CanvasType::Index_t;

  using Point_t = Point<Coordinate_t>;
  using Polygon_t = typename RasterPolygonBase<CanvasType>::Polygon_t;
  using PolygonList_t = typename RasterPolygonBase<CanvasType>::PolygonList_t;

  static void draw(CanvasType& canvas, const Polygon_t& polygon, const Entry_t& value)
  {
    typename RasterLine<CanvasType>::LineList_t lines;
    for (typename Polygon_t::const_iterator point = polygon.begin(); point != polygon.end(); ++point)
    {
      const Point_t& p0 = *point;
      const Point_t& p1 = (point + 1 == polygon.end())?*polygon.begin():*(point + 1);
      lines.push_back(typename RasterLine<CanvasType>::Line_t(p0, p1));
    }

    RasterLine<CanvasType>::draw(canvas, lines, value);
  }
};

} // namespace rastl

#endif // RTL_RASTER_POLYGON_BORDER_HPP
