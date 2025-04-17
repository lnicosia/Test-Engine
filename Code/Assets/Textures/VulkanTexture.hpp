#ifndef _VULKAN_TEXTURE_HPP_
#define _VULKAN_TEXTURE_HPP_

#include "Texture.hpp"

#include "Rendering/Vulkan/QueueFamilyIndices.hpp"
#include "vulkan/vulkan.h"

namespace te
{
	struct VulkanOwnerData
	{
		std::vector<QueueFamilyIndices>* queueFamilies;
		VkPhysicalDevice physicalDevice{};
		VkDevice device{};
		VkCommandPool transferPool{};
		VkCommandPool graphicsPool{};
		VkQueue transferQueue{};
		VkQueue graphicsQueue{};
	};

	class VulkanTexture : public Texture
	{
	public:
		VulkanTexture() = delete;
		VulkanTexture(VulkanOwnerData ownerData, const std::string& path);
		VulkanTexture(const VulkanTexture& ref) = delete;
		VulkanTexture(VulkanTexture&& ref);
		VulkanTexture& operator=(VulkanTexture&& ref);
		VulkanTexture& operator=(const VulkanTexture& ref) = delete;
		~VulkanTexture();

		void cleanUp();
		
		/** */
		void setOwnerDevice(VkDevice inDevice);
		
		VkImageView getImageView();
		VkSampler getSampler();

	private:
		void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
			VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout,
			VkCommandPool commandPool, VkQueue queue);
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		void createTextureImageView();
		void createTextureSampler();

	protected:
		virtual const std::string getAssetType() const override;

	private:
		VkImage textureImage{};
		VkDeviceMemory textureImageMemory{};
		VkImageView textureImageView{};
		VkSampler textureSampler{};

		VulkanOwnerData ownerData{};
	};
} // namespace te

#endif // _VULKAN_TEXTURE_HPP_