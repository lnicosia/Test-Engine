#ifndef _VULKAN_MESH_HPP_
#define _VULKAN_MESH_HPP_

#include "../MeshInternal.hpp"

#include "Rendering/Vulkan/VulkanDevice.hpp"
#include "Rendering/Vulkan/VulkanMaterial.hpp"
#include "Maths/Matrix.hpp"

#include "vulkan/vulkan.h"

class VulkanDevice;

namespace te
{

	class VulkanMesh : public MeshInternal
	{
	
	public:

		VulkanMesh(const std::string& path, VulkanDevice* device);
		VulkanMesh(VulkanDevice* vulkanDevice, const MeshGeometry& geometry);

		const VkBuffer getVertexBuffer() const;
		const VkDeviceMemory getVertexBufferMemory() const;
		const VkBuffer getIndexBuffer() const;
		const VkDeviceMemory getIndexBufferMemory() const;
		size_t getIndexBufferSize() const;
		const std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT>& getDescriptorSets() const;

		void setMaterial(std::shared_ptr<Material> newMat) override;
		
	protected:

		void setup(AssimpImporter& importer) override;
		void setupVertices(const std::vector<Vertex>& vertices) override;
		void setupIndices(const std::vector<uint32_t>& indices) override;
		void cleanup() override;

	private:

		void setupDescriptors();

	private:

		/** Vertex buffer */
		VkBuffer vertexBuffer{};
		VkDeviceMemory vertexBufferMemory{};

		/** Index buffer */
		VkBuffer indexBuffer{};
		VkDeviceMemory indexBufferMemory{};
		size_t indexBufferSize;

		std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> descriptorSets;

		VulkanDevice* vulkanDevice = nullptr;

	};

} // namespace te


#endif // _VULKAN_MESH_HPP_