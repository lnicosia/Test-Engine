#include "MathsUtils.hpp"

namespace sml
{
	float frsqrt(float nb)
	{
		unsigned int i;
		float x2, y;

		x2 = nb * 0.5f;
		y = nb;
		unsigned int* p1 = reinterpret_cast<unsigned int*>(&nb);
		i = 0x5f3759df - (*p1 >> 1);
		float* p2 = reinterpret_cast<float*>(&i);
		float tmp = *p2;
		y = tmp * (1.5f - (x2 * tmp * tmp));
		return y;
	}

} // namespace sml