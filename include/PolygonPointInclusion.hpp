
#ifndef RASTL_POLYGON_POINT_INCLUSION_HPP
#define RASTL_POLYGON_POINT_INCLUSION_HPP

namespace rastl {

template<typename PointType, typename PolygonType>
using PointInclusionTest = bool(const PolygonType&, const PointType&);

///
/// \brief  see https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
///
template<typename PointType, typename PolygonType>
bool isPointInsidePolygonPNPOLY(const PolygonType& polygon, const PointType& point)
{
  bool inside = false;

  for (auto polygonPoint = polygon.begin(); polygonPoint != polygon.end(); ++polygonPoint)
  {
    const PointType& p0 = *polygonPoint;
    const PointType& p1 = (polygonPoint + 1 == polygon.end())?*polygon.begin():*(polygonPoint + 1);

    bool test = (p1.y >= point.y) != (p0.y >= point.y);
    if (test)
    {
      test = (point.x < IntDivRound((p0.x - p1.x) * (point.y - p1.y), (p0.y - p1.y)) + p1.x);
    }

    if (test)
    {
      inside = !inside;
    }
  }

  return inside;
}

///
/// \brief  http://geomalgorithms.com/a03-_inclusion.html
///
template<typename PointType, typename PolygonType>
bool isPointInsidePolygonCrossingNumber(const PolygonType& polygon, const PointType& point)
{
  bool inside = false;

  for (auto polygonPoint = polygon.begin(); polygonPoint != polygon.end(); ++polygonPoint)
  {
    const PointType& p0 = *polygonPoint;
    const PointType& p1 = (polygonPoint + 1 == polygon.end())?*polygon.begin():*(polygonPoint + 1);

    if (((p0.y <= point.y) && (p1.y > point.y)) || ((p0.y > point.y) && (p1.y <=  point.y)))
    {
      float vt = (float) (point.y  - p0.y) / (p1.y - p0.y);
      if (point.x <  p0.x + vt * (p1.x - p0.x))
      {
        inside = !inside;
      }
    }
  }

  return inside;
}

template<typename PointType>
int isLeft(const PointType& p0, const PointType& p1, const PointType& p2)
{
  return ((p1.x - p0.x) * (p2.y - p0.y) - (p2.x -  p0.x) * (p1.y - p0.y));
}

template<typename PointType, typename PolygonType>
bool isPointInsidePolygonWindingNumber(const PolygonType& polygon, const PointType& point)
{
  int windingNumber = 0;

  for (auto polygonPoint = polygon.begin(); polygonPoint != polygon.end(); ++polygonPoint)
  {
    const PointType& p0 = *polygonPoint;
    const PointType& p1 = (polygonPoint + 1 == polygon.end())?*polygon.begin():*(polygonPoint + 1);

    if (p0.y <= point.y)
    {
      if (p1.y > point.y)
      {
        if (isLeft(p0, p1, point) > 0)
        {
          ++windingNumber;
        }
      }

    } else
    {
      if (p1.y <= point.y)
      {
        if (isLeft(p0, p1, point) < 0)
        {
          --windingNumber;
        }
      }
    }
  }

  return (windingNumber != 0);
}

} // namespace rastl

#endif // RASTL_POLYGON_POINT_INCLUSION_HPP
