#include "VulkanRenderer.hpp"
#include "SDLWindow.hpp"
#include "Debug/Assert.hpp"

#include <cstring>

namespace te
{
	VulkanRenderer::VulkanRenderer(): Renderer(RendererType::TE_VULKAN, WindowManager::TE_SDL),
		instance(), debugMessenger()
	{
		std::shared_ptr<SDLWindow> winPtr = std::shared_ptr<SDLWindow>(
			new SDLWindow(1600, 900, RendererType::TE_VULKAN));
		window = winPtr;

		window->loadVulkan();
		initVulkan();
	}

	VulkanRenderer::VulkanRenderer(WindowManager wManager): Renderer(RendererType::TE_VULKAN, wManager),
		instance(), debugMessenger()
	{
		// TODO
		uninplemented();
		if (wManager != WindowManager::TE_SDL)
			ThrowException("Only SDL is implemented for now");
	}

	VulkanRenderer::~VulkanRenderer()
	{
		if (enableValidationLayers)
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		vkDestroyInstance(instance, nullptr);
	}

	void VulkanRenderer::initVulkan()
	{
		createInstance();
		if (enableValidationLayers)
			setupDebugMessenger();
	}

	void VulkanRenderer::createInstance()
	{
		if (enableValidationLayers && !checkValidationLayerSupport())
			ThrowException("Validation layers are not available");

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
		
		uint32_t availableExtensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(availableExtensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, extensions.data());

		LOG(TE_RENDERING_LOG, TE_VERBOSE, "Available Vulkan extensions:\n");
		for (auto extension : extensions)
		{
			LOG(TE_RENDERING_LOG, TE_VERBOSE, "\t%s\n", extension.extensionName);
		}

		uint32_t neededExtensionCount = 0;
		std::vector<const char*> extensionNames;
		window->getVulkanInstanceExtensions(&neededExtensionCount, extensionNames, enableValidationLayers);
		createInfo.enabledExtensionCount = neededExtensionCount;
		createInfo.ppEnabledExtensionNames = extensionNames.data();

		if (enableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(this->validationLayers.size());
			createInfo.ppEnabledLayerNames = this->validationLayers.data();
			VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

			populateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else
		{
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		LOG(TE_RENDERING_LOG, TE_LOG, "Creating Vulkan instance\n");
		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			ThrowException("Failed to create Vulkan instance!");
		}
	}

	bool VulkanRenderer::checkValidationLayerSupport()
	{
		uint32_t layerCount;

		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : this->validationLayers)
		{
			bool found = false;
			
			for (const auto& layerProperty : availableLayers)
			{
				if (!strcmp(layerName, layerProperty.layerName))
				{
					found = true;
					break;
				}
			}
			if (!found)
				return false;
		}
		return true;
	}

	void VulkanRenderer::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
		createInfo.pUserData = nullptr;
	}

	void VulkanRenderer::setupDebugMessenger()
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo;

		populateDebugMessengerCreateInfo(createInfo);
		if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
			ThrowException("Failed to setup vulkan debug message\n");
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanRenderer::debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		//LOG(TE_RENDERING_LOG, TE_LOG, "Validation layer: %s\n", pCallbackData->pMessage);
		if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			LOG(TE_RENDERING_LOG, TE_WARNING, "Vulkan warning: %s\n", pCallbackData->pMessage);
		}
		else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		{
			LOG(TE_RENDERING_LOG, TE_ERROR, "Vulkan error: %s\n", pCallbackData->pMessage);
		}
		else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
		{
			LOG(TE_RENDERING_LOG, TE_LOG, "Vulkan log: %s\n", pCallbackData->pMessage);
		}
		return VK_FALSE;
	}

	VkResult VulkanRenderer::CreateDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		else
		{
			LOG(TE_RENDERING_LOG, TE_ERROR, "Could not load 'vkCreateDebugUtilsMessengerEXT'\n");
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void VulkanRenderer::DestroyDebugUtilsMessengerEXT(
		VkInstance instance,
		VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
			func(instance, debugMessenger, pAllocator);
		else
		{
			LOG(TE_RENDERING_LOG, TE_ERROR, "Could not load 'vkDestroyDebugUtilsMessengerEXT'\n");
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
		uninplemented();
	}

}