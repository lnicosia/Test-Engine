#ifndef _VULKAN_TEXTURE_HPP_
#define _VULKAN_TEXTURE_HPP_

#include "Texture.hpp"

#include "Rendering/Vulkan/QueueFamilyIndices.hpp"
#include "vulkan/vulkan.h"

namespace te
{
	struct VulkanOwnerData
	{
		std::vector<QueueFamilyIndices>* queueFamilies{};
		VkPhysicalDevice physicalDevice{};
		VkDevice device{};
		VkCommandPool transferPool{};
		VkCommandPool graphicsPool{};
		VkQueue transferQueue{};
		VkQueue graphicsQueue{};

		VulkanOwnerData()
		{
		}

		VulkanOwnerData(std::vector<QueueFamilyIndices>* queueFamilies,
			VkPhysicalDevice physicalDevice, VkDevice device,
			VkCommandPool transferPool, VkCommandPool graphicsPool,
			VkQueue transferQueue, VkQueue graphicsQueue):
			queueFamilies{queueFamilies},
			physicalDevice{physicalDevice}, device{device},
			transferPool{transferPool}, graphicsPool{graphicsPool},
			transferQueue{transferQueue}, graphicsQueue{graphicsQueue}
		{
		}

		VulkanOwnerData(VulkanOwnerData&& ref): queueFamilies{std::move(ref.queueFamilies)},
			physicalDevice{std::move(ref.physicalDevice)}, device{std::move(ref.device)},
			transferPool{std::move(ref.transferPool)}, graphicsPool{std::move(ref.graphicsPool)},
			transferQueue{std::move(ref.transferQueue)}, graphicsQueue{std::move(ref.graphicsQueue)}
		{
		}

		VulkanOwnerData& operator=(VulkanOwnerData&& ref)
		{
			queueFamilies = std::move(ref.queueFamilies);
			physicalDevice = std::move(ref.physicalDevice);
			device = std::move(ref.device);
			transferPool = std::move(ref.transferPool);
			graphicsPool = std::move(ref.graphicsPool);
			transferQueue = std::move(ref.transferQueue);
			graphicsQueue = std::move(ref.graphicsQueue);
			return *this;
		}
	};

	class VulkanTexture : public Texture
	{
	public:
		VulkanTexture() = delete;
		VulkanTexture(VulkanOwnerData&& ownerData, const std::string& path);
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