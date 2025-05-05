#ifndef _ASSIMP_HELPERS_HPP_
#define _ASSIMP_HELPERS_HPP_

#include "Maths/Matrix.hpp"
#include "Maths/Vector.hpp"
#include "Maths/Quaternion.hpp"

//	Fix for assimp
#undef max
#undef min
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace te
{
	sml::quat AssimpToSmlQuat(const aiQuaternion& quat);
	sml::vec3 AssimpToSmlVec3(const aiVector3D& vec);
	sml::vec4 AssimpToSmlVec4(const aiColor4D& vec);
	sml::mat4 AssimpToSmlMatrix(const aiMatrix4x4 mat);
}

#endif // _ASSIMP_HELPERS_HPP_