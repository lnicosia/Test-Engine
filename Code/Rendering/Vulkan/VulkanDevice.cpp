#include "VulkanDevice.hpp"

#include "Rendering/Vulkan/VulkanUtils.hpp"
#include "Rendering/Vulkan/DescriptorSets/VulkanDescriptorWriter.hpp"
#include "Rendering/Vulkan/VulkanVertex.hpp"
#include "Rendering/Vulkan/VulkanMesh.hpp"
#include "Rendering/Shaders/Shader.hpp"
#include "Assets/Textures/VulkanTexture.hpp"
#include "Assets/AssetManager.hpp"

#include "Debug/Exception.hpp"
#include "Debug/Log.hpp"

#include "stb_image.h"

#include <set>
#include <chrono>

namespace te
{
	VulkanDevice::VulkanDevice(std::shared_ptr<Window> window): GPUDevice{}, window{window}
	{
		initVulkan();
	}

	VulkanDevice::~VulkanDevice()
	{
		cleanupSwapChain();
		/* TODO: handle multiple logical devices */

		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Destroying textures\n");
		for (auto& vulkanTexture : textures)
		{
			vulkanTexture->cleanUp();
		}

		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Destroying default texture\n");
		std::shared_ptr<VulkanTexture> texture =
			std::dynamic_pointer_cast<VulkanTexture>(AssetManager::getInstance().getDefaultTexture());
		if (texture)
		{
			texture->cleanUp();
		}

		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Destroying uniform buffers\n");
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroyBuffer(devices[0], frames[i].sceneBuffer, nullptr);
			vkFreeMemory(devices[0], frames[i].sceneBufferMemory, nullptr);
		}

		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Destroying descriptor pools\n");
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			frames[i].sceneDescriptorAllocator.cleanupPools(devices[0]);
			frames[i].meshDescriptorAllocator.cleanupPools(devices[0]);
		}
		vkDestroyDescriptorSetLayout(devices[0], sceneDescriptorSetLayout, nullptr);
		vkDestroyDescriptorSetLayout(devices[0], meshDescriptorSetLayout, nullptr);

		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Destroying vertex buffers\n");
		for (size_t i = 0; i < vertexBuffers.size(); i++)
		{
			vkDestroyBuffer(devices[0], vertexBuffers[i], nullptr);
			vkFreeMemory(devices[0], vertexBufferMemories[i], nullptr);
		}
		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Destroying index buffers\n");
		for (size_t i = 0; i < indexBuffers.size(); i++)
		{
			vkDestroyBuffer(devices[0], indexBuffers[i], nullptr);
			vkFreeMemory(devices[0], indexBufferMemories[i], nullptr);
		}

		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Destroying pipeline\n");
		vkDestroyPipeline(devices[0], opaquePipeline.pipeline, nullptr);
		vkDestroyPipelineLayout(devices[0], opaquePipeline.layout, nullptr);

		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Destroying render pass\n");
		vkDestroyRenderPass(devices[0], renderPass, nullptr);

		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Destroying semaphores\n");
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroySemaphore(devices[0], frames[i].imageAvailableSemaphore, nullptr);
			vkDestroySemaphore(devices[0], frames[i].renderFinishedSemaphore, nullptr);
			vkDestroyFence(devices[0], frames[i].inFlightFence, nullptr);
		}
		vkDestroyCommandPool(devices[0], graphicsCommandPool, nullptr);
		vkDestroyCommandPool(devices[0], transferCommandPool, nullptr);

		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Destroying device\n");
		vkDestroyDevice(devices[0], nullptr);
		vkDestroySurfaceKHR(vulkanInstance, surface, nullptr);
		if (enableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(vulkanInstance, debugMessenger, nullptr);
		}
		vkDestroyInstance(vulkanInstance, nullptr);
	}

	void VulkanDevice::initVulkan()
	{
		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Creating Vulkan instance\n");
		createInstance();
		if (enableValidationLayers)
		{
			setupDebugMessenger();
		}
		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Creating platform specific surface\n");
		window->createVulkanSurface(vulkanInstance, &surface);
		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Selecting physical device\n");
		selectPhysicalDevices();
		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Creating logical device\n");
		createLogicalDevices();
		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Creating swap chain\n");
		createSwapChain();
		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Creating image views\n");
		createImageViews();
		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Creating render pass\n");
		createRenderPass();
		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Creating descriptor set layouts\n");
		createDescriptorSetLayouts();
		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Creating graphics pipeline\n");
		createOpaquePipeline();
		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Creating command pools\n");
		createCommandPools();
		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Creating depth buffer\n");
		createDepthResources();
		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Creating frame buffers\n");
		createFrameBuffers();
		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Creating scene description buffers\n");
		createSceneBuffers();
		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Creating descriptor pool\n");
		createDescriptorPools();
		for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			//sceneDescriptorSets[i] = frames[i].sceneDescriptorAllocator.allocate(devices[0], sceneDescriptorSetLayout);
			/*updateSceneBuffer(i);
			writer.writeBuffer(0, frames[i].sceneBuffer, sizeof(SceneBufferObject),
			0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
			writer.updateSet(devices[0], sceneDescriptorSets[i]);*/
		}
		//sceneBufferObject.view = sml::lookAt(sml::vec3(2.0f, 2.0f, 2.0f), sml::vec3(0.0f, 0.0f, 0.0f), sml::vec3(0.0f, 0.0f, 1.0f));
		sceneBufferObject.projection = sml::perspective(SML_TO_RADIANS * 45.0f, swapChainExtent.width / (float) swapChainExtent.height, 0.1f, 1000.0f);
		sceneBufferObject.projection[1][1] *= -1;
		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Creating command buffer\n");
		createCommandBuffers();
		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Creating synchronization objects\n");
		createSyncObjects();
	}

	void VulkanDevice::createInstance()
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

		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Available Vulkan extensions:\n");
		for (auto extension : extensions)
		{
			TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "\t%s\n", extension.extensionName);
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

	bool VulkanDevice::checkValidationLayerSupport()
	{
		uint32_t layerCount;
		std::vector<VkLayerProperties> availableLayers;

		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		availableLayers.resize(layerCount);
		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "%u available layers\n", layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : this->validationLayers)
		{
			bool found = false;
			
			TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Searching for layer %s\n", layerName);
			for (const auto& layerProperty : availableLayers)
			{
				TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Searching for property %s\n", layerProperty.layerName);
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

	void VulkanDevice::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
		createInfo.pUserData = nullptr;
	}

	void VulkanDevice::setupDebugMessenger()
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo;

		populateDebugMessengerCreateInfo(createInfo);
		if (CreateDebugUtilsMessengerEXT(vulkanInstance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
		{
			ThrowException("Failed to setup vulkan debug message\n");
		}
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDevice::debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Validation layer: %s\n", pCallbackData->pMessage);
		if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			TE_LOG(TE_RENDERING_LOG, TE_WARNING, "Vulkan warning: %s\n", pCallbackData->pMessage);
		}
		else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		{
			TE_LOG(TE_RENDERING_LOG, TE_ERROR, "Vulkan error: %s\n", pCallbackData->pMessage);
		}
		else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
		{
			TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Vulkan log: %s\n", pCallbackData->pMessage);
		}
		return VK_FALSE;
	}

	VkResult VulkanDevice::CreateDebugUtilsMessengerEXT(
		VkInstance currentVulkanInstance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(currentVulkanInstance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(currentVulkanInstance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else
		{
			TE_LOG(TE_RENDERING_LOG, TE_ERROR, "Could not load 'vkCreateDebugUtilsMessengerEXT'\n");
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void VulkanDevice::DestroyDebugUtilsMessengerEXT(
		VkInstance currentVulkanInstance,
		VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(currentVulkanInstance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(vulkanInstance, debugMessenger, pAllocator);
		}
		else
		{
			TE_LOG(TE_RENDERING_LOG, TE_ERROR, "Could not load 'vkDestroyDebugUtilsMessengerEXT'\n");
		}
	}

	void VulkanDevice::selectPhysicalDevices()
	{
		uint32_t deviceCount = 0;
		std::vector<VkPhysicalDevice> deviceQueries;

		vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, nullptr);
		if (!deviceCount)
		{
			ThrowException("No GPU found\n");
		}

		deviceQueries.resize(deviceCount);
		vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, deviceQueries.data());

		for (const VkPhysicalDevice& physicalDevice : deviceQueries)
		{
			if (isDeviceSuitable(physicalDevice))
			{
				/* Only selecting the first suitable GPU for now */
				/* TODO: either give a score to each device and pick the best one,
				allow to use multiple devices or ask the user to select one */
				physicalDevices[0] = physicalDevice;
				break;
			}
		}

		if (physicalDevices[0] == VK_NULL_HANDLE)
		{
			ThrowException("No suitable GPU found\n");
		}
	}

	bool VulkanDevice::isDeviceSuitable(VkPhysicalDevice physicalDevice)
	{
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;

		vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
		vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

		/* TODO: Check for real wanted features and properties */

		QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

		bool requiredExtensionsSupported = checkDeviceExtensionSupport(physicalDevice);

		bool swapChainSupported = false;
		if (requiredExtensionsSupported)
		{
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);
			swapChainSupported = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		if (indices.isComplete() && requiredExtensionsSupported && swapChainSupported
			&& deviceFeatures.samplerAnisotropy)
		{
			TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Selecting device '%s'\n",
				deviceProperties.deviceName);
			return true;
		}
		return false;
	}

	bool VulkanDevice::checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	QueueFamilyIndices VulkanDevice::findQueueFamilies(VkPhysicalDevice physicalDevice)
	{
		QueueFamilyIndices indices;
		uint32_t queueFamilyCount = 0;
		std::vector<VkQueueFamilyProperties> queueFamilyQueries;

		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

		if (queueFamilyCount == 0)
		{
			TE_LOG(TE_RENDERING_LOG, TE_ERROR, "No queue family available\n");
		}
		queueFamilyQueries.resize(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyQueries.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilyQueries)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
			if (presentSupport)
			{
				indices.presentFamily = i;
			}

			if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT
				&& !(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT))
			{
				indices.transferFamily = i;
			}

			if (indices.isComplete())
			{
				break;
			}

			i++;
		}

		return indices;
	}

	void VulkanDevice::createLogicalDevices()
	{
		/* TODO: handle multiple physical devices */
		queueFamilies.push_back(findQueueFamilies(physicalDevices[0]));

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies  =
			{queueFamilies[0].graphicsFamily.value(),
			queueFamilies[0].presentFamily.value(),
			queueFamilies[0].transferFamily.value()};

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
		deviceFeatures.samplerAnisotropy = VK_TRUE;

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
		vkGetDeviceQueue(devices[0], queueFamilies[0].transferFamily.value(), 0, &transferQueue);
	}

	SwapChainSupportDetails VulkanDevice::querySwapChainSupport(VkPhysicalDevice device)
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

	VkSurfaceFormatKHR VulkanDevice::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
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

	VkPresentModeKHR VulkanDevice::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
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

	VkExtent2D VulkanDevice::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
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

	void VulkanDevice::createSwapChain()
	{
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevices[0]);

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		swapChainImageFormat = surfaceFormat.format;
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);
		swapChainExtent = extent;

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		/** maxImageCount == 0 means no max */
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

		VkResult res = vkCreateSwapchainKHR(devices[0], &createInfo, nullptr, &swapChain);
		if (res != VK_SUCCESS)
		{
			ThrowException("Failed to create swap chain!\n");
		}

		vkGetSwapchainImagesKHR(devices[0], swapChain, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(devices[0], swapChain, &imageCount, swapChainImages.data());
	}

	void VulkanDevice::cleanupSwapChain()
	{
		vkDestroyImageView(devices[0], depthImageView, nullptr);
		vkDestroyImage(devices[0], depthImage, nullptr);
		vkFreeMemory(devices[0], depthImageMemory, nullptr);
		for (auto framebuffer : framebuffers)
		{
			vkDestroyFramebuffer(devices[0], framebuffer, nullptr);
		}
		for (auto imageView : swapChainImageViews)
		{
			vkDestroyImageView(devices[0], imageView, nullptr);
		}
		vkDestroySwapchainKHR(devices[0], swapChain, nullptr);
	}

	void VulkanDevice::recreateSwapChain()
	{
		int windowWidth = 0;
		int windowHeight = 0;
		window->getFrameSize(&windowWidth, &windowHeight);
		while (windowWidth == 0 || windowHeight == 0)
		{
			window->handleEvents();
			window->getFrameSize(&windowWidth, &windowHeight);
		}
		vkDeviceWaitIdle(devices[0]);

		cleanupSwapChain();

		createSwapChain();
		createImageViews();
		createDepthResources();
		createFrameBuffers();
	}

	void VulkanDevice::createImageViews()
	{
		swapChainImageViews.resize(swapChainImages.size());

		for (size_t i = 0; i < swapChainImages.size(); i++)
		{
			swapChainImageViews[i] = createImageView(swapChainImages[i],
				swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
		}
	}

	void VulkanDevice::createDescriptorSetLayouts()
	{
		VkDescriptorSetLayoutBinding sceneLayoutBinding{};
		sceneLayoutBinding.binding = 0;
		sceneLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		sceneLayoutBinding.descriptorCount = 1;
		sceneLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		sceneLayoutBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutCreateInfo layoutCreateInfo{};
		layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		std::array<VkDescriptorSetLayoutBinding, 1> sceneBindings{ sceneLayoutBinding };
		layoutCreateInfo.bindingCount = static_cast<uint32_t>(sceneBindings.size());
		layoutCreateInfo.pBindings = sceneBindings.data();

		if (vkCreateDescriptorSetLayout(devices[0], &layoutCreateInfo, nullptr, &sceneDescriptorSetLayout) != VK_SUCCESS)
		{
			ThrowException("Failed to create descriptor set layout!");
		}

		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
		samplerLayoutBinding.binding = 0;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		VkDescriptorSetLayoutBinding textureArrayLayoutBinding{};
		textureArrayLayoutBinding.binding = 1;
		textureArrayLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		textureArrayLayoutBinding.descriptorCount = 1;
		textureArrayLayoutBinding.pImmutableSamplers = nullptr;
		textureArrayLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		std::array<VkDescriptorSetLayoutBinding, 2> meshBindings{ samplerLayoutBinding,
			textureArrayLayoutBinding };
		layoutCreateInfo.bindingCount = static_cast<uint32_t>(meshBindings.size());
		layoutCreateInfo.pBindings = meshBindings.data();

		if (vkCreateDescriptorSetLayout(devices[0], &layoutCreateInfo, nullptr, &meshDescriptorSetLayout) != VK_SUCCESS)
		{
			ThrowException("Failed to create descriptor set layout!");
		}
	}

	void VulkanDevice::createOpaquePipeline()
	{
		Shader vertex(Logger::getRootDirPath() + "Shaders/SPIR-V/triangle.vert.spv");
		Shader fragment(Logger::getRootDirPath() + "Shaders/SPIR-V/triangle.frag.spv");

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

		std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages =
			{ vertShaderStageInfo, fragShaderStageInfo};

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

		auto bindingDescription = VulkanVertex::getBindingDescription();
		auto attributeDescriptions = VulkanVertex::getAttributeDescriptions();

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

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
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
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

		VkPipelineDepthStencilStateCreateInfo depthStencil{};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = VK_TRUE;
		depthStencil.depthWriteEnable = VK_TRUE;
		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.minDepthBounds = 0.0f; // Optional
		depthStencil.maxDepthBounds = 1.0f; // Optional
		depthStencil.stencilTestEnable = VK_FALSE;
		depthStencil.front = {}; // Optional
		depthStencil.back = {}; // Optional

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

		VkPushConstantRange pushConstantRanges{};
		pushConstantRanges.size = sizeof(sml::mat4);
		pushConstantRanges.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		std::array<VkDescriptorSetLayout, 2> descriptorSetLayouts{
			sceneDescriptorSetLayout, meshDescriptorSetLayout
		};
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRanges;

		if (vkCreatePipelineLayout(devices[0], &pipelineLayoutInfo, nullptr, &opaquePipeline.layout) != VK_SUCCESS)
		{
			ThrowException("Failed to create pipeline layout!\n");
		}

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
		pipelineInfo.pStages = shaderStages.data();
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = &depthStencil;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = opaquePipeline.layout;
		pipelineInfo.renderPass = renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		pipelineInfo.basePipelineIndex = -1; // Optional

		if (vkCreateGraphicsPipelines(devices[0], VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &opaquePipeline.pipeline) != VK_SUCCESS)
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

	VkShaderModule VulkanDevice::createShaderModule(const std::vector<char>& code)
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
			TE_LOG(TE_RENDERING_LOG, TE_ERROR, "Failed to create shader module\n");
		}

		return shaderModule;
	}

	void VulkanDevice::createRenderPass()
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

		VkAttachmentDescription depthAttachment{};
		depthAttachment.format = findDepthFormat();
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask =
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask =
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask =
			VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		std::array<VkAttachmentDescription, 2> attachments{ colorAttachment, depthAttachment };
		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(devices[0], &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
		{
			ThrowException("Failed to create render pass!\n");
		}
	}

	void VulkanDevice::createFrameBuffers()
	{
		framebuffers.resize(swapChainImageViews.size());

		for (size_t i = 0; i < swapChainImageViews.size(); i++)
		{
			std::array<VkImageView, 2> attachments =
			{
				swapChainImageViews[i],
				depthImageView
			};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = swapChainExtent.width;
			framebufferInfo.height = swapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(devices[0], &framebufferInfo, nullptr, &framebuffers[i]) != VK_SUCCESS)
			{
				ThrowException("Failed to create framebuffer!\n");
			}
		}
	}

	void VulkanDevice::createCommandPools()
	{
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilies[0].graphicsFamily.value();
		if (vkCreateCommandPool(devices[0], &poolInfo, nullptr, &graphicsCommandPool) != VK_SUCCESS)
		{
			ThrowException("Failed to create main command pool!\n");
		}
		poolInfo.queueFamilyIndex = queueFamilies[0].transferFamily.value();
		if (vkCreateCommandPool(devices[0], &poolInfo, nullptr, &transferCommandPool) != VK_SUCCESS)
		{
			ThrowException("Failed to create transfer command pool!\n");
		}
	}

	VkFormat VulkanDevice::findSupportedFormat(const std::vector<VkFormat>& candidates,
		VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for (VkFormat format : candidates)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(physicalDevices[0], format, &props);
			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
			{
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
			{
				return format;
			}
		}
		ThrowException("Could not find a supported format\n");
		return VkFormat{};
	}

	VkFormat VulkanDevice::findDepthFormat()
	{
		return findSupportedFormat(
			{VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	}

	void VulkanDevice::createDepthResources()
	{
		VkFormat depthFormat = findDepthFormat();
		createImage(swapChainExtent.width, swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
		depthImageView = createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
		transitionImageLayout(graphicsCommandPool, graphicsQueue,
			depthImage, depthFormat,
			VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	}

	void VulkanDevice::createVertexBuffer(const std::vector<Vertex>& inVertices,
		VkBuffer& outBuffer, VkDeviceMemory& outMemory)
	{
		VkDeviceSize bufferSize = sizeof(inVertices[0]) * inVertices.size();

		VkBuffer stagingBuffer;
    	VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(devices[0], stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, inVertices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(devices[0], stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, outBuffer, outMemory);
		copyBuffer(stagingBuffer, outBuffer, bufferSize);
		vertexBuffers.push_back(outBuffer);
		vertexBufferMemories.push_back(outMemory);

		vkDestroyBuffer(devices[0], stagingBuffer, nullptr);
    	vkFreeMemory(devices[0], stagingBufferMemory, nullptr);
	}
	
	void VulkanDevice::createIndexBuffer(const std::vector<uint32_t>& inIndices,
		VkBuffer& outBuffer, VkDeviceMemory& outMemory)
	{
		VkDeviceSize bufferSize = sizeof(inIndices[0]) * inIndices.size();

		VkBuffer stagingBuffer;
    	VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(devices[0], stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, inIndices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(devices[0], stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, outBuffer, outMemory);
		copyBuffer(stagingBuffer, outBuffer, bufferSize);
		indexBuffers.push_back(outBuffer);
		indexBufferMemories.push_back(outMemory);

		vkDestroyBuffer(devices[0], stagingBuffer, nullptr);
    	vkFreeMemory(devices[0], stagingBufferMemory, nullptr);
	}

	void VulkanDevice::createSceneBuffers()
	{
		VkDeviceSize bufferSize = sizeof(SceneBufferObject);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			frames[i].sceneBuffer, frames[i].sceneBufferMemory);

			vkMapMemory(devices[0], frames[i].sceneBufferMemory, 0, bufferSize, 0, &frames[i].sceneBufferMapped);
		}
	}

	void VulkanDevice::createDescriptorPools()
	{
		for (size_t i = 0 ; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			std::vector<VulkanDescriptorAllocator::PoolSize> scenePoolSizes =
			{
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1.0f }
			};
			frames[i].sceneDescriptorAllocator.init(devices[0], 1000, scenePoolSizes);
			
			std::vector<VulkanDescriptorAllocator::PoolSize> meshPoolSizes =
			{
				{ VK_DESCRIPTOR_TYPE_SAMPLER, 1.0f },
				{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1.0f }
			};

			frames[i].meshDescriptorAllocator.init(devices[0], 1000, meshPoolSizes);
		}
	}

	void VulkanDevice::updateDescriptorSet(VkDescriptorSet descriptorSet,
		const std::shared_ptr<VulkanTexture> texture)
	{
		VulkanDescriptorWriter writer;
		writer.writeSampler(0, texture->getSampler(),
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_SAMPLER);
		writer.writeImage(1, texture->getImageView(),
		VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);
		writer.updateSet(devices[0], descriptorSet);
	}

	void VulkanDevice::createMeshDescriptorSets(std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT>& outDescriptorSets)
	{
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			outDescriptorSets[i] = frames[i].meshDescriptorAllocator.allocate(devices[0], meshDescriptorSetLayout);
		}
	}

	void VulkanDevice::createCommandBuffers()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = graphicsCommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			if (vkAllocateCommandBuffers(devices[0], &allocInfo, &frames[i].commandBuffer) != VK_SUCCESS)
			{
				ThrowException("Failed to create command buffer!\n");
			}
		}
	}

	void VulkanDevice::createSyncObjects()
	{
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			if (vkCreateSemaphore(devices[0], &semaphoreInfo, nullptr, &frames[i].imageAvailableSemaphore) != VK_SUCCESS)
			{
				ThrowException("Failed to create image semaphore!\n");
			}
			if (vkCreateSemaphore(devices[0], &semaphoreInfo, nullptr, &frames[i].renderFinishedSemaphore) != VK_SUCCESS)
			{
				ThrowException("Failed to create render semaphore!\n");
			}
			if (vkCreateFence(devices[0], &fenceInfo, nullptr, &frames[i].inFlightFence) != VK_SUCCESS)
			{
				ThrowException("Failed to create in flight fence!\n");
			}
		}
	}

	void VulkanDevice::updateSceneBuffer(uint32_t currentFrame)
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	
		//ubo.model = sml::rotate(time * SML_TO_RADIANS * 90.0f, sml::vec3(0.0f, 0.0f, 1.0f));
		sceneBufferObject.view = sml::lookAt(sml::vec3(2.0f, 2.0f, 2.0f), sml::vec3(0.0f, 0.0f, 0.0f), sml::vec3(0.0f, 0.0f, 1.0f));
		sceneBufferObject.projection = sml::perspective(SML_TO_RADIANS * 45.0f, swapChainExtent.width / (float) swapChainExtent.height, 0.1f, 10.0f);
		sceneBufferObject.projection[1][1] *= -1;

		memcpy(frames[currentFrame].sceneBufferMapped, &sceneBufferObject, sizeof(sceneBufferObject));
	}


	void VulkanDevice::createBuffer(VkDeviceSize& size, VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties, VkBuffer& outBuffer, VkDeviceMemory& outBufferMemory)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
		
		uint32_t queueFamilyIndices[] =
		{
			queueFamilies[0].graphicsFamily.value(),
			queueFamilies[0].transferFamily.value()
		};
		bufferInfo.queueFamilyIndexCount = 2;
		bufferInfo.pQueueFamilyIndices = queueFamilyIndices;

		if (vkCreateBuffer(devices[0], &bufferInfo, nullptr, &outBuffer) != VK_SUCCESS)
		{
			ThrowException("Failed to create vertex buffer!");
		}

		VkMemoryRequirements memRequirements{};
		vkGetBufferMemoryRequirements(devices[0], outBuffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(physicalDevices[0], memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(devices[0], &allocInfo, nullptr, &outBufferMemory) != VK_SUCCESS)
		{
			ThrowException("Failed to allocate vertex buffer memory");
		}
		vkBindBufferMemory(devices[0], outBuffer, outBufferMemory, 0);
	}

	
	void VulkanDevice::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& outImage, VkDeviceMemory& outImageMemory)
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
		if (vkCreateImage(devices[0], &imageInfo, nullptr, &outImage) != VK_SUCCESS)
		{
			ThrowException("Failed to create image!\n");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(devices[0], outImage, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(physicalDevices[0], memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if (vkAllocateMemory(devices[0], &allocInfo, nullptr, &outImageMemory) != VK_SUCCESS)
		{
			ThrowException("Failed to allocate image memory!\n");
		}

		vkBindImageMemory(devices[0], outImage, outImageMemory, 0);
	}

	VkImageView VulkanDevice::createImageView( VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VkImageView imageView;
		if (vkCreateImageView(devices[0], &viewInfo, nullptr, &imageView) != VK_SUCCESS)
		{
			ThrowException("Failed to create texture image view!\n");
		}

		return imageView;
	}

	void VulkanDevice::createVertexBuffer(const std::vector<Vertex>& vertices,
		const std::vector<uint32_t>& indices, void* outBuffer, void* outBufferMemory)
	{
		VkBuffer vertexBuffer = static_cast<VkBuffer>(outBuffer);
		VkDeviceMemory vertexBufferMemory = static_cast<VkDeviceMemory>(outBufferMemory);
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

		VkBuffer stagingBuffer;
    	VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(devices[0], stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(devices[0], stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
		copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

		vkDestroyBuffer(devices[0], stagingBuffer, nullptr);
    	vkFreeMemory(devices[0], stagingBufferMemory, nullptr);
	}

	void VulkanDevice::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
	{
		VkCommandBuffer commandBuffer = beginSingleTimeCommands(transferCommandPool, devices[0]);

		VkBufferCopy copyRegion{};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		endSingleTimeCommands(devices[0], transferQueue, commandBuffer, transferCommandPool);
	}

	void VulkanDevice::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
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

		std::array<VkClearValue, 2> clearColors{};
		clearColors[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
		clearColors[1].depthStencil = {1.0f, 0};
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearColors.size());
		renderPassInfo.pClearValues = clearColors.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, opaquePipeline.pipeline);

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

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
			opaquePipeline.layout, 0, 1, &sceneDescriptorSets[currentFrame], 0, nullptr);
		for (const RenderObject& obj : renderObjects)
		{			
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
				opaquePipeline.layout, 1, 1, &obj.desc[currentFrame], 0, nullptr);

			vkCmdBindVertexBuffers(commandBuffer, 0, 1, &obj.vertexBuffer,
				&obj.vertexBufferOffset);
			vkCmdBindIndexBuffer(commandBuffer, obj.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
			vkCmdPushConstants(commandBuffer, opaquePipeline.layout, VK_SHADER_STAGE_VERTEX_BIT,
				0, sizeof(sml::mat4), &obj.transform);
			vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(obj.indexBufferSize), 1, 0, 0, 0);

			frameStats.drawCallCount++;
		}

		vkCmdEndRenderPass(commandBuffer);

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			ThrowException("Failed to end command buffer!\n");
		}
	}

	void VulkanDevice::drawFrame(const Camera& camera)
	{
		vkWaitForFences(devices[0], 1, &frames[currentFrame].inFlightFence, VK_TRUE, UINT64_MAX);

		frames[currentFrame].deletionQueue.flush();
		if (frames[currentFrame].mustUpdateCamera)
		{
			updateCameraDescriptors(currentFrame);
			frames[currentFrame].mustUpdateCamera = false;
		}
		
		uint32_t imageIndex;
		VkResult res = vkAcquireNextImageKHR(devices[0], swapChain, UINT64_MAX, frames[currentFrame].imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
		if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR || frameBufferResized)
		{
			frameBufferResized = false;
			// TODO: Allow window resize
			TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Swap chain needs recreation\n");
			recreateSwapChain();
			return;
		}
		else if (res != VK_SUCCESS)
		{
			ThrowException("Failed to acquire swap chain image");
		}
		vkResetFences(devices[0], 1, &frames[currentFrame].inFlightFence);
	
		vkResetCommandBuffer(frames[currentFrame].commandBuffer, 0);
	
		recordCommandBuffer(frames[currentFrame].commandBuffer, imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { frames[currentFrame].imageAvailableSemaphore };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &frames[currentFrame].commandBuffer;

		VkSemaphore signalSemaphores[] = { frames[currentFrame].renderFinishedSemaphore };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, frames[currentFrame].inFlightFence) != VK_SUCCESS)
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
		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void VulkanDevice::createTexture(const std::string& path, VkImage& outImage, VkDeviceMemory& outImageMemory)
	{
		int texWidth, texHeight, texChannels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* pixels = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		if (!pixels)
		{
			ThrowException("Could not import " + path + "\n");
		}
		VkDeviceSize imageSize = texWidth * texHeight * 4;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferMemory);
		void* data;

		vkMapMemory(devices[0], stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(imageSize));
		vkUnmapMemory(devices[0], stagingBufferMemory);
		
		stbi_image_free(pixels);
		createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, outImage, outImageMemory);

		transitionImageLayout(transferCommandPool, transferQueue,
			outImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		copyBufferToImage(stagingBuffer, outImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
		transitionImageLayout(graphicsCommandPool, graphicsQueue,
			outImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		
		vkDestroyBuffer(devices[0], stagingBuffer, nullptr);
		vkFreeMemory(devices[0], stagingBufferMemory, nullptr);
	}
	
	void VulkanDevice::transitionImageLayout(VkCommandPool commandPool, VkQueue queue,
		VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
	{
		VkCommandBuffer commandBuffer = beginSingleTimeCommands(commandPool, devices[0]);

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			if (hasStencilComponent(format))
			{
				barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
		}
		else
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}
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
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else
		{
			ThrowException("Unsupported layout transition\n");
		}

		vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		endSingleTimeCommands(devices[0], queue, commandBuffer, commandPool);
	}

	void VulkanDevice::cleanUpTexture(VkImage textureImage, VkDeviceMemory textureImageMemory,
		VkImageView textureImageView, VkSampler textureSampler)
	{
		vkDestroySampler(devices[0], textureSampler, nullptr);
		vkDestroyImageView(devices[0], textureImageView, nullptr);
		vkDestroyImage(devices[0], textureImage, nullptr);
		vkFreeMemory(devices[0], textureImageMemory, nullptr); 
	}
	
	VkSampler VulkanDevice::createTextureSampler()
	{
		VkSampler newSampler;
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE; // TODO: user choice!

		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(physicalDevices[0], &properties);
		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy; // TODO: user choice!

		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;

		if (vkCreateSampler(devices[0], &samplerInfo, nullptr, &newSampler) != VK_SUCCESS)
		{
			ThrowException("Failed to create texture sampler!\n");
		}
		return newSampler;
	}
	
	void VulkanDevice::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
	{
		VkCommandBuffer commandBuffer = beginSingleTimeCommands(transferCommandPool, devices[0]);

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

		endSingleTimeCommands(devices[0], transferQueue, commandBuffer, transferCommandPool);
	}

	void VulkanDevice::updateCameraContext(const Camera& camera)
	{
		sceneBufferObject.view = camera.view;
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			memcpy(frames[i].sceneBufferMapped, &sceneBufferObject, sizeof(sceneBufferObject));
			frames[i].mustUpdateCamera = true;
		}
	}

	void VulkanDevice::updateCameraDescriptors(uint32_t frameIndex)
	{
		VulkanDescriptorWriter writer;
		frames[frameIndex].sceneDescriptorAllocator.clearPools(devices[0]);
		sceneDescriptorSets[frameIndex] = frames[frameIndex].sceneDescriptorAllocator.allocate(devices[0],
			sceneDescriptorSetLayout);
		memcpy(frames[frameIndex].sceneBufferMapped, &sceneBufferObject, sizeof(sceneBufferObject));
		writer.writeBuffer(0, frames[frameIndex].sceneBuffer, sizeof(SceneBufferObject),
		0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		writer.updateSet(devices[0], sceneDescriptorSets[frameIndex]);
	}

	void VulkanDevice::updateDrawContext(const Scene& scene)
	{
		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Updating scene rendering cache\n");
		renderObjects.clear();

		size_t meshCount = 0;
		for (std::shared_ptr<MeshInternal> mesh : scene.getInternalMeshes())
		{
			std::shared_ptr<VulkanMesh> vulkanMesh = std::dynamic_pointer_cast<VulkanMesh>(mesh);
			if (!vulkanMesh || !vulkanMesh->isVisible())
			{
				TE_LOG(TE_RENDERING_LOG, TE_ERROR, "Failed to cast internal mesh to vulkan mesh\n");
				continue;
			}
			meshCount++;
		}
		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "%zu meshes to render\n", meshCount);
		renderObjects.resize(meshCount);
		const std::vector<std::shared_ptr<MeshInternal>>& internalMeshes = scene.getInternalMeshes();
		for (size_t i = 0; i < meshCount; i++)
		{
			std::shared_ptr<VulkanMesh> vulkanMesh = std::dynamic_pointer_cast<VulkanMesh>(internalMeshes[i]);
			if (vulkanMesh && vulkanMesh->isVisible())
			{
				renderObjects[i].vertexBuffer = vulkanMesh->getVertexBuffer();
				renderObjects[i].indexBuffer = vulkanMesh->getIndexBuffer();
				renderObjects[i].indexBufferSize = vulkanMesh->getIndexBufferSize();
				for (size_t j = 0; j < MAX_FRAMES_IN_FLIGHT; j++)
				{
					renderObjects[i].desc = vulkanMesh->getDescriptorSets();
				}
				// TMP
				renderObjects[i].transform = sml::mat4(1.0f);
				renderObjects[i].transform *= sml::translate(sml::vec3{static_cast<float>(i), 0.0f, 0.0f});
			}
		}
	}

	/** Getters */
	std::vector<VkDevice>& VulkanDevice::getDevices()
	{
		return devices;
	}

} // namespace te