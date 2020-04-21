
#ifndef RTL_RASTER_POLYGON_SCANLINE_BASE_HPP
#define RTL_RASTER_POLYGON_SCANLINE_BASE_HPP

#include <vector>
#include <list>
#include <algorithm>

#include "Math.hpp"
#include "Geometry.hpp"
#include "Canvas.hpp"
#include "RasterPolygonBase.hpp"

namespace rastl {

template<typename CanvasType>
class RasterPolygonScanlineBase: RasterPolygonBase<CanvasType>
{
public:
  using Coordinate_t = typename CanvasType::Coordinate_t;
  using Entry_t = typename CanvasType::Entry_t;
  using Index_t = typename CanvasType::Index_t;

  using Polygon_t = typename RasterPolygonBase<CanvasType>::Polygon_t;
  using PolygonList_t = typename RasterPolygonBase<CanvasType>::PolygonList_t;

  using PolygonPoint_t = Point<Index_t>;
  using PolygonLine_t = Line<Index_t>;
  using PolygonLineList_t = std::list<PolygonLine_t>;

  enum ScanlineIntersectionType
  {
    SIT_NONE           = 0,
    SIT_LINE_CROSS     = 1,
    SIT_LINE_START     = 2,
    SIT_LINE_END       = 3,
    SIT_PARALLEL       = 4,
    SIT_NUM_OF_ENTRIES = 5,
  };

  struct ScanlineIntersection
  {
    ScanlineIntersectionType type;
    Index_t x0, x1;
  };

  using ScanlineIntersectionList = std::vector<ScanlineIntersection>;

  static void convertPolygonToLines(const CanvasType& canvas, const Polygon<Coordinate_t>& polygon, PolygonLineList_t& lines)
  {
    for (auto point = polygon.begin(); point != polygon.end(); ++point)
    {
      const Point<Coordinate_t>& p0 = *point;
      const Point<Coordinate_t>& p1 = (point + 1 == polygon.end())?*polygon.begin():*(point + 1);

      PolygonPoint_t point0, point1;
      canvas.convertPointCoordinateToIndex(p0, point0.x, point0.y);
      canvas.convertPointCoordinateToIndex(p1, point1.x, point1.y);

      lines.push_back(PolygonLine_t(point0, point1));
    }
  }

  static void computeIntersection(const PolygonLine_t& line, Index_t y, ScanlineIntersection &intersection)
  {
    if ((line.p0.y <= y && y <= line.p1.y) || (line.p1.y <= y && y <= line.p0.y))
    {
      Index_t diffX = std::abs(line.p1.x - line.p0.x);
      Index_t diffY = std::abs(line.p1.y - line.p0.y);
      int stepX = signum(line.p1.x - line.p0.x);
      int stepY = signum(line.p1.y - line.p0.y);
      Index_t minX = std::min(line.p0.x, line.p1.x);
      Index_t minY = std::min(line.p0.y, line.p1.y);
      Index_t localY = y - minY;
      Index_t localX0, localX1;

  #if VERBOSE >= 2
      std::cout << "diffX: " << diffX << ", diffY: " << diffY << std::endl;
      std::cout << "stepX: " << stepX << ", stepY: " << stepY << std::endl;
      std::cout << "minX: " << minX << ", minY: " << minY << std::endl;
  #endif

      if (diffY == 0)
      {
        intersection.type = SIT_PARALLEL;

        localX0 = 0;
        localX1 = diffX;

      } else
      {
        bool lineStart = (line.p0.y == y && stepY > 0) || (line.p1.y == y && stepY < 0);
        bool lineEnd = (line.p0.y == y && stepY < 0) || (line.p1.y == y && stepY > 0);
  #if VERBOSE >= 2
        std::cout << "lineStart: " << lineStart << ", lineEnd: " << lineEnd << std::endl;
  #endif

        if (lineStart)
        {
          intersection.type = SIT_LINE_START;

        } else if (lineEnd)
        {
          intersection.type = SIT_LINE_END;

        } else
        {
          intersection.type = SIT_LINE_CROSS;
        }

        bool flat = (std::abs(diffX) > std::abs(diffY));
  #if VERBOSE >= 2
        std::cout << "flat: " << flat << std::endl;
  #endif

        if (flat)
        {
          localX0 = IntDivOutwardZero((localY * 2 - (lineStart?0:1)) * diffX, (diffY * 2));
          localX1 = IntDivTowardZero((localY * 2 + (lineEnd?0:1)) * diffX, (diffY * 2));

        } else
        {
          localX0 = localX1 = IntDivRound((localY * 2) * diffX, (diffY * 2));
        }
      }

  #if VERBOSE >= 2
      std::cout << "x0: " << localX0 << ", x1: " << localX1 << std::endl;
  #endif

      if ((stepX < 0) != (stepY < 0))
      {
        localX0 = diffX - localX0;
        localX1 = diffX - localX1;
      }

      localX0 += minX;
      localX1 += minX;

      if (localX0 > localX1) std::swap(localX0, localX1);

  #if VERBOSE >= 2
      std::cout << "x0: " << localX0 << ", x1: " << localX1 << std::endl;
  #endif

      intersection.x0 = localX0;
      intersection.x1 = localX1;

    } else
    {
      intersection.type = SIT_NONE;
      intersection.x0 = -1;
      intersection.x1 = -1;
    }
  }

  static void computeIntersections(PolygonLineList_t& lines, Index_t scanline, ScanlineIntersectionList& intersections, bool removeLines)
  {
    typename PolygonLineList_t::iterator line = lines.begin();

    while (line != lines.end())
    {
      ScanlineIntersection intersection;
      computeIntersection(*line, scanline, intersection);

      if (intersection.type != SIT_NONE)
      {
        intersections.push_back(intersection);
      }

      if (removeLines && (intersection.type == SIT_LINE_END || intersection.type == SIT_PARALLEL))
      {
        line = lines.erase(line);

      } else
      {
        ++line;
      }
    }
  }

  static void sortIntersectionsByStart(ScanlineIntersectionList& intersections)
  {
    struct {
      bool operator()(const ScanlineIntersection& a, const ScanlineIntersection& b) const
      {
        return a.x0 < b.x0;
      }
    } compareIntersectionsLess;

    std::sort(intersections.begin(), intersections.end(), compareIntersectionsLess);
  }

  static void sortIntersectionsByStop(ScanlineIntersectionList& intersections)
  {
    struct {
      bool operator()(const ScanlineIntersection& a, const ScanlineIntersection& b) const
      {
        return a.x1 < b.x1;
      }
    } compareIntersectionsLess;

    std::sort(intersections.begin(), intersections.end(), compareIntersectionsLess);
  }

  static void prepareIntersections(PolygonLineList_t& lines, Index_t scanline, ScanlineIntersectionList& intersectionsSortedByStart, ScanlineIntersectionList& intersectionsSortedByStop, bool removeLines)
  {
    computeIntersections(lines, scanline, intersectionsSortedByStart, removeLines);

    intersectionsSortedByStop = intersectionsSortedByStart;
    sortIntersectionsByStart(intersectionsSortedByStart);
    sortIntersectionsByStop(intersectionsSortedByStop);
  }

  static bool getLineCrossings(int *counts)
  {
    int lineCrossings = counts[SIT_LINE_CROSS];
    if (counts[SIT_LINE_START] % 2 != 0 && counts[SIT_LINE_END] % 2 != 0) ++lineCrossings;
    if (counts[SIT_PARALLEL] % 2 != 0 && counts[SIT_LINE_START] % 2 != 0) ++lineCrossings;
    if (counts[SIT_PARALLEL] % 2 != 0 && counts[SIT_LINE_END] % 2 != 0) ++lineCrossings;

    return lineCrossings;
  }

  static void fillScanlineSegmentFromIntersections(CanvasType& canvas, const Entry_t& value, Index_t scanline, Index_t segmentOffset, Index_t segmentSize, int numberOfScanlineIntersections, ScanlineIntersection* intersectionsSortedByStart, ScanlineIntersection* intersectionsSortedByStop)
  {
    int fillBorderCount = 0;
    bool fillBody = false;

    int startTop = 0;
    int stopTop = 0;
    Index_t segmentX = segmentOffset;
    Index_t segmentEnd = segmentOffset + segmentSize;

    while ((startTop < numberOfScanlineIntersections || stopTop < numberOfScanlineIntersections) && segmentX < segmentEnd)
    {
      Index_t x = segmentX;
      if (startTop < numberOfScanlineIntersections) x = std::min(x, intersectionsSortedByStart[startTop].x0);
      if (stopTop < numberOfScanlineIntersections) x = std::min(x, intersectionsSortedByStop[stopTop].x1);

      while (startTop < numberOfScanlineIntersections && intersectionsSortedByStart[startTop].x0 == x)
      {
        ++fillBorderCount;
        ++startTop;
      }

      bool fillBorder = (fillBorderCount > 0);
      int intersectionCounts[SIT_NUM_OF_ENTRIES] = { 0 };

      while (stopTop < numberOfScanlineIntersections && intersectionsSortedByStop[stopTop].x1 == x)
      {
        --fillBorderCount;
        ++intersectionCounts[intersectionsSortedByStop[stopTop].type];
        ++stopTop;
      }

      if (getLineCrossings(intersectionCounts) % 2 != 0) fillBody = !fillBody;

      if (x == segmentX)
      {
        if (fillBorder || fillBody) canvas.setEntry(x, scanline, value);
        ++segmentX;
      }
    }
  }
};

} // namespace rastl

#endif // RTL_RASTER_POLYGON_SCANLINE_BASE_HPP
