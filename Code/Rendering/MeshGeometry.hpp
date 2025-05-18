#ifndef _MESH_GEOMETRY_HPP_
#define _MESH_GEOMETRY_HPP_

#include "Vertex.hpp"
#include "Maths/Matrix.hpp"

#include <vector>

namespace te
{
	struct MeshBounds
	{
		float radius;
		sml::vec3 origin;
		sml::vec3 extents;
	};

	struct MeshGeometry
	{
		MeshGeometry(std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices):
			vertices{std::move(vertices)}, indices{std::move(indices)}
		{
			computeBounds();
		}

		void computeBounds();

		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};
		MeshBounds bounds{};

		sml::mat4 localTransform;
	};
} // namespace te


#endif // _MESH_GEOMETRY_HPP_