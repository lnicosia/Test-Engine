#ifndef _MESH_INTERNAL_HPP_
#define _MESH_INTERNAL_HPP_

#include "Assets/Asset.hpp"
#include "Assets/Importers/Assimp/AssimpImporter.hpp"

#include "Rendering/MeshGeometry.hpp"
#include "Rendering/Materials/Material.hpp"
#include "Rendering/Vertex.hpp"

#include "Maths/Matrix.hpp"

namespace te
{
	class MeshInternal: public Asset
	{

	public:

		MeshInternal() = default;
		MeshInternal(const std::string& path);
		~MeshInternal();

		virtual void setMaterial(std::shared_ptr<Material> newMat) = 0;

		MeshBounds getBounds() const;
		sml::mat4 getLocalTransform() const;
		sml::mat4 getWorldTransform() const;
		bool isVisible() const;

		bool shouldCull(const sml::mat4& viewProj) const;

	protected:

		virtual void setup(AssimpImporter& importer) = 0;
		virtual void setupVertices(const std::vector<Vertex>& indices) = 0;
		virtual void setupIndices(const std::vector<uint32_t>& indices) = 0;
		virtual void cleanup() = 0;
		
		const std::string getAssetType() const override;

		void updateTransform(const sml::mat4& parentMat);

	protected:

		std::shared_ptr<Material> material{};

		bool bIsVisible = true;

		sml::mat4 localTransform{};
		sml::mat4 worldTransform{};
		MeshBounds bounds{};
	};
	
} // namespace te


#endif // _MESH_INTERNAL_HPP_