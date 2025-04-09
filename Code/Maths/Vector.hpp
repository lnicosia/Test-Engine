#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_

#include <array>

namespace sml
{
    template <typename T = float, size_t size = 3 >
	struct Vector
	{
		std::array<T, size> data;

		union
		{
			float norm;
			float length;
		};
	};

	typedef Vector<float, 2> vec2;
	typedef Vector<float, 3> vec3;
	
} // namespace sml

#endif // _VECTOR_HPP_