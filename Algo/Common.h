
#ifndef _COMMON_H_
#define _COMMON_H_

#include <math.h>

#define XJPI 3.14159265358979324

#ifndef Pi
#define Pi   3.14159265358979324
#endif

#define XJ_TOL 1e-6
#define XJ_SMALL           0.000001

namespace xjdef
{
	// define the standard trig values
#ifdef PI
#undef PI
#undef PI_2
#undef PI_4
#endif
const double PI   = 3.14159265358979323846;
const double PI_2 = 1.57079632679489661923;
const double PI_4 = 0.78539816339744830962;
const double LN_2 = 0.69314718055994530942;
const double INVLN_2 = 1.0 / LN_2;


template<typename T>
inline T absolute(T v) {return v<(T)0 ? -v : v;}

/** return true if float lhs and rhs are equivalent,
  * meaning that the difference between them is less than an epsilon value
  * which defaults to 1e-6.
*/
inline bool equivalent(float lhs,float rhs,float epsilon=1e-6)
  { float delta = rhs-lhs; return delta<0.0f?delta>=-epsilon:delta<=epsilon; }

/** return true if double lhs and rhs are equivalent,
  * meaning that the difference between them is less than an epsilon value
  * which defaults to 1e-6.
*/
inline bool equivalent(double lhs,double rhs,double epsilon=1e-6)
  { double delta = rhs-lhs; return delta<0.0?delta>=-epsilon:delta<=epsilon; }

/** return the minimum of two values, equivalent to std::min.
  * std::min not used because of STL implementation under IRIX not containing
  * std::min.
*/
template<typename T>
inline T minimum(T lhs,T rhs) { return lhs<rhs?lhs:rhs; }

/** return the maximum of two values, equivalent to std::max.
  * std::max not used because of STL implementation under IRIX not containing
  * std::max.
*/
template<typename T>
inline T maximum(T lhs,T rhs) { return lhs>rhs?lhs:rhs; }

}

#endif