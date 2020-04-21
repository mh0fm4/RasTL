
#ifndef RTL_CANVAS_HPP
#define RTL_CANVAS_HPP

#include <algorithm>

namespace rastl {

template<typename FabricType, typename CoordinateType, typename IndexType = typename FabricType::Index_t>
class Canvas
{
public:
  using Entry_t = typename FabricType::Entry_t;
  using Coordinate_t = CoordinateType;
  using Index_t = IndexType;

  FabricType* m_fabric;
  // std::pair<Coordinate_t, Coordinate_t> m_minCoord, m_maxCoord;
  IndexType m_firstDim, m_secondDim, m_leadDim;

  typename FabricType::Entry_t& getEntry(IndexType i0, IndexType i1)
  {
    return m_fabric->getEntry(i0, i1);
  }

  void setEntry(IndexType i0, IndexType i1, const typename FabricType::Entry_t& entry)
  {
    if (0 <= i0 && i0 < m_firstDim && 0 <= i1 && i1 < m_secondDim)
    {
      m_fabric->setEntry(i0, i1, entry);
    }
  }

  void convertPointCoordinateToIndex(const Point<Coordinate_t> &point, IndexType& i0, IndexType& i1) const
  {
    i0 = static_cast<IndexType>(point.x);
    i1 = static_cast<IndexType>(point.y);
  }

  void print()
  {
    IndexType count = 0;
    m_fabric->map(0, m_firstDim, 0, m_secondDim,
      [this, &count](typename FabricType::Entry_t& entry)
      {
        std::cout << entry << " ";
        ++count;
        if (count == m_leadDim)
        {
          std::cout << std::endl;
          count = 0;
        }
      }
    );
  }

  void fill(const typename FabricType::Entry_t& value)
  {
    m_fabric->map(0, m_firstDim, 0, m_secondDim,
      [value](typename FabricType::Entry_t& entry)
      {
        entry = value;
      }
    );
  }
};

} // namespace rastl

#endif // RTL_CANVAS_HPP
