#ifndef _VULKAN_DEVICE_HPP_
#define _VULKAN_DEVICE_HPP_

#include "Rendering/Window/Window.hpp"
#include "Rendering/GPUDevice.hpp"
#include "Rendering/DeletionQueue.hpp"
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
			VkSemaphore computeFinishedSemaphore{};
			VkFence computeInFlightFence{};

			/** Command buffer */
			VkCommandBuffer graphicsCommandBuffer{};
			VkCommandBuffer computeCommandBuffer{};

			/** Descriptors */
			VulkanDescriptorAllocator sceneDescriptorAllocator{};
			VulkanDescriptorAllocator meshDescriptorAllocator{};
			VulkanDescriptorAllocator particleDescriptorAllocator{};
			
			/** Scene buffers */
			VkBuffer sceneBuffer{};
			VkDeviceMemory sceneBufferMemory{};
			void* sceneBufferMapped{};

			/** Compute */
			VkBuffer computeBuffer{};
			VkDeviceMemory computeBufferMemory{};

			DeletionQueue deletionQueue;
			bool mustUpdateCamera = true;
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
		void createTextureImage(const std::string& path,
			VkImage& outImage, VkDeviceMemory& outImageMemory, 
			uint32_t& outMipLevels);
		void createImage(uint32_t width, uint32_t height, uint32_t mipLevels,
			VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling,
			VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
			VkImage& outImage, VkDeviceMemory& outImageMemory);
		VkImageView createImageView(VkImage image, VkFormat format,
			VkImageAspectFlags aspectFlags, uint32_t mipLevels);
		VkSampler createTextureSampler(uint32_t mipLevels = 1);
		void transitionImageLayout(VkCommandPool commandPool, VkQueue queue,
			VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout,
			uint32_t mipLevels);
		void cleanUpTexture(VkImage textureImage, VkDeviceMemory textureImageMemory,
			VkImageView textureImageView, VkSampler textureSampler);
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		void generateMipmaps(VkImage image, VkFormat imageFormat,
			int32_t width, int32_t height, uint32_t mipLevels);

		/** Descriptor sets */
		void createMeshDescriptorSets(std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT>& outDescriptorSets);
		void createParticleDescriptorSets(std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT>& outDescriptorSets);
		void updateMeshDescriptorSet(VkDescriptorSet descriptorSet, const std::shared_ptr<VulkanTexture> texture);
		void updateParticleDescriptorSet(VkDescriptorSet descriptorSet,
			VkBuffer firstBuffer, VkBuffer secondBuffer, size_t bufferSize);

		/** Drawing */
		void updateSceneBuffer(uint32_t currentFrame);
		void drawFrame(const Camera& camera, float deltaTime) override;
		void updateDrawContext(const Scene& scene) override;
		void updateCameraDescriptors(uint32_t frameIndex);
		void updateCameraContext(const Camera& camera) override;

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
		void createComputePipeline();
		void createParticleGraphicsPipeline();
		void createFrameBuffers();

		void createCommandPools();
		void createCommandBuffers();

		void createDescriptorSetLayouts();
		void createDescriptorPools();
		void createSceneBuffers();
		
		/** Compute data creation */
		void createTMPComputeBuffer();
		void createParticleDescriptorSet(VkDescriptorSet VkDescriptorSetLayout, VkBuffer buffer);

		/** Depth buffer */
		void createDepthResources();
		VkFormat findDepthFormat();
		void createColorResources();

		void createSyncObjects();
		
		/** Command records */
		void recordGraphicsCommandBuffer(VkCommandBuffer commandBuffer,
			uint32_t imageIndex);
		void recordComputeCommandBuffer(VkCommandBuffer commandBuffer,
			uint32_t imageIndex, float deltaTime);

		bool checkValidationLayerSupport();
		
		/** Utils/queries */
		bool isDeviceSuitable(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates,
			VkImageTiling tiling, VkFormatFeatureFlags features);
		VkSampleCountFlagBits getMaxUsableSampleCount();

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
		VkQueue computeQueue{};
		
		/** Swap chain */
		VkSwapchainKHR swapChain{};
		std::vector<VkImage> swapChainImages{};
		std::vector<VkImageView> swapChainImageViews{};
		VkFormat swapChainImageFormat{};
		VkExtent2D swapChainExtent{};
		bool frameBufferResized = false;

		/** Render pass */
		VkRenderPass MSAArenderPass{};

		/** Descriptor sets */
		VkDescriptorSetLayout sceneDescriptorSetLayout{};
		VkDescriptorSetLayout meshDescriptorSetLayout{};
		VkDescriptorSetLayout particleDescriptorSetLayout{};
		std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> sceneDescriptorSets{};

		/** Basic pipelines */
		VulkanPipeline opaquePipeline{};
		VulkanPipeline particleGraphicsPipeline{};
		VulkanPipeline computePipeline{};

		/** Buffers */
		std::vector<VkFramebuffer> framebuffers{};

		/** Command pools */
		VkCommandPool graphicsCommandPool{};
		VkCommandPool transferCommandPool{};
		VkCommandPool computeCommandPool{};

		/** Depth buffer */
		VkImage depthImage{};
		VkDeviceMemory depthImageMemory{};
		VkImageView depthImageView{};

		/** TMP hardcoded Compute */
		std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> particleDescriptorSets{};

		/** Multisampling */
		VkSampleCountFlagBits MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		VkSampleCountFlagBits maxMSAASamples = VK_SAMPLE_COUNT_1_BIT;
		VkImage colorImage{};
		VkDeviceMemory colorImageMemory{};
		VkImageView colorImageView{};

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