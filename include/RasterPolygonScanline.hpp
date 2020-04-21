
#ifndef RTL_RASTER_POLYGON_SCANLINE_HPP
#define RTL_RASTER_POLYGON_SCANLINE_HPP

#include <algorithm>

#include "Geometry.hpp"
#include "Canvas.hpp"
#include "RasterPolygonScanlineBase.hpp"

namespace rastl {

template<typename CanvasType>
class RasterPolygonScanlineSequential: RasterPolygonScanlineBase<CanvasType>
{
public:
  using Base = RasterPolygonScanlineBase<CanvasType>;

  using Coordinate_t = typename CanvasType::Coordinate_t;
  using Entry_t = typename CanvasType::Entry_t;
  using Index_t = typename CanvasType::Index_t;

  using Polygon_t = typename RasterPolygonScanlineBase<CanvasType>::Polygon_t;
  using PolygonList_t = typename RasterPolygonScanlineBase<CanvasType>::PolygonList_t;

  static void fill(CanvasType& canvas, const Polygon_t& polygon, const Entry_t& value)
  {
    RasterPolygonBase<CanvasType>::checkPolygon(polygon);

    typename Base::PolygonLineList_t lines;
    Base::convertPolygonToLines(canvas, polygon, lines);

    struct {
      bool operator()(const typename Base::PolygonLine_t& a, const typename Base::PolygonLine_t& b) const
      {
        return std::min(a.p0.y, a.p1.y) < std::min(b.p0.y, b.p1.y);
      }
    } comparePolygonLinesLess;

    lines.sort(comparePolygonLinesLess);

    typename Base::PolygonLineList_t activeLines;

    for (Index_t scanline = 0; scanline < canvas.m_secondDim; ++scanline)
    {
      while (!lines.empty())
      {
        typename Base::PolygonLineList_t::iterator nextLine = lines.begin();

        if (std::min(nextLine->p0.y, nextLine->p1.y) > scanline) break;

        activeLines.splice(activeLines.end(), lines, nextLine);
      }

      if (!activeLines.empty())
      {
        typename Base::ScanlineIntersectionList intersections, intersections2;
        Base::prepareIntersections(activeLines, scanline, intersections, intersections2, true);

        Base::fillScanlineSegmentFromIntersections(canvas, value, scanline, 0, canvas.m_firstDim, intersections.size(), intersections.data(), intersections2.data());
      }
    }
  }
};


template<typename CanvasType>
class RasterPolygonScanlineParallelLines: RasterPolygonScanlineBase<CanvasType>
{
public:
  using Base = RasterPolygonScanlineBase<CanvasType>;

  using Coordinate_t = typename CanvasType::Coordinate_t;
  using Entry_t = typename CanvasType::Entry_t;
  using Index_t = typename CanvasType::Index_t;

  using Polygon_t = typename Base::Polygon_t;
  using PolygonList_t = typename Base::PolygonList_t;

  static void fill(CanvasType& canvas, const Polygon_t& polygon, const Entry_t& value)
  {
    RasterPolygonBase<CanvasType>::checkPolygon(polygon);

    typename Base::PolygonLineList_t lines;
    Base::convertPolygonToLines(canvas, polygon, lines);

    for (Index_t scanline = 0; scanline < canvas.m_secondDim; ++scanline)
    {
      typename Base::ScanlineIntersectionList intersections, intersections2;
      Base::prepareIntersections(lines, scanline, intersections, intersections2, false);

      Base::fillScanlineSegmentFromIntersections(canvas, value, scanline, 0, canvas.m_firstDim, intersections.size(), intersections.data(), intersections2.data());
    }
  }
};


template<typename CanvasType>
class RasterPolygonScanlineParallelPixels: RasterPolygonScanlineBase<CanvasType>
{
public:
  using Base = RasterPolygonScanlineBase<CanvasType>;

  using Coordinate_t = typename CanvasType::Coordinate_t;
  using Entry_t = typename CanvasType::Entry_t;
  using Index_t = typename CanvasType::Index_t;

  using Polygon_t = typename Base::Polygon_t;
  using PolygonList_t = typename Base::PolygonList_t;

  static void fill(CanvasType& canvas, const Polygon_t& polygon, const Entry_t& value)
  {
    RasterPolygonBase<CanvasType>::checkPolygon(polygon);

    typename Base::PolygonLineList_t lines;
    Base::convertPolygonToLines(canvas, polygon, lines);

    for (Index_t scanline = 0; scanline < canvas.m_secondDim; ++scanline)
    {
      typename Base::ScanlineIntersectionList intersections, intersections2;
      Base::prepareIntersections(lines, scanline, intersections, intersections2, false);

      for (Index_t entry = 0; entry < canvas.m_firstDim; ++entry)
      {
        Base::fillScanlineSegmentFromIntersections(canvas, value, scanline, entry, 1, intersections.size(), intersections.data(), intersections2.data());
      }
    }
  }
};


template<typename CanvasType>
using RasterPolygonScanline = RasterPolygonScanlineSequential<CanvasType>;

} // namespace rastl

#endif // RTL_RASTER_POLYGON_SCANLINE_HPP
