#ifndef _VERTEX_HPP_
#define _VERTEX_HPP_

#include "Maths/Vector.hpp"

namespace te
{
	struct Vertex
	{
	public:
		constexpr Vertex() = default;
		constexpr Vertex(const sml::vec3& pos, const sml::vec3& color): pos(pos), color(color) {}

		sml::vec3 pos;
		sml::vec3 color;
	};
};

#endif // _VERTEX_HPP_