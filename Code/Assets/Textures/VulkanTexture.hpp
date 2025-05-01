#ifndef _VULKAN_TEXTURE_HPP_
#define _VULKAN_TEXTURE_HPP_

#include "Texture.hpp"
#include "Rendering/Vulkan/VulkanDevice.hpp"
#include "vulkan/vulkan.h"

class VulkanDevice;

namespace te
{

	class VulkanTexture : public Texture
	{
	public:
		VulkanTexture() = delete;
		VulkanTexture(const std::string& path, VulkanDevice* vulkanDevice);
		VulkanTexture(const std::string& path, std::string&& textureType, VulkanDevice* vulkanDevice);
		VulkanTexture(const std::string& path, unsigned char* pixels, unsigned int size, std::string&& textureType,
			VulkanDevice* vulkanDevice);
		VulkanTexture(const VulkanTexture& ref) = delete;
		VulkanTexture(VulkanTexture&& ref);
		VulkanTexture& operator=(VulkanTexture&& ref);
		VulkanTexture& operator=(const VulkanTexture& ref) = delete;
		~VulkanTexture();

		void cleanUp() override;
		
		/** */
		void setOwnerDevice(VkDevice inDevice);
		
		VkImageView getImageView();
		VkSampler getSampler();

	private:
		void setup(const std::string& path) override;
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
		VkDescriptorImageInfo imageInfo{};
		VkDescriptorSet descriptorSet{};

		VulkanDevice* vulkanDevice = nullptr;
	};
} // namespace te

#endif // _VULKAN_TEXTURE_HPP_