#include "VulkanDescriptorAllocator.hpp"

#include "Debug/Exception.hpp"
#include "Debug/Log.hpp"

namespace te
{
	
	void VulkanDescriptorAllocator::init(VkDevice device, uint32_t maxSets, std::span<PoolSize> inPoolSizes)
	{
		poolSizes.clear();
		poolSizes.resize(inPoolSizes.size());

		for (size_t i = 0; i < inPoolSizes.size(); i++)
		{
			poolSizes[i] = inPoolSizes[i];
		}

		VkDescriptorPool newPool = createPool(device, maxSets, inPoolSizes);

		setsPerPool = static_cast<uint32_t>(maxSets * 1.5);

		
		readyPools.emplace_back(newPool);
	}
	
	void VulkanDescriptorAllocator::clearPools(VkDevice device)
	{
		for (VkDescriptorPool readyPool : readyPools)
		{
			vkResetDescriptorPool(device, readyPool, 0);
		}
		for (VkDescriptorPool fullPool : fullPools)
		{
			vkResetDescriptorPool(device, fullPool, 0);
			readyPools.push_back(fullPool);
		}
		fullPools.clear();
	}

	void VulkanDescriptorAllocator::cleanupPools(VkDevice device)
	{
		for (VkDescriptorPool readyPool : readyPools)
		{
			vkDestroyDescriptorPool(device, readyPool, nullptr);
		}
		readyPools.clear();
		for (VkDescriptorPool fullPool : fullPools)
		{
			vkDestroyDescriptorPool(device, fullPool, nullptr);
		}
		fullPools.clear();
	}

	VkDescriptorSet VulkanDescriptorAllocator::allocate(VkDevice device, VkDescriptorSetLayout layout)
	{
		VkDescriptorPool pool = getPool(device);

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &layout;

		VkDescriptorSet newSet{};
		VkResult res = vkAllocateDescriptorSets(device, &allocInfo, &newSet);

		if (res == VK_ERROR_OUT_OF_POOL_MEMORY || res == VK_ERROR_FRAGMENTED_POOL)
		{
			fullPools.push_back(pool);

			pool = getPool(device);
			allocInfo.descriptorPool = pool;

			if (vkAllocateDescriptorSets(device, &allocInfo, &newSet) != VK_SUCCESS)
			{
				ThrowException("Could not allocate descriptor sets\n");
			}
		}
		readyPools.push_back(pool);

		return newSet;
	}

	VkDescriptorPool VulkanDescriptorAllocator::getPool(VkDevice device)
	{
		VkDescriptorPool newPool;

		if (!readyPools.empty())
		{
			newPool = readyPools.back();
			readyPools.pop_back();
		}
		else
		{
			newPool = createPool(device, setsPerPool, poolSizes);
			setsPerPool = static_cast<uint32_t>(setsPerPool * 1.5);
			if (setsPerPool > 4096)
			{
				setsPerPool = 4096;
			}
		}
		return newPool;
	}

	VkDescriptorPool VulkanDescriptorAllocator::createPool(VkDevice device, uint32_t setCount, std::span<PoolSize> inPoolSizes)
	{
		VkDescriptorPool newPool{};

		std::vector<VkDescriptorPoolSize> descriptorPoolSizes{};
		descriptorPoolSizes.reserve(inPoolSizes.size());
		
		for (size_t i = 0; i < inPoolSizes.size(); i++)
		{
			descriptorPoolSizes[i] = VkDescriptorPoolSize{
				inPoolSizes[i].type, static_cast<uint32_t>(inPoolSizes[i].ratio * setCount)};
		}

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.maxSets = setCount;
		poolInfo.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size());
		poolInfo.pPoolSizes = descriptorPoolSizes.data();

		if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &newPool) != VK_SUCCESS)
		{
			ThrowException("Could not create descriptor pool\n");
		}
		return newPool;
	}
} // namespace te