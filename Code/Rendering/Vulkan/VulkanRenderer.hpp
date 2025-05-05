#ifndef _VULKAN_RENDERER_HPP_
# define _VULKAN_RENDERER_HPP_

#include "../Renderer.hpp"
#include "../Assets/Textures/VulkanTexture.hpp"
#include "../Vertex.hpp"
#include "QueueFamilyIndices.hpp"

#include "vulkan/vulkan.h"

namespace te
{
	class VulkanRenderer: public Renderer
	{
	public:
		VulkanRenderer();
		VulkanRenderer(WindowAPI wManager);
		~VulkanRenderer();

		void render() override;
		void renderText(const char* text, std::shared_ptr<Font> font,
			Point2<int> pos, int size) override;

		std::shared_ptr<MeshInternal> createSolidMesh(const MeshGeometry& geometry) override;
		virtual std::shared_ptr<MeshInternal> loadMesh(const std::string& path) override;
		virtual std::shared_ptr<Texture> loadTexture(const std::string& path) override;

		std::shared_ptr<VulkanDevice> getVulkanDevice();

	private:

		std::shared_ptr<VulkanDevice> vulkanDevice;
	};
}

#endif // _VULKAN_RENDERER_HPP_