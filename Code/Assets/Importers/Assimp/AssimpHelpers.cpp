#include "AssimpHelpers.hpp"

namespace te
{
	sml::vec3	AssimpToSmlVec3(const aiVector3D& vec)
	{
		return (sml::vec3(vec.x, vec.y, vec.z));
	}

	sml::vec4	AssimpToSmlVec4(const aiColor4D& vec)
	{
		return (sml::vec4(vec.r, vec.g, vec.b, vec.a));
	}

	sml::quat	AssimpToSmlQuat(const aiQuaternion& quat)
	{
		return {};
		//return (sml::quat(quat.w, quat.x, quat.y, quat.z));
	}

	sml::mat4	AssimpToSmlMatrix(const aiMatrix4x4 mat)
	{
		return {
			{ static_cast<float>(mat.a1), static_cast<float>(mat.a2), static_cast<float>(mat.a3), static_cast<float>(mat.a4) },
			{ static_cast<float>(mat.b1), static_cast<float>(mat.b2), static_cast<float>(mat.b3), static_cast<float>(mat.b4) },
			{ static_cast<float>(mat.c1), static_cast<float>(mat.c2), static_cast<float>(mat.c3), static_cast<float>(mat.c4) },
			{ static_cast<float>(mat.d1), static_cast<float>(mat.d2), static_cast<float>(mat.d3), static_cast<float>(mat.d4) }};
	}
}