
#include <iostream>

#define VERBOSE 1

#include "Math.hpp"
#include "Geometry.hpp"
#include "Fabric.hpp"
#include "Canvas.hpp"
#include "RasterLine.hpp"
#include "RasterPolygon.hpp"

using namespace rastl;


int main()
{
  using Entry_t = uint16_t;
  using Coordinate_t = int16_t;
  using Point_t = Point<Coordinate_t>;
  using Line_t = Line<Coordinate_t>;
  using Fabric_t = FabricPlain<Entry_t>;
  using Canvas_t = Canvas<Fabric_t, Coordinate_t>;

#define DX 16
#define DY 16

  const int firstDim = DX;
  const int secondDim = DY;
  Entry_t values[firstDim * secondDim];

  Fabric_t fabric;
  fabric.m_entries = values;
  fabric.m_leadDim = firstDim;

  Canvas_t canvas;
  canvas.m_fabric = &fabric;
  // canvas.m_values = values;
  canvas.m_firstDim = firstDim;
  canvas.m_secondDim = secondDim;
  canvas.m_leadDim = firstDim;

  canvas.fill(0);
  canvas.print();

#define P0X  1
#define P0Y  2
#define P1X  7
#define P1Y  8

  Polygon<Coordinate_t> polygon;
#if 0
  polygon.push_back(Point<Coordinate_t>(P0X, P0Y));
  polygon.push_back(Point<Coordinate_t>(P1X, P1Y));
#elif 0
  polygon.push_back(Point<Coordinate_t>(DX - 1 - P0X, P0Y));
  polygon.push_back(Point<Coordinate_t>(DX - 1 - P1X, P1Y));
#elif 0
  polygon.push_back(Point<Coordinate_t>(DX - 1 - P0X, DY - 1 - P0Y));
  polygon.push_back(Point<Coordinate_t>(DX - 1 - P1X, DY - 1 - P1Y));
#elif 0
  polygon.push_back(Point<Coordinate_t>(P0X, DY - 1 - P0Y));
  polygon.push_back(Point<Coordinate_t>(P1X, DY - 1 - P1Y));
#elif 1
  polygon.push_back(Point<Coordinate_t>(0, 0));
  polygon.push_back(Point<Coordinate_t>(9, 3));
  polygon.push_back(Point<Coordinate_t>(4, 9));
  polygon.push_back(Point<Coordinate_t>(6, 9));
  polygon.push_back(Point<Coordinate_t>(3, 14));
#else
  polygon.push_back(Point<Coordinate_t>(8, 0));
  polygon.push_back(Point<Coordinate_t>(8, 8));
  polygon.push_back(Point<Coordinate_t>(4, 4));
  polygon.push_back(Point<Coordinate_t>(0, 4));
  polygon.push_back(Point<Coordinate_t>(4, 4));
#endif

  polygon.shift(0, 0);

  Polygon<Coordinate_t> polygon2 = polygon;
  polygon2.shift(5, 0);

  Line_t line(Point_t(P0X, P0Y), Point_t(P1X, P1Y));

  std::cout << "RasterLine::draw" << std::endl;
  canvas.fill(0);
  RasterLine<Canvas_t>::draw(canvas, line, Entry_t(1));
  canvas.print();
  std::cout << std::endl;

  std::cout << "RasterPolygon::draw" << std::endl;
  canvas.fill(0);
  RasterPolygon<Canvas_t>::draw(canvas, polygon, Entry_t(1));
  canvas.print();
  std::cout << std::endl;

  std::cout << "RasterPolygonScanlineSequential::fill" << std::endl;
  canvas.fill(0);
  RasterPolygonScanlineSequential<Canvas_t>::fill(canvas, polygon, Entry_t(1));
  canvas.print();
  std::cout << std::endl;

  std::cout << "RasterPolygonScanlineParallelLines::fill" << std::endl;
  canvas.fill(0);
  RasterPolygonScanlineParallelLines<Canvas_t>::fill(canvas, polygon, Entry_t(1));
  canvas.print();
  std::cout << std::endl;

  std::cout << "RasterPolygonScanlineParallelPixels::fill" << std::endl;
  canvas.fill(0);
  RasterPolygonScanlineParallelPixels<Canvas_t>::fill(canvas, polygon, Entry_t(1));
  canvas.print();
  std::cout << std::endl;

  std::cout << "RasterPolygonPointInclusion<PNPOLY>::fill" << std::endl;
  canvas.fill(0);
  RasterPolygonPointInclusion<Canvas_t, isPointInsidePolygonPNPOLY>::fill(canvas, polygon, Entry_t(1));
  canvas.print();
  std::cout << std::endl;

  std::cout << "RasterPolygonPointInclusion<CrossingNumber>::fill" << std::endl;
  canvas.fill(0);
  RasterPolygonPointInclusion<Canvas_t, isPointInsidePolygonCrossingNumber>::fill(canvas, polygon, Entry_t(1));
  canvas.print();
  std::cout << std::endl;

  std::cout << "RasterPolygonPointInclusion<WindingNumber>::fill" << std::endl;
  canvas.fill(0);
  RasterPolygonPointInclusion<Canvas_t, isPointInsidePolygonWindingNumber>::fill(canvas, polygon, Entry_t(1));
  canvas.print();
  std::cout << std::endl;

  RasterPolygon<Canvas_t>::PolygonList_t polygons;
  polygons.push_back(polygon);
  polygons.push_back(polygon2);

  std::cout << "RasterPolygon::draw" << std::endl;
  canvas.fill(0);
  RasterPolygon<Canvas_t>::draw(canvas, polygons, Entry_t(1));
  canvas.print();
  std::cout << std::endl;

  return 0;
}
