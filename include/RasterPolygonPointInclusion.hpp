
#ifndef RTL_RASTER_POLYGON_POINT_INCLUSION_HPP
#define RTL_RASTER_POLYGON_POINT_INCLUSION_HPP

#include "Geometry.hpp"
#include "Canvas.hpp"
#include "PolygonPointInclusion.hpp"
#include "RasterPolygonBorder.hpp"

namespace rastl {

template<typename CanvasType, PointInclusionTest<Point<typename CanvasType::Coordinate_t>, Polygon<typename CanvasType::Coordinate_t>> isPointInsidePolygon = isPointInsidePolygonPNPOLY>
class RasterPolygonPointInclusion: RasterPolygonBase<CanvasType>
{
public:
  using Coordinate_t = typename CanvasType::Coordinate_t;
  using Entry_t = typename CanvasType::Entry_t;
  using Index_t = typename CanvasType::Index_t;

  using Polygon_t = typename RasterPolygonBase<CanvasType>::Polygon_t;
  using PolygonList_t = typename RasterPolygonBase<CanvasType>::PolygonList_t;

  static void fill(CanvasType& canvas, const Polygon_t& polygon, const Entry_t& value)
  {
    RasterPolygonBase<CanvasType>::checkPolygon(polygon);

    for (Index_t y = 0; y < canvas.m_secondDim; ++y)
    {
      for (Index_t x = 0; x < canvas.m_firstDim; ++x)
      {
        if (isPointInsidePolygon(polygon, Point<Coordinate_t>(x, y)))
        {
          canvas.setEntry(x, y, value);
        }
      }
    }

    RasterPolygonBorder<CanvasType>::draw(canvas, polygon, value);
  }
};

} // namespace rastl

#endif // RTL_RASTER_POLYGON_POINT_INCLUSION_HPP
