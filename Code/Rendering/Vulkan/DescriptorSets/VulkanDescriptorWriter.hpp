#ifndef _VULKAN_DESCRIPTOR_WRITER_HPP_
#define _VULKAN_DESCRIPTOR_WRITER_HPP_

#include "vulkan/vulkan.h"

#include <deque>
#include <vector>

namespace te
{
	class VulkanDescriptorWriter
	{

	public:

		void writeCombinedImage(uint32_t binding, VkImageView imageView, VkSampler sampler, VkImageLayout layout, VkDescriptorType type);
		void writeImage(uint32_t binding, VkImageView imageView, VkImageLayout layout, VkDescriptorType type);
		void writeSampler(uint32_t binding, VkSampler sampler, VkImageLayout layout, VkDescriptorType type);
		void writeBuffer(uint32_t binding, VkBuffer buffer, size_t size, size_t offset, VkDescriptorType type); 

		void clear();
		void updateSet(VkDevice device, VkDescriptorSet set);

	private:

		std::deque<VkDescriptorImageInfo> imageInfos;
		std::deque<VkDescriptorBufferInfo> bufferInfos;
		std::vector<VkWriteDescriptorSet> writes;

	};
} // namespace te

#endif // _VULKAN_DESCRIPTOR_WRITER_HPP