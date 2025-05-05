#ifndef _MATHS_UTILS_HPP_
#define _MATHS_UTILS_HPP_

#define SML_TO_RADIANS 0.01745329251f
#define SML_TO_DEGREES 57.2957795131f

namespace sml
{
	/** Doom fast inverse square root */
	float frsqrt(float nb);

	/*template < typename T >
	const T& min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template < typename T >
	const T& max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}*/

} // namespace sml

#endif // _MATHS_UTILS_HPP_