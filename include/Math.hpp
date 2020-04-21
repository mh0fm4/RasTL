
#ifndef RTL_MATH_HPP
#define RTL_MATH_HPP

#include <cstdint>
#include <cmath>

namespace rastl {

template<typename T>
T IntDivRound(T dividend, T divisor)
{
  T add = (std::abs(divisor) / 2) * ((dividend > 0)?1:-1);
  T div = (dividend + add) / divisor;

  return div;
}


template<typename T>
T IntDivTowardZero(T dividend, T divisor)
{
  T add = 0;
  T div = (dividend + add) / divisor;

  return div;
}


template<typename T>
T IntDivOutwardZero(T dividend, T divisor)
{
  int16_t add = (std::abs(divisor) - 1) * ((dividend > 0)?1:-1);
  int16_t div = (dividend + add) / divisor;

  return div;
}


template<typename T> inline constexpr
int signum(T x, std::false_type is_signed)
{
  return T(0) < x;
}


template<typename T> inline constexpr
int signum(T x, std::true_type is_signed)
{
  return (T(0) < x) - (x < T(0));
}


template<typename T> inline constexpr
int signum(T x)
{
  return signum(x, std::is_signed<T>());
}

} // namespace rastl

#endif // RTL_MATH_HPP
