
#ifndef RTL_FABRIC_HPP
#define RTL_FABRIC_HPP

namespace rastl {

template<typename IndexType = int>
class FabricBase
{
public:
  using Index_t = IndexType;

  template<typename MapFunction, typename ... Args>
  void map(Index_t i0start, Index_t i0size, Index_t i1start, Index_t i1size, MapFunction mapFunction, Args... args)
  {
    for (Index_t i1 = i1start; i1 < i1start + i1size; ++i1)
    {
      for (Index_t i0 = i0start; i0 < i0start + i0size; ++i0)
      {
        mapFunction(i0, i1, args...);
      }
    }
  }
};

template<typename EntryType, typename IndexType = int>
class FabricPlain : public FabricBase<IndexType>
{
public:
  using Entry_t = EntryType;
  using Index_t = IndexType;

  Entry_t *m_entries;
  Index_t m_leadDim;

  Entry_t& getEntry(Index_t i0, Index_t i1)
  {
    return m_entries[i1 * m_leadDim + i0];
  }

  void setEntry(Index_t i0, Index_t i1, const Entry_t& entry)
  {
    m_entries[i1 * m_leadDim + i0] = entry;
  }

  template<typename MapFunction, typename ... Args>
  void map(Index_t i0start, Index_t i0size, Index_t i1start, Index_t i1size, MapFunction mapFunction, Args... args)
  {
    FabricBase<Index_t>::map(i0start, i0size, i1start, i1size,
      [this, mapFunction, args...](Index_t i0, Index_t i1)
      {
        mapFunction(getEntry(i0, i1), args...);
      }
    );
  }
};

} // namespace rastl

#endif // __RTL_FABRIC_HPP__
