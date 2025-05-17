#ifndef _VERTEX_HPP_
#define _VERTEX_HPP_

#include "Maths/Vector.hpp"
#include <cstdint>

namespace te
{
	struct Vertex
	{
	public:
		constexpr Vertex() = default;
		constexpr Vertex(const sml::vec3& pos, const sml::vec3& color, const sml::vec2& texCoord):
			pos{pos}, color{color}, texCoord{texCoord} {}
			constexpr Vertex(sml::vec3&& pos, sml::vec3&& color, sml::vec2&& texCoord):
			pos{std::move(pos)}, color{std::move(color)}, texCoord{std::move(texCoord)} {}

		sml::vec3 pos{};
		sml::vec3 color{};
		sml::vec2 texCoord{};
		sml::vec3 norm{};
	};
};

#endif // _VERTEX_HPP_