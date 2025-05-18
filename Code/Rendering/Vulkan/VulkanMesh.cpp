#include "VulkanMesh.hpp"

#include "Assets/Importers/Assimp/AssimpImporter.hpp"
#include "Assets/Textures/VulkanTexture.hpp"
#include "Assets/AssetManager.hpp"

#include "Debug/Log.hpp"

namespace te
{
	VulkanMesh::VulkanMesh(const std::string& path, VulkanDevice* vulkanDevice):
		MeshInternal(path), vulkanDevice{vulkanDevice}
	{
		material = std::make_shared<VulkanMaterial>();
		AssimpImporter importer;
		if (importer.readFile(paths[0].string(), 0))
		{
			this->loaded = true;
		}
		else
		{
			this->loaded = false;
			return;
		}
		setup(importer);
	}

	VulkanMesh::VulkanMesh(VulkanDevice* vulkanDevice, const MeshGeometry& geometry): MeshInternal(),
		vulkanDevice{vulkanDevice}
	{
		material = std::make_shared<VulkanMaterial>();
		setupVertices(geometry.vertices);
		setupIndices(geometry.indices);
		setupDescriptors();
		bounds = geometry.bounds;
	}

	void VulkanMesh::setup(AssimpImporter& importer)
	{
		// TODO
		for (auto aiTexture : importer.getAiTextures())
		{

		}
		for (const MeshGeometry& geometry: importer.meshes)
		{
			// TODO: handle multiple meshes
			setupVertices(geometry.vertices);
			setupIndices(geometry.indices);
			bounds = geometry.bounds;
		}
		setupDescriptors();
	}

	void VulkanMesh::setupVertices(const std::vector<Vertex>& vertices)
	{
		if (!vulkanDevice)
		{
			TE_LOG(TE_RENDERING_LOG, TE_ERROR, "No valid vulkan device\n");
			return;
		}
		vulkanDevice->createVertexBuffer(vertices, vertexBuffer, vertexBufferMemory);
	}

	void VulkanMesh::setupIndices(const std::vector<uint32_t>& indices)
	{
		if (!vulkanDevice)
		{
			TE_LOG(TE_RENDERING_LOG, TE_ERROR, "No valid vulkan device\n");
			return;
		}
		vulkanDevice->createIndexBuffer(indices, indexBuffer, indexBufferMemory);
		indexBufferSize = indices.size();
	}

	void VulkanMesh::setupDescriptors()
	{
		if (!vulkanDevice)
		{
			TE_LOG(TE_RENDERING_LOG, TE_ERROR, "No valid vulkan device\n");
			return;
		}
		vulkanDevice->createMeshDescriptorSets(descriptorSets);
		std::shared_ptr<VulkanTexture> vulkanTexture = std::dynamic_pointer_cast<VulkanTexture>(
			material->texture);
		if (!vulkanTexture)
		{
			TE_LOG(TE_RENDERING_LOG, TE_ERROR, "Could not set material: no valid vulkan texture\n");
			return;
		}
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vulkanDevice->updateMeshDescriptorSet(descriptorSets[i], vulkanTexture);
		}
	}

	void VulkanMesh::setMaterial(std::shared_ptr<Material> newMat)
	{
		if (!vulkanDevice)
		{
			TE_LOG(TE_RENDERING_LOG, TE_ERROR, "No valid vulkan device\n");
			return;
		}
		std::shared_ptr<VulkanTexture> vulkanTexture = std::dynamic_pointer_cast<VulkanTexture>(
			newMat->texture);
		if (!vulkanTexture)
		{
			TE_LOG(TE_RENDERING_LOG, TE_ERROR, "Could not set material: no valid vulkan texture\n");
			return;
		}
		material = newMat;
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vulkanDevice->updateMeshDescriptorSet(descriptorSets[i], vulkanTexture);
		}
	}

	void VulkanMesh::cleanup()
	{

	}

	const VkBuffer VulkanMesh::getVertexBuffer() const
	{
		return vertexBuffer;
	}

	const VkDeviceMemory VulkanMesh::getVertexBufferMemory() const
	{
		return vertexBufferMemory;
	}

	const VkBuffer VulkanMesh::getIndexBuffer() const
	{
		return indexBuffer;
	}

	const VkDeviceMemory VulkanMesh::getIndexBufferMemory() const
	{
		return indexBufferMemory;
	}

	size_t VulkanMesh::getIndexBufferSize() const
	{
		return indexBufferSize;
	}

	const std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT>& VulkanMesh::getDescriptorSets() const
	{
		return descriptorSets;
	}

} // namespace te