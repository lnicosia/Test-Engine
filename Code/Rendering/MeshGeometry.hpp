#ifndef _MESH_GEOMETRY_HPP_
#define _MESH_GEOMETRY_HPP_

#include "Vertex.hpp"

#include <vector>

namespace te
{
	struct MeshGeometry
	{
		MeshGeometry(std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices):
			vertices{std::move(vertices)}, indices{std::move(indices)}
		{
		}

		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};
	};

} // namespace te


#endif // _MESH_GEOMETRY_HPP_