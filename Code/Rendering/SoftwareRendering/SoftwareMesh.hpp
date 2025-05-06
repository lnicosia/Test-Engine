#ifndef _SOFTWARE_MESH_HPP
#define _SOFTWARE_MESH_HPP

#include "Rendering/MeshInternal.hpp"

namespace te
{
	class SoftwareMesh : public MeshInternal
	{
	public:
		SoftwareMesh();

		void setMaterial(std::shared_ptr<Material> newMat) override;

	protected:

		void setup(AssimpImporter& importer) override;
		void setupVertices(const std::vector<Vertex>& indices) override;
		void setupIndices(const std::vector<uint32_t>& indices) override;
		void cleanup() override;
	};
} // namespace te

#endif // _SOFTWARE_MESH_HPP