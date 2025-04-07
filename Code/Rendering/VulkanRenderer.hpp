#ifndef _VULKAN_RENDERER_HPP_
# define _VULKAN_RENDERER_HPP_

#include "Renderer.hpp"

#include "vulkan/vulkan.h"

#include <optional>

const int MAX_FRAMES_IN_FLIGHT = 2;

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
		struct SwapChainSupportDetails
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

		/* Vulkan data */
		VkInstance vulkanInstance;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkSurfaceKHR surface = VK_NULL_HANDLE;
		std::vector<VkPhysicalDevice> physicalDevices = { VK_NULL_HANDLE };
		std::vector<VkDevice> devices = { VK_NULL_HANDLE };
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		std::vector<QueueFamilyIndices> queueFamilies;
		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		std::vector<VkImageView> swapChainImageViews;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		VkRenderPass renderPass;
		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;
		std::vector<VkFramebuffer> framebuffers;
		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;

		uint32_t currentFrame = 0;

		bool frameBufferResized = false;

		const std::vector<const char*> validationLayers =
		{
			"VK_LAYER_KHRONOS_validation"
		};

		const std::vector<const char*> deviceExtensions =
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

#if !defined(NDEBUG) && defined(ENABLE_VALIDATION_LAYERS)
		const bool enableValidationLayers = true;
#else
		const bool enableValidationLayers = false;
#endif

		void initVulkan();
		void createInstance();
		bool checkValidationLayerSupport();
		void selectPhysicalDevices();
		void createLogicalDevices();

		void createSwapChain();
		void recreateSwapChain();
		void cleanupSwapChain();

		void createImageViews();
		void createRenderPass();
		void createGraphicsPipeline();
		void createFrameBuffers();
		void createCommandPool();
		void createCommandBuffers();
		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		void createSyncObjects();

		void drawFrame();

		bool isDeviceSuitable(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);

		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void setupDebugMessenger();

		/* Surface */
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

		/* Presentation mode */
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

		/* Swap chain extent */
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		/* Shaders */
		/* The type is char because of std::istream::read() which needs a char* pointer */
		VkShaderModule createShaderModule(const std::vector<char>& code);

		/* Extension loading functions */
		VkResult CreateDebugUtilsMessengerEXT(
			VkInstance vulkanInstance,
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator,
			VkDebugUtilsMessengerEXT* pDebugMessenger);
		void DestroyDebugUtilsMessengerEXT(
			VkInstance vulkanInstance,
			VkDebugUtilsMessengerEXT debugMessenger,
			const VkAllocationCallbacks* pAllocator);
	};
}

#endif // _VULKAN_RENDERER_HPP_