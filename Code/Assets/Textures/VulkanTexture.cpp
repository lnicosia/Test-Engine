#include "VulkanTexture.hpp"

#include "Rendering/Vulkan/VulkanUtils.hpp"
#include "Debug/Exception.hpp"
#include "Debug/Log.hpp"
#include "stb_image.h"

#include <cstring>

namespace te
{
	VulkanTexture::VulkanTexture(VulkanOwnerData&& ownerData, const std::string& path): ownerData{std::move(ownerData)}, Texture{path}
	{
		int texWidth, texHeight, texChannels;
		std::string filePath{Logger::ROOT_DIR_PATH + "Resources/Textures/bigdoor2.bmp"};
		stbi_uc* pixels = stbi_load(filePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		if (!pixels)
		{
			ThrowException("Could not import " + filePath + "\n");
		}
		VkDeviceSize imageSize = texWidth * texHeight * 4;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		createBuffer(*ownerData.queueFamilies, ownerData.physicalDevice, ownerData.device,
			imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferMemory);
		void* data;

		vkMapMemory(ownerData.device, stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(imageSize));
		vkUnmapMemory(ownerData.device, stagingBufferMemory);
		
		stbi_image_free(pixels);
		createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

		transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, ownerData.transferPool, ownerData.transferQueue);
		copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
		transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, ownerData.graphicsPool, ownerData.graphicsQueue);
		
		vkDestroyBuffer(ownerData.device, stagingBuffer, nullptr);
		vkFreeMemory(ownerData.device, stagingBufferMemory, nullptr);

		createTextureImageView();
		createTextureSampler();
	}

	VulkanTexture::VulkanTexture(VulkanTexture&& ref): Texture{std::move(ref)}, 
		textureImage{std::move(ref.textureImage)}, textureImageMemory{std::move(ref.textureImageMemory)},
		textureImageView{std::move(ref.textureImageView)}, textureSampler{std::move(ref.textureSampler)},
		ownerData{std::move(ref.ownerData)}
	{
	}

	VulkanTexture& VulkanTexture::operator=(VulkanTexture&& ref)
	{
		textureImage= std::move(ref.textureImage);
		textureImageMemory = std::move(ref.textureImageMemory);
		textureImageView = std::move(ref.textureImageView);
		textureSampler = std::move(ref.textureSampler);
		ownerData = std::move(ref.ownerData);
		return *this;
	}

	VulkanTexture::~VulkanTexture()
	{
	}

	void VulkanTexture::cleanUp()
	{
		vkDestroySampler(ownerData.device, textureSampler, nullptr);
		vkDestroyImageView(ownerData.device, textureImageView, nullptr);
		vkDestroyImage(ownerData.device, textureImage, nullptr);
		vkFreeMemory(ownerData.device, textureImageMemory, nullptr); 
	}

	void VulkanTexture::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.flags = 0; // Optional
		if (vkCreateImage(ownerData.device, &imageInfo, nullptr, &textureImage) != VK_SUCCESS)
		{
			ThrowException("Failed to create image!\n");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(ownerData.device, textureImage, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(ownerData.physicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if (vkAllocateMemory(ownerData.device, &allocInfo, nullptr, &textureImageMemory) != VK_SUCCESS)
		{
			ThrowException("Failed to allocate image memory!\n");
		}

		vkBindImageMemory(ownerData.device, textureImage, textureImageMemory, 0);
	}

	void VulkanTexture::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout,
		VkCommandPool commandPool, VkQueue queue)
	{
		VkCommandBuffer commandBuffer = beginSingleTimeCommands(commandPool, ownerData.device);

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else
		{
			ThrowException("Unsupported layout transition\n");
		}

		vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		endSingleTimeCommands(commandBuffer, commandPool, ownerData.device, queue);
	}

	void VulkanTexture::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
	{
		VkCommandBuffer commandBuffer = beginSingleTimeCommands(ownerData.transferPool, ownerData.device);

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		
		region.imageOffset = {0, 0, 0};
		region.imageExtent = {width, height, 1};

		vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, &region);

		endSingleTimeCommands(commandBuffer, ownerData.transferPool, ownerData.device, ownerData.transferQueue);
	}

	void VulkanTexture::createTextureImageView()
	{
		textureImageView = createImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB, ownerData.device);
	}

	void VulkanTexture::createTextureSampler()
	{
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE; // TODO: user choice!

		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(ownerData.physicalDevice, &properties);
		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy; // TODO: user choice!

		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;

		if (vkCreateSampler(ownerData.device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS)
		{
			ThrowException("Failed to create texture sampler!\n");
		}
	}

	VkImageView VulkanTexture::getImageView()
	{
		return textureImageView;
	}

	VkSampler VulkanTexture::getSampler()
	{
		return textureSampler;
	}

	void VulkanTexture::setOwnerDevice(VkDevice inDevice)
	{
		ownerData.device = inDevice;
	}
	
	const std::string VulkanTexture::getAssetType() const
	{
		return {"Vulkan texture"};
	}

} // namespace te