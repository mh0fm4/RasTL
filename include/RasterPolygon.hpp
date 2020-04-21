
#ifndef RTL_RASTER_POLYGON_HPP
#define RTL_RASTER_POLYGON_HPP

#include "Geometry.hpp"
#include "Canvas.hpp"
#include "RasterPolygonBase.hpp"
#include "RasterPolygonBorder.hpp"
#include "RasterPolygonScanline.hpp"
#include "RasterPolygonPointInclusion.hpp"

namespace rastl {

template<typename CanvasType>
class RasterPolygon: RasterPolygonBase<CanvasType>
{
public:
  using Coordinate_t = typename CanvasType::Coordinate_t;
  using Entry_t = typename CanvasType::Entry_t;
  using Index_t = typename CanvasType::Index_t;

  using Polygon_t = typename RasterPolygonBase<CanvasType>::Polygon_t;
  using PolygonList_t = typename RasterPolygonBase<CanvasType>::PolygonList_t;

  static void draw(CanvasType& canvas, const Polygon_t& polygon, const Entry_t& value)
  {
    RasterPolygonBase<CanvasType>::checkPolygon(polygon);

    RasterPolygonBorder<CanvasType>::draw(canvas, polygon, value);
  }

  static void draw(CanvasType& canvas, const PolygonList_t& polygons, const Entry_t& value)
  {
    for (typename PolygonList_t::const_iterator polygon = polygons.begin(); polygon != polygons.end(); ++polygon)
    {
      draw(canvas, *polygon, value);
    }
  }

  static void fill(CanvasType& canvas, const Polygon_t& polygon, const Entry_t& value)
  {
    RasterPolygonBase<CanvasType>::checkPolygon(polygon);

    RasterPolygonScanline<CanvasType>::fill(canvas, polygon, value);
  }

  static void fill(CanvasType& canvas, const PolygonList_t& polygons, const Entry_t& value)
  {
    for (typename PolygonList_t::const_iterator polygon = polygons.begin(); polygon != polygons.end(); ++polygon)
    {
      fill(canvas, *polygon, value);
    }
  }
};

} // namespace rastl

#endif // RTL_RASTER_POLYGON_HPP
