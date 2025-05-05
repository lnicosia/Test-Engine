#ifndef _VULKAN_DEVICE_HPP_
#define _VULKAN_DEVICE_HPP_

#include "Rendering/Window/Window.hpp"
#include "Rendering/GPUDevice.hpp"
#include "Rendering/Vulkan/VulkanMaterial.hpp"
#include "Rendering/Vulkan/QueueFamilyIndices.hpp"
#include "Rendering/Vulkan/DescriptorSets/VulkanDescriptorAllocator.hpp"

#include "vulkan/vulkan.h"


namespace te
{
	class VulkanTexture;

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};
	
	struct SceneBufferObject
	{
		alignas(16) sml::mat4 view;
		alignas(16) sml::mat4 projection;
	};

	class VulkanDevice : public GPUDevice
	{

		struct FrameData
		{
			/** Synchro */
			VkSemaphore imageAvailableSemaphore{};
			VkSemaphore renderFinishedSemaphore{};
			VkFence inFlightFence{};

			/** Command buffer */
			VkCommandBuffer commandBuffer{};

			/** Descriptors */
			VulkanDescriptorAllocator descriptorAllocator{};
			
			/** Scene buffers */
			VkBuffer sceneBuffer{};
			VkDeviceMemory sceneBufferMemory{};
			void* sceneBufferMapped{};
		};

	public:
		VulkanDevice() = delete;
		VulkanDevice(std::shared_ptr<Window> window);
		~VulkanDevice();

		void createVertexBuffer(const std::vector<Vertex>& vertices,
			const std::vector<uint32_t>& indices, void* outBuffer, void* outBufferMemory) override;

		/** Buffers and image creation and transition */
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void createBuffer(VkDeviceSize& size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
			VkBuffer& outBuffer, VkDeviceMemory& outBufferMemory);

		/** Buffers */
		void createVertexBuffer(const std::vector<Vertex>& inVertices,
			VkBuffer& outBuffer, VkDeviceMemory& outMemory);
		void createIndexBuffer(const std::vector<uint32_t>& inIndices,
			VkBuffer& outBuffer, VkDeviceMemory& outMemory);

		/** Textures */
		void createTexture(const std::string& path, VkImage& outImage, VkDeviceMemory& outImageMemory);
		void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
			VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& outImage, VkDeviceMemory& outImageMemory);
		VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
		VkSampler createTextureSampler();
		void transitionImageLayout(VkCommandPool commandPool, VkQueue queue, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void cleanUpTexture(VkImage textureImage, VkDeviceMemory textureImageMemory,
			VkImageView textureImageView, VkSampler textureSampler);
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

		/** Descriptor sets */
		void createDescriptorSets(std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT>& outDescriptorSets);
		void updateDescriptorSet(VkDescriptorSet descriptorSet, const std::shared_ptr<VulkanTexture> texture);

		/** Drawing */
		void updateSceneBuffer(uint32_t currentFrame);
		void drawFrame() override;
		void updateDrawContext(const Scene& scene) override;
		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

		/** Getters */
		std::vector<VkDevice>& getDevices();

	private:
		/** Setup */
		void initVulkan();
		void createInstance();
		void selectPhysicalDevices();
		void createLogicalDevices();

		void createSwapChain();
		void recreateSwapChain();
		void cleanupSwapChain();

		void createImageViews();
		void createRenderPass();
		void createOpaquePipeline();
		void createFrameBuffers();
		void createCommandPools();
		void createCommandBuffers();

		void createDescriptorSetLayouts();
		void createDescriptorPools();
		void createSceneBuffers();

		/** Depth buffer */
		void createDepthResources();
		VkFormat findDepthFormat();

		void createSyncObjects();
		
		bool checkValidationLayerSupport();
		
		/** Utils/queries */
		bool isDeviceSuitable(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates,
			VkImageTiling tiling, VkFormatFeatureFlags features);

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

	private:

		/* Base data */
		std::shared_ptr<Window> window{};

		VkInstance vulkanInstance{};
		std::vector<VkPhysicalDevice> physicalDevices{ VK_NULL_HANDLE };
		std::vector<VkDevice> devices{ VK_NULL_HANDLE };
		VkSurfaceKHR surface = VK_NULL_HANDLE;

		/** Queues */
		std::vector<QueueFamilyIndices> queueFamilies{};
		VkQueue graphicsQueue{};
		VkQueue presentQueue{};
		VkQueue transferQueue{};
		
		/** Swap chain */
		VkSwapchainKHR swapChain{};
		std::vector<VkImage> swapChainImages{};
		std::vector<VkImageView> swapChainImageViews{};
		VkFormat swapChainImageFormat{};
		VkExtent2D swapChainExtent{};
		bool frameBufferResized = false;

		/** Render pass */
		VkRenderPass renderPass{};

		/** Descriptor sets */
		std::array<VkDescriptorSetLayout, 2> descriptorSetLayouts{};
		std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> sceneDescriptorSets{};

		/** Pipelines */
		VulkanMaterialPipeline opaquePipeline;

		/** Buffers */
		std::vector<VkFramebuffer> framebuffers{};

		/** Command pools */
		VkCommandPool graphicsCommandPool{};
		VkCommandPool transferCommandPool{};

		/** Depth buffer */
		VkImage depthImage{};
		VkDeviceMemory depthImageMemory{};
		VkImageView depthImageView{};

		/** Frames */
		uint32_t currentFrame = 0;
		std::array<FrameData, MAX_FRAMES_IN_FLIGHT> frames;

		/** Debug data */
		VkDebugUtilsMessengerEXT debugMessenger{};
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

		/** -- Draw context -- */
		struct RenderObject
		{
			VkBuffer vertexBuffer{};
			VkBuffer indexBuffer{};
			size_t indexBufferSize = 0;
			VkDeviceSize vertexBufferOffset{0};
			std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> desc{};
			sml::mat4 transform;
		};
		std::vector<RenderObject> renderObjects{};
		SceneBufferObject sceneBufferObject;

		/** To-draw vertex and index data */
		std::vector<VkBuffer> drawnVertexBuffers{};
		std::vector<VkBuffer> drawnIndexBuffers{};
		std::vector<size_t> drawnIndexBufferSizes{};
		std::vector<VkDeviceSize> vertexBufferOffsets = {0};

		/** Owned data. Don't forget to delete this */
		std::vector<VkBuffer> vertexBuffers{};
		std::vector<VkDeviceMemory> vertexBufferMemories{};
		std::vector<VkBuffer> indexBuffers{};
		std::vector<VkDeviceMemory> indexBufferMemories{};
		std::vector<VkImage> images{};
		std::vector<VkImageView> imageViews{};
		std::vector<VkDeviceMemory> imageMemories{};
		std::vector<VkSampler> imageSamplers{};
	};
} // namespace te

#endif // _VULKAN_DEVICE_HPP_