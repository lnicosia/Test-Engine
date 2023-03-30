#include "VulkanRenderer.hpp"
#include "SDLWindow.hpp"

namespace te
{
	VulkanRenderer::VulkanRenderer(): Renderer(RendererType::TE_VULKAN, WindowManager::TE_SDL)
	{
		std::shared_ptr<SDLWindow> winPtr = std::shared_ptr<SDLWindow>(
			new SDLWindow(1600, 900, RendererType::TE_VULKAN));
		window = winPtr;

		window->loadVulkan();
		initVulkan();
	}

	VulkanRenderer::VulkanRenderer(WindowManager wManager): Renderer(RendererType::TE_VULKAN, wManager)
	{
		// TODO
		if (wManager != WindowManager::TE_SDL)
			throw std::runtime_error("Only SDL is implemented for now");
	}

	VulkanRenderer::~VulkanRenderer()
	{
		vkDestroyInstance(instance, nullptr);
	}

	void VulkanRenderer::initVulkan()
	{
		createInstance();
	}

	void VulkanRenderer::createInstance()
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Test Engine - Vulkan";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Test Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		/* List available extensions 
		   TODO: print or not?
		*/
		
		/*uint32_t availableExtensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(availableExtensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, extensions.data());

		LOG("Avaiable Vulkan extensions:\n");
		for (auto extension : extensions)
		{
			LOG("\t%s\n", extension.extensionName);
		}*/


		createInfo.enabledLayerCount = 0;
		uint32_t neededExtensionCount = 0;
		std::vector<const char*> extensionNames;
		window->getVulkanInstanceExtensions(&neededExtensionCount, extensionNames);
		createInfo.enabledExtensionCount = neededExtensionCount;
		createInfo.ppEnabledExtensionNames = extensionNames.data();

		LOG(TE_RESOURCE_LOG, "Creating Vulkan instance\n");
		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create Vulkan instance!");
		}
	}

	void VulkanRenderer::render()
	{
		while (running == true)
		{
			if (window->handleEvents() == 1)
				running = false;
		}
		// TODO
	}

	void VulkanRenderer::renderText(const char* text, std::shared_ptr<Font> font, Point2<int> pos, int size)
	{
		// TODO
	}
}