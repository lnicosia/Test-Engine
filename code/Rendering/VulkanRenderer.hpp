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
		VkInstance instance;

		void initVulkan();
		void createInstance();

		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

		bool checkValidationLayerSupport();

	};
}

#endif // _VULKAN_RENDERER_HPP_