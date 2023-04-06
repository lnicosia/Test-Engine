#ifndef _VULKAN_RENDERER_HPP_
# define _VULKAN_RENDERER_HPP_

#include "Renderer.hpp"

#include "vulkan/vulkan.h"

namespace te
{
	class VulkanRenderer: public Renderer
	{
	public:
		VulkanRenderer();
		VulkanRenderer(WindowManager wManager);
		~VulkanRenderer();

		void render() override;
		void renderText(const char* text, std::shared_ptr<Font> font,
			Point2<int> pos, int size) override;
	private:
		/* Vulkan data */
		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkSurfaceKHR surface = VK_NULL_HANDLE;
		std::vector<VkPhysicalDevice> physicalDevices = { VK_NULL_HANDLE };
		std::vector<VkDevice> devices = { VK_NULL_HANDLE };
		VkQueue graphicsQueue;
		VkQueue presentQueue;

		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

		void initVulkan();
		void createInstance();
		bool checkValidationLayerSupport();
		void selectPhysicalDevices();
		void createLogicalDevices();

		bool isDeviceSuitable(VkPhysicalDevice device);

		struct QueueFamilyIndices
		{
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;
			QueueFamilyIndices() : graphicsFamily() {}

			bool isComplete()
			{
				return graphicsFamily.has_value() && presentFamily.has_value();
			}
		};

		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);

		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void setupDebugMessenger();

		/* Extension loading functions */
		VkResult CreateDebugUtilsMessengerEXT(
			VkInstance instance,
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator,
			VkDebugUtilsMessengerEXT* pDebugMessenger);
		void DestroyDebugUtilsMessengerEXT(
			VkInstance instance,
			VkDebugUtilsMessengerEXT debugMessenger,
			const VkAllocationCallbacks* pAllocator);
	};
}

#endif // _VULKAN_RENDERER_HPP_