#ifndef _TRANSFORM_HPP_
#define _TRANSFORM_HPP_

#include "Maths/Vector.hpp"
#include "Maths/Matrix.hpp"

namespace te
{
	struct Transform
	{
		sml::vec3 pos{};
		sml::vec3 rot{};
		sml::vec3 scale{};

		sml::mat4 mat{};
		sml::mat4 posMat{};
		sml::mat4 rotMat{};
		sml::mat4 scaleMat{};
	};
} // namespace te

#endif // _TRANSFORM_HPP_