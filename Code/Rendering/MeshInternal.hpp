#ifndef _MESH_INTERNAL_HPP_
#define _MESH_INTERNAL_HPP_

#include "Assets/Asset.hpp"
#include "Assets/Importers/Assimp/AssimpImporter.hpp"
#include "MeshGeometry.hpp"
#include "Vertex.hpp"

namespace te
{
	class MeshInternal: public Asset
	{

	public:

		MeshInternal() = default;
		MeshInternal(const std::string& path);
		~MeshInternal();

		bool isVisible() const;

		virtual void load() = 0;

	protected:

		virtual void setup(AssimpImporter& importer) = 0;
		virtual void setupVertices(const std::vector<Vertex>& indices) = 0;
		virtual void setupIndices(const std::vector<uint32_t>& indices) = 0;
		virtual void cleanup() = 0;

		const std::string getAssetType() const override;

	protected:

		std::vector<std::shared_ptr<Texture>> textures;
		bool bIsVisible = true;
	};
	
} // namespace te


#endif // _MESH_INTERNAL_HPP_