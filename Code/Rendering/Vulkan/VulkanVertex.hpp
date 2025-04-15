#ifndef _VULKAN_VERTEX_HPP_
#define _VULKAN_VERTEX_HPP_

#include "../Vertex.hpp"

namespace te
{
	struct VulkanVertex// : public Vertex
	{
		//constexpr VulkanVertex(const sml::vec3& pos, const sml::vec3& color): Vertex(pos, color) {}

		sml::vec3 pos;
		sml::vec3 color;
		static constexpr VkVertexInputBindingDescription getBindingDescription()
		{
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(VulkanVertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			return bindingDescription;
		}

		static constexpr std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
		{
			std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);

			return attributeDescriptions;
		}
	};
};

#endif // _VULKAN_VERTEX_HPP_