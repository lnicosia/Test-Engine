#define NOMINMAX

#include "VulkanRenderer.hpp"
#include "SDLWindow.hpp"
#include "Debug/Assert.hpp"
#include "Platform.hpp"
#include "Shaders/Shader.hpp"
#include "Assets/CheckFileType.hpp"

#ifdef TE_WINDOWS
# define VK_USE_PLATFORM_WIN32_KHR
# include "vulkan/vulkan.h"
#endif

#include <cstring>
#include <set>
#include <limits>

namespace te
{
	VulkanRenderer::VulkanRenderer(): Renderer(RendererType::TE_VULKAN, WindowManager::TE_SDL),
		vulkanInstance(), debugMessenger()
	{
		std::shared_ptr<SDLWindow> winPtr = std::shared_ptr<SDLWindow>(
			new SDLWindow(1600, 900, RendererType::TE_VULKAN));
		window = winPtr;

		window->loadVulkan();
		initVulkan();
	}

	VulkanRenderer::VulkanRenderer(WindowManager wManager): Renderer(RendererType::TE_VULKAN, wManager),
		vulkanInstance(), debugMessenger()
	{
		// TODO
		uninplemented();
		if (wManager != WindowManager::TE_SDL)
		{
			ThrowException("Only SDL is implemented for now\n");
		}
	}

	VulkanRenderer::~VulkanRenderer()
	{
		/* TODO: handle multiple logical devices */
		vkDestroySemaphore(devices[0], imageAvailableSemaphore, nullptr);
		vkDestroySemaphore(devices[0], renderFinishedSemaphore, nullptr);
		vkDestroyFence(devices[0], inFlightFence, nullptr);
		vkDestroyCommandPool(devices[0], commandPool, nullptr);
		for (auto framebuffer : framebuffers)
		{
			vkDestroyFramebuffer(devices[0], framebuffer, nullptr);
		}
		vkDestroyPipeline(devices[0], graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(devices[0], pipelineLayout, nullptr);
		vkDestroyRenderPass(devices[0], renderPass, nullptr);
		for (auto imageView : swapChainImageViews)
		{
			vkDestroyImageView(devices[0], imageView, nullptr);
		}
		vkDestroySwapchainKHR(devices[0], swapChain, nullptr);
		vkDestroyDevice(devices[0], nullptr);
		vkDestroySurfaceKHR(vulkanInstance, surface, nullptr);
		if (enableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(vulkanInstance, debugMessenger, nullptr);
		}
		vkDestroyInstance(vulkanInstance, nullptr);
	}

	void VulkanRenderer::initVulkan()
	{
		LOG(TE_RENDERING_LOG, TE_LOG, "Creating Vulkan instance\n");
		createInstance();
		if (enableValidationLayers)
		{
			setupDebugMessenger();
		}
		LOG(TE_RENDERING_LOG, TE_LOG, "Creating platform specific surface\n");
		window->createVulkanSurface(vulkanInstance, &surface);
		LOG(TE_RENDERING_LOG, TE_LOG, "Selecting physical device\n");
		selectPhysicalDevices();
		LOG(TE_RENDERING_LOG, TE_LOG, "Creating logical device\n");
		createLogicalDevices();
		LOG(TE_RENDERING_LOG, TE_LOG, "Creating swap chain\n");
		createSwapChain();
		LOG(TE_RENDERING_LOG, TE_LOG, "Creating image views\n");
		createImageViews();
		LOG(TE_RENDERING_LOG, TE_LOG, "Creating render pass\n");
		createRenderPass();
		LOG(TE_RENDERING_LOG, TE_LOG, "Creating graphics pipeline\n");
		createGraphicsPipeline();
		LOG(TE_RENDERING_LOG, TE_LOG, "Creating frame buffers\n");
		createFramebuffers();
		LOG(TE_RENDERING_LOG, TE_LOG, "Creating command pools\n");
		createCommandPool();
		LOG(TE_RENDERING_LOG, TE_LOG, "Creating command buffer\n");
		createCommandBuffer();
		LOG(TE_RENDERING_LOG, TE_LOG, "Creating synchronization objects\n");
		createSyncObjects();
	}

	void VulkanRenderer::createInstance()
	{
		if (enableValidationLayers && !checkValidationLayerSupport())
		{
			ThrowException("Validation layers are not available");
		}

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Test Engine - Vulkan";
		appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
		appInfo.pEngineName = "Test Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
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

		if (vkCreateInstance(&createInfo, nullptr, &vulkanInstance) != VK_SUCCESS)
		{
			ThrowException("Failed to create Vulkan instance!\n");
		}
	}

	bool VulkanRenderer::checkValidationLayerSupport()
	{
		uint32_t layerCount;
		std::vector<VkLayerProperties> availableLayers;

		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		availableLayers.resize(layerCount);
		LOG(TE_RENDERING_LOG, TE_DISPLAY, "%u available layers\n", layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : this->validationLayers)
		{
			bool found = false;
			
			LOG(TE_RENDERING_LOG, TE_DISPLAY, "Searching for layer %s\n", layerName);
			for (const auto& layerProperty : availableLayers)
			{
				LOG(TE_RENDERING_LOG, TE_DISPLAY, "Searching for property %s\n", layerProperty.layerName);
				if (!strcmp(layerName, layerProperty.layerName))
				{
					found = true;
					break;
				}
			}
			if (!found)
			{
				return false;
			}
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
		if (CreateDebugUtilsMessengerEXT(vulkanInstance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
		{
			ThrowException("Failed to setup vulkan debug message\n");
		}
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
		VkInstance vulkanInstance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(vulkanInstance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(vulkanInstance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else
		{
			LOG(TE_RENDERING_LOG, TE_ERROR, "Could not load 'vkCreateDebugUtilsMessengerEXT'\n");
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void VulkanRenderer::DestroyDebugUtilsMessengerEXT(
		VkInstance vulkanInstance,
		VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(vulkanInstance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(vulkanInstance, debugMessenger, pAllocator);
		}
		else
		{
			LOG(TE_RENDERING_LOG, TE_ERROR, "Could not load 'vkDestroyDebugUtilsMessengerEXT'\n");
		}
	}

	void VulkanRenderer::selectPhysicalDevices()
	{
		uint32_t deviceCount = 0;
		std::vector<VkPhysicalDevice> devices;

		vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, nullptr);
		if (!deviceCount)
		{
			ThrowException("No GPU found\n");
		}

		devices.resize(deviceCount);
		vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, devices.data());

		for (const VkPhysicalDevice& device : devices)
		{
			if (isDeviceSuitable(device))
			{
				/* Only selecting the first suitable GPU for now */
				/* TODO: either give a score to each device and pick the best one
				or allow use multiple devices */
				this->physicalDevices[0] = device;
				break;
			}
		}

		if (this->physicalDevices[0] == VK_NULL_HANDLE)
		{
			ThrowException("No suitable GPU found\n");
		}
	}

	bool VulkanRenderer::isDeviceSuitable(VkPhysicalDevice device)
	{
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;

		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		/* TODO: Check for real wanted features and properties */
		

		QueueFamilyIndices indices = findQueueFamilies(device);

		bool requiredExtensionsSupported = checkDeviceExtensionSupport(device);

		bool swapChainSupported = false;
		if (requiredExtensionsSupported)
		{
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
			swapChainSupported = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		if (indices.isComplete() && requiredExtensionsSupported && swapChainSupported)
		{
			LOG(TE_RENDERING_LOG, TE_LOG, "Selecting device '%s'\n",
				deviceProperties.deviceName);
			return true;
		}
		return false;
	}

	bool VulkanRenderer::checkDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	VulkanRenderer::QueueFamilyIndices VulkanRenderer::findQueueFamilies(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices;
		uint32_t queueFamilyCount = 0;
		std::vector<VkQueueFamilyProperties> queueFamilies;

		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		queueFamilies.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
			if (presentSupport)
			{
				indices.presentFamily = i;
			}

			/* Add other wanted families here */

			if (indices.isComplete())
			{
				break;
			}

			i++;
		}

		return indices;
	}

	void VulkanRenderer::createLogicalDevices()
	{
		/* TODO: handle multiple physical devices */
		queueFamilies.push_back(findQueueFamilies(physicalDevices[0]));

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies  =
			{queueFamilies[0].graphicsFamily.value(), queueFamilies[0].presentFamily.value()};

		float queuePriority = 1.0f;
		for (const uint32_t queueFamily: uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

		if (enableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(physicalDevices[0], &createInfo, nullptr, &devices[0]) != VK_SUCCESS)
		{
			ThrowException("Failed to create logical device!\n");
		}

		vkGetDeviceQueue(devices[0], queueFamilies[0].graphicsFamily.value(), 0, &graphicsQueue);
		vkGetDeviceQueue(devices[0], queueFamilies[0].presentFamily.value(), 0, &presentQueue);
	}

	VulkanRenderer::SwapChainSupportDetails VulkanRenderer::querySwapChainSupport(VkPhysicalDevice device)
	{
		SwapChainSupportDetails details;
		uint32_t formatCount;
		uint32_t presentModeCount;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
		if (formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
		if (presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR VulkanRenderer::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB
				&& availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}
		return availableFormats[0];
	}

	VkPresentModeKHR VulkanRenderer::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanRenderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}
		else
		{
			int w, h;
			window->getFrameSize(&w, &h);
			VkExtent2D actualExtent =
			{
				static_cast<uint32_t>(w),
				static_cast<uint32_t>(h)
			};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
			return actualExtent;
		}
	}

	void VulkanRenderer::createSwapChain()
	{
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevices[0]);

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		swapChainImageFormat = surfaceFormat.format;
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);
		swapChainExtent = extent;

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
		{
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		uint32_t queueFamilyIndices[] =
		{
			queueFamilies[0].graphicsFamily.value(),
			queueFamilies[0].presentFamily.value()
		};

		if (queueFamilies[0].graphicsFamily != queueFamilies[0].presentFamily)
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // Optional
			createInfo.pQueueFamilyIndices = nullptr; // Optional
		}

		if (vkCreateSwapchainKHR(devices[0], &createInfo, nullptr, &swapChain) != VK_SUCCESS)
		{
			ThrowException("Failed to create swap chain!\n");
		}

		vkGetSwapchainImagesKHR(devices[0], swapChain, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(devices[0], swapChain, &imageCount, swapChainImages.data());
	}

	void VulkanRenderer::createImageViews()
	{
		swapChainImageViews.resize(swapChainImages.size());

		for (size_t i = 0; i < swapChainImages.size(); i++)
		{
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = swapChainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = swapChainImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(devices[0], &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
			{
				ThrowException("Failed to create image view " + std::to_string(i) + "\n");
			}
		}
	}

	void VulkanRenderer::createGraphicsPipeline()
	{
		Shader vertex(Logger::ROOT_DIR_PATH + "Shaders/GLSL/triangle.vert.spv");
		Shader fragment(Logger::ROOT_DIR_PATH + "Shaders/GLSL/triangle.frag.spv");

		VkShaderModule vertShaderModule = createShaderModule(vertex.buff);
		VkShaderModule fragShaderModule = createShaderModule(fragment.buff);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vertShaderModule;
		vertShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragShaderModule;
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

		/*VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float) swapChainExtent.width;
		viewport.height = (float) swapChainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = {0, 0};
		scissor.extent = swapChainExtent;
		*/

		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f; // Optional
		rasterizer.depthBiasClamp = 0.0f; // Optional
		rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0; // Optional
		pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

		if (vkCreatePipelineLayout(devices[0], &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			ThrowException("Failed to create pipeline layout!\n");
		}

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr; // Optional
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = pipelineLayout;
		pipelineInfo.renderPass = renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		pipelineInfo.basePipelineIndex = -1; // Optional

		if (vkCreateGraphicsPipelines(devices[0], VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
		{
			ThrowException("Failed to create graphics pipeline!\n");
		}

		if (fragShaderModule != VK_NULL_HANDLE)
		{
			vkDestroyShaderModule(devices[0], fragShaderModule, nullptr);
		}
		if (vertShaderModule != VK_NULL_HANDLE)
		{
			vkDestroyShaderModule(devices[0], vertShaderModule, nullptr);
		}
	}

	VkShaderModule VulkanRenderer::createShaderModule(const std::vector<char>& code)
	{
		if (code.size() == 0 || !code.data())
		{
			return VkShaderModule{};
		}

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule{};
		if (vkCreateShaderModule(devices[0], &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
		{
			LOG(TE_RENDERING_LOG, TE_ERROR, "Failed to create shader module\n");
		}

		return shaderModule;
	}

	void VulkanRenderer::createRenderPass()
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = swapChainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(devices[0], &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
		{
			ThrowException("Failed to create render pass!\n");
		}
	}

	void VulkanRenderer::createFramebuffers()
	{
		framebuffers.resize(swapChainImageViews.size());

		for (size_t i = 0; i < swapChainImageViews.size(); i++)
		{
			VkImageView attachments[] =
			{
				swapChainImageViews[i]
			};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = swapChainExtent.width;
			framebufferInfo.height = swapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(devices[0], &framebufferInfo, nullptr, &framebuffers[i]) != VK_SUCCESS)
			{
				ThrowException("Failed to create framebuffer!\n");
			}
		}
	}

	void VulkanRenderer::createCommandPool()
	{
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilies[0].graphicsFamily.value();
		if (vkCreateCommandPool(devices[0], &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
		{
			ThrowException("Failed to create command pool!\n");
		}
	}

	void VulkanRenderer::createCommandBuffer()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(devices[0], &allocInfo, &commandBuffer) != VK_SUCCESS)
		{
			ThrowException("Failed to create command buffer!\n");
		}
	}

	void VulkanRenderer::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		{
			ThrowException("Failed to begin recording command buffer!\n");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = framebuffers[imageIndex];

		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = swapChainExtent;

		VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(swapChainExtent.width);
		viewport.height = static_cast<float>(swapChainExtent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = {0, 0};
		scissor.extent = swapChainExtent;
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		vkCmdDraw(commandBuffer, 3, 1, 0, 0);

		vkCmdEndRenderPass(commandBuffer);

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			ThrowException("Failed to end command buffer!\n");
		}
	}

	void VulkanRenderer::createSyncObjects()
	{
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		if (vkCreateSemaphore(devices[0], &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
			vkCreateSemaphore(devices[0], &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
			vkCreateFence(devices[0], &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS)
		{
			ThrowException("Failed to create semaphores!\n");
		}
	}

	void VulkanRenderer::drawFrame()
	{
		vkWaitForFences(devices[0], 1, &inFlightFence, VK_TRUE, UINT64_MAX);
		vkResetFences(devices[0], 1, &inFlightFence);

		uint32_t imageIndex;
		vkAcquireNextImageKHR(devices[0], swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
	
		vkResetCommandBuffer(commandBuffer, 0);
		recordCommandBuffer(commandBuffer, imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS)
		{
			ThrowException("Failed to submit draw command\n");
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = {swapChain};
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		vkQueuePresentKHR(presentQueue, &presentInfo);
	}

	void VulkanRenderer::render()
	{
		while (running == true)
		{
			if (window->handleEvents() == 1)
			{
				running = false;
			}

			drawFrame();
		}
		vkDeviceWaitIdle(devices[0]);
		// TODO
	}

	void VulkanRenderer::renderText(const char* text, std::shared_ptr<Font> font, Point2<int> pos, int size)
	{
		// TODO
		uninplemented();
	}

}