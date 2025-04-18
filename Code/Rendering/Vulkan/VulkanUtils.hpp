#ifndef _VULKAN_UTILS_HPP_
#define _VULKAN_UTILS_HPP_

#include "vulkan/vulkan.h"
#include "QueueFamilyIndices.hpp"
#include <vector>

namespace te
{

	uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
	
	/** Buffers and image creation and transition */
	void createBuffer(std::vector<QueueFamilyIndices>& queueFamilies, VkPhysicalDevice physicalDevice, VkDevice device,
		VkDeviceSize& size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
		VkBuffer& outBuffer, VkDeviceMemory& outBufferMemory);
	void createImage(VkPhysicalDevice physicalDevice, VkDevice device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& outImage, VkDeviceMemory& outImageMemory);
	VkImageView createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	void transitionImageLayout(VkDevice device, VkCommandPool commandPool, VkQueue queue,
		VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
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