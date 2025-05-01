#include "VulkanDescriptorWriter.hpp"

namespace te
{
	void VulkanDescriptorWriter::writeSampler(uint32_t binding, VkSampler sampler, VkImageLayout layout, VkDescriptorType type)
	{
		const VkDescriptorImageInfo& samplerInfo = imageInfos.emplace_back(
			VkDescriptorImageInfo
			{
				.sampler = sampler,
				.imageLayout = layout
			}
		);

		VkWriteDescriptorSet write
		{
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = VK_NULL_HANDLE,
			.dstBinding = binding,
			.descriptorCount = 1,
			.descriptorType = type,
			.pImageInfo = &samplerInfo
		};

		writes.push_back(write);
	}

	void VulkanDescriptorWriter::writeImage(uint32_t binding, VkImageView imageView, VkImageLayout layout, VkDescriptorType type)
	{
		const VkDescriptorImageInfo& imageInfo = imageInfos.emplace_back(
			VkDescriptorImageInfo
			{
				.imageView = imageView,
				.imageLayout = layout
			}
		);

		VkWriteDescriptorSet write
		{
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = VK_NULL_HANDLE,
			.dstBinding = binding,
			.descriptorCount = 1,
			.descriptorType = type,
			.pImageInfo = &imageInfo
		};

		writes.push_back(write);
	}

	void VulkanDescriptorWriter::writeCombinedImage(uint32_t binding, VkImageView imageView, VkSampler sampler, VkImageLayout layout, VkDescriptorType type)
	{
		const VkDescriptorImageInfo& imageInfo = imageInfos.emplace_back(
			VkDescriptorImageInfo
			{
				.sampler = sampler,
				.imageView = imageView,
				.imageLayout = layout
			}
		);

		VkWriteDescriptorSet write
		{
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = VK_NULL_HANDLE,
			.dstBinding = binding,
			.descriptorCount = 1,
			.descriptorType = type,
			.pImageInfo = &imageInfo
		};

		writes.push_back(write);
	}

	void VulkanDescriptorWriter::writeBuffer(uint32_t binding, VkBuffer buffer, size_t size, size_t offset, VkDescriptorType type)
	{
		const VkDescriptorBufferInfo& bufferInfo = bufferInfos.emplace_back(
			VkDescriptorBufferInfo
			{
				.buffer = buffer,
				.offset = offset,
				.range = size
			}
		);

		VkWriteDescriptorSet write
		{
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = VK_NULL_HANDLE,
			.dstBinding = binding,
			.descriptorCount = 1,
			.descriptorType = type,
			.pBufferInfo = &bufferInfo
		};

		writes.push_back(write);
	}

	void VulkanDescriptorWriter::clear()
	{
		imageInfos.clear();
		bufferInfos.clear();
		writes.clear();
	}

	void VulkanDescriptorWriter::updateSet(VkDevice device, VkDescriptorSet set)
	{
		for (auto& write : writes)
		{
			write.dstSet = set;
		}

		vkUpdateDescriptorSets(device, static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
	}

} // namespace te