#include "VulkanMesh.hpp"

#include "Assets/Importers/Assimp/AssimpImporter.hpp"
#include "Assets/Textures/VulkanTexture.hpp"
#include "Assets/AssetManager.hpp"

#include "Debug/Log.hpp"

namespace te
{
	VulkanMesh::VulkanMesh(const std::string& path, VulkanDevice* vulkanDevice):
		vulkanDevice{vulkanDevice}, MeshInternal(path)
	{
		
	}

	VulkanMesh::VulkanMesh(VulkanDevice* vulkanDevice, const MeshGeometry& geometry): MeshInternal(),
		vulkanDevice{vulkanDevice}
	{
		setupVertices(geometry.vertices);
		setupIndices(geometry.indices);
		setupUniformBuffers();
		setupDescriptors();
	}

	void VulkanMesh::load()
	{
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

	void VulkanMesh::setup(AssimpImporter& importer)
	{
		// TODO
		for (auto aiTexture : importer.getAiTextures())
		{

		}
		for (const MeshGeometry& geometry: importer.meshes)
		{
			setupVertices(geometry.vertices);
			setupIndices(geometry.indices);
		}
		setupUniformBuffers();
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

	void VulkanMesh::setupUniformBuffers()
	{
		vulkanDevice->createUniformBuffers(uniformBuffers, uniformBufferMemories);
	}

	void VulkanMesh::setupDescriptors()
	{
		if (!vulkanDevice)
		{
			TE_LOG(TE_RENDERING_LOG, TE_ERROR, "No valid vulkan device\n");
			return;
		}
		vulkanDevice->createDescriptorSets(descriptorSets);
		std::shared_ptr<VulkanTexture> texture;
		if (!textures.empty())
		{
			texture = std::dynamic_pointer_cast<VulkanTexture>(textures[0]);
		}
		else
		{
			texture = std::dynamic_pointer_cast<VulkanTexture>(AssetManager::getInstance().getDefaultTexture());
		}
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vulkanDevice->updateDescriptorSet(descriptorSets[i], uniformBuffers[i], texture);
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

} // namespace te