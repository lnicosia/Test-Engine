#include "VulkanTexture.hpp"

#include "Rendering/Vulkan/VulkanUtils.hpp"
#include "Rendering/Vulkan/VulkanDevice.hpp"
#include "Debug/Exception.hpp"
#include "Debug/Log.hpp"

#include <cstring>
#include <memory>

namespace te
{
	VulkanTexture::VulkanTexture(const std::string& path, std::string&& textureType, VulkanDevice* vulkanDevice):
		Texture{path, std::move(textureType)}, vulkanDevice{vulkanDevice}
	{
		setup(path);
	}

	VulkanTexture::VulkanTexture(const std::string& path, unsigned char* pixels, unsigned int size,
		std::string&& textureType, VulkanDevice* vulkanDevice):
		Texture{path, pixels, size, std::move(textureType)}, vulkanDevice{vulkanDevice}
	{
		setup(path);
	}

	VulkanTexture::VulkanTexture(const std::string& path, VulkanDevice* vulkanDevice):
		Texture{path}, vulkanDevice{vulkanDevice}
	{
		setup(path);
	}

	VulkanTexture::VulkanTexture(VulkanTexture&& ref): Texture{std::move(ref)}, vulkanDevice{std::move(ref.vulkanDevice)}
	{
	}

	void VulkanTexture::setup(const std::string& path)
	{
		vulkanDevice->createTextureImage(path, textureImage, textureImageMemory, mipLevels);
		textureImageView = vulkanDevice->createImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB,
			VK_IMAGE_ASPECT_COLOR_BIT, mipLevels);
		textureSampler = vulkanDevice->createTextureSampler(mipLevels);

		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureImageView;
		imageInfo.sampler = textureSampler;
	}

	VulkanTexture& VulkanTexture::operator=(VulkanTexture&& ref)
	{
		vulkanDevice = std::move(ref.vulkanDevice);
		return *this;
	}

	VulkanTexture::~VulkanTexture()
	{
	}

	void VulkanTexture::cleanUp()
	{
		vulkanDevice->cleanUpTexture(textureImage, textureImageMemory, textureImageView, textureSampler);
	}

	void VulkanTexture::createTextureImageView()
	{
	}

	VkImageView VulkanTexture::getImageView()
	{
		return textureImageView;
	}

	VkSampler VulkanTexture::getSampler()
	{
		return textureSampler;
	}

	const std::string VulkanTexture::getAssetType() const
	{
		return {"Vulkan texture"};
	}

} // namespace te