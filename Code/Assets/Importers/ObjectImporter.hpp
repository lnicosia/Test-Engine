#ifndef _OBJECT_IMPORTER_HPP_
#define _OBJECT_IMPORTER_HPP_

#include "Rendering/MeshGeometry.hpp"
#include "Assets/Textures/Texture.hpp"
#include "Maths/Matrix.hpp"

#include <string>
#include <vector>
#include <map>

namespace te
{
	enum Axis
	{
		X_UP,
		Y_UP,
		Z_UP
	};

	struct BoneInfo
	{
		int	id;
		std::string	name;
		sml::mat4	offsetMatrix;
		sml::mat4	localMatrix;
		sml::mat4	modelMatrix;
		sml::mat4	originalMatrix;

		BoneInfo(): id(0), name(),
		offsetMatrix(), localMatrix(), modelMatrix(), originalMatrix()
		{}
	};

	class ObjectImporter
	{
	public:
		ObjectImporter();
		virtual ~ObjectImporter();

		virtual bool readFile(const std::string& path, unsigned int flags) = 0;

		sml::vec3 max{};
		sml::vec3 min{};
		bool isRangeInit;

		std::vector<MeshGeometry> meshes{};

		std::string	directory{};
		std::string	path{};

		std::map<std::string, BoneInfo> bones{};
		int nbBones;
		Axis axis;

	private:

	};
}
#endif // _OBJECT_IMPORTER_HPP_