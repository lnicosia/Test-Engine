#ifndef _VULKAN_UTILS_HPP_
#define _VULKAN_UTILS_HPP_

#include "vulkan/vulkan.h"
#include "QueueFamilyIndices.hpp"
#include <vector>

namespace te
{

	uint32_t findMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
	
	void createBuffer(std::vector<QueueFamilyIndices>& queueFamilies, VkPhysicalDevice& ownerPhysicalDevice, VkDevice& ownerDevice,
		VkDeviceSize& size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
		VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	VkCommandBuffer beginSingleTimeCommands(VkCommandPool& commandPool, VkDevice device);
	void endSingleTimeCommands(VkCommandBuffer& commandBuffer, VkCommandPool& commandPool, VkDevice device, VkQueue queue);
	/** Buffer commands */
	void setupCommandBuffer(); // TODO
	void addCommandToBuffer(VkCommandBuffer& commandBuffer); // TODO
	void flushCommandBuffer(VkCommandBuffer& commandBuffer); // TODO

	VkImageView createImageView(VkImage& image, VkFormat format, VkDevice& device);

} // namespace te

#endif // _VULKAN_UTILS_HPP_