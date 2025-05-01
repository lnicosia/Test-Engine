#ifndef _VULKAN_UTILS_HPP_
#define _VULKAN_UTILS_HPP_

#include "vulkan/vulkan.h"
#include "QueueFamilyIndices.hpp"
#include <vector>

namespace te
{

	uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

	bool hasStencilComponent(VkFormat format);

	/** Single use buffer commands */
	VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool, VkDevice device);
	void endSingleTimeCommands(VkDevice device, VkQueue queue, VkCommandBuffer commandBuffer, VkCommandPool commandPool);
	/** Buffer commands */
	void setupCommandBuffer(); // TODO
	void addCommandToBuffer(VkCommandBuffer commandBuffer); // TODO
	void flushCommandBuffer(VkCommandBuffer commandBuffer); // TODO


} // namespace te

#endif // _VULKAN_UTILS_HPP_