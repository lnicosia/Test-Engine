#define NOMINMAX

#include "VulkanRenderer.hpp"
#include "VulkanUtils.hpp"
#include "VulkanDevice.hpp"
#include "VulkanMesh.hpp"
#include "../Window/SDLWindow.hpp"
#include "../Debug/Assert.hpp"
#include "../Platform.hpp"
#include "../Shaders/Shader.hpp"
#include "../Assets/CheckFileType.hpp"
#include "../Assets/AssetManager.hpp"
#include "Maths/Tests/MatrixTest.hpp"
#include "Assets/AssetManager.hpp"

#ifdef TE_WINDOWS
# define VK_USE_PLATFORM_WIN32_KHR
# include "vulkan/vulkan.h"
#endif

namespace te
{
	VulkanRenderer::VulkanRenderer(): Renderer(RendererType::TE_VULKAN, WindowAPI::TE_SDL)
	{
		std::shared_ptr<SDLWindow> winPtr = std::make_shared<SDLWindow>(
			1600, 900, RendererType::TE_VULKAN);
		window = winPtr;

		window->loadVulkan();
		initCameraBindings();

		vulkanDevice = std::make_shared<VulkanDevice>(winPtr);
		for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vulkanDevice->updateCameraDescriptors(i);
		}
		device = vulkanDevice;
		AssetManager::getInstance().loadDefaultTexture<VulkanTexture>(Logger::getRootDirPath() + DEFAULT_TEXTURE_PATH, vulkanDevice.get());
	}

	VulkanRenderer::VulkanRenderer(WindowAPI wManager): Renderer(RendererType::TE_VULKAN, wManager)
	{
		// TODO
		unimplemented();
		if (wManager != WindowAPI::TE_SDL)
		{
			ThrowException("Only SDL is implemented for now\n");
		}
	}

	VulkanRenderer::~VulkanRenderer()
	{
	}

	void VulkanRenderer::render()
	{
		Renderer::render();

		if (vkDeviceWaitIdle(vulkanDevice->getDevices()[0]) != VK_SUCCESS)
		{
			TE_LOG(TE_RENDERING_LOG, TE_ERROR, "Could not wait for device to become idle\n");
		}
		// TODO
	}

	void VulkanRenderer::renderText(const char* text, std::shared_ptr<Font> font, Point2<int> pos, int size)
	{
		// TODO
		unimplemented();
	}
	
	std::shared_ptr<MeshInternal> VulkanRenderer::createSolidMesh(const MeshGeometry& geometry)
	{
		return std::make_shared<te::VulkanMesh>(vulkanDevice.get(), geometry);
	}

	std::shared_ptr<MeshInternal> VulkanRenderer::loadMesh(const std::string& path)
	{
		std::shared_ptr<MeshInternal> newMesh =
			te::AssetManager::getInstance().loadAsset<te::VulkanMesh>(
			path, vulkanDevice.get());
		
		return newMesh;
	}

	std::shared_ptr<Texture> VulkanRenderer::loadTexture(const std::string& path)
	{
		return te::AssetManager::getInstance().loadAsset<te::VulkanTexture>(
			path, vulkanDevice.get());
	}

	std::shared_ptr<VulkanDevice> VulkanRenderer::getVulkanDevice()
	{
		return vulkanDevice;
	}
}