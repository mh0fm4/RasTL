
#ifndef RTL_RASTER_POLYGON_BASE_HPP
#define RTL_RASTER_POLYGON_BASE_HPP

#include <cassert>

#include "Geometry.hpp"
#include "Canvas.hpp"
#include "RasterBase.hpp"

namespace rastl {

template<typename CanvasType>
class RasterPolygonBase: RasterBase<CanvasType>
{
public:
  using Coordinate_t = typename CanvasType::Coordinate_t;
  using Entry_t = typename CanvasType::Entry_t;
  using Index_t = typename CanvasType::Index_t;

  using Polygon_t = Polygon<Coordinate_t>;
  using PolygonList_t = List<Polygon_t>;

  static void checkPolygon(const Polygon_t& polygon)
  {
    assert(1 < polygon.size());
  }
};

} // namespace rastl

#endif // RTL_RASTER_POLYGON_BASE_HPP
