#include "MeshGeometry.hpp"

namespace te
{
	void MeshGeometry::computeBounds()
	{
		if (vertices.empty())
		{
			return;
		}
		sml::vec3 minPos = vertices[0].pos;
		sml::vec3 maxPos = minPos;

		for (const auto& vertex : vertices)
		{
			minPos = sml::min(minPos, vertex.pos);
			maxPos = sml::max(maxPos, vertex.pos);
		}

		bounds.origin = (minPos + maxPos) * 0.5f;
		bounds.extents = (maxPos - minPos) * 0.5f;
		bounds.radius = bounds.extents.length();

		/*printf("min pos = [%f %f %f], max pos = [%f %f %f]\n",
			minPos.x, minPos.y, minPos.z, maxPos.x, maxPos.y, maxPos.z);
		printf("Mesh origin = [%f, %f, %f], "
			"radius = %f, extents = [%f %f %f]\n",
			bounds.origin.x, bounds.origin.y, bounds.origin.z,
			bounds.radius, bounds.extents.x, bounds.extents.y, bounds.extents.z);*/
	}
} // namespace te