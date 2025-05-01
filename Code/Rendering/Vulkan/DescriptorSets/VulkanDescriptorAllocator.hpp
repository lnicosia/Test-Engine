#ifndef _VULKAN_DESCRIPTOR_ALLOCATOR_HPP_
#define _VULKAN_DESCRIPTOR_ALLOCATOR_HPP_

#include "vulkan/vulkan.h"

#include <span>
#include <vector>

namespace te
{
	class VulkanDescriptorAllocator
	{

	public:

		struct PoolSize
		{
			VkDescriptorType type;
			float ratio;
		};

		void init(VkDevice device, uint32_t maxSets, std::span<PoolSize> inPoolSizes);
		void clearPools(VkDevice device);
		void cleanupPools(VkDevice device);

		VkDescriptorSet allocate(VkDevice device, VkDescriptorSetLayout layout);

	private:

		VkDescriptorPool getPool(VkDevice device);
		VkDescriptorPool createPool(VkDevice device, uint32_t setCount, std::span<PoolSize> inPoolSizes);

	private:
		
		std::vector<PoolSize> poolSizes{};
		std::vector<VkDescriptorPool> fullPools{};
		std::vector<VkDescriptorPool> readyPools{};
		uint32_t setsPerPool = 1;
	};
} // namespace te

#endif // _VULKAN_DESCRIPTOR_ALLOCATOR_HPP_