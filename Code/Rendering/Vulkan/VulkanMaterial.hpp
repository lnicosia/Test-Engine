#ifndef _VULKAN_MATERIAL_HPP_
#define _VULKAN_MATERIAL_HPP_

#include "Rendering/Materials/Material.hpp"

#include "vulkan/vulkan.h"

class VulkanTexture;

namespace te
{
	
	struct VulkanPipeline
	{
		VkPipeline pipeline{};
		VkPipelineLayout layout{};
	};

	struct VulkanMaterial : public Material
	{

	public:

		VulkanMaterial();

	public:
		
		VulkanPipeline* pipeline{};
		VkDescriptorSet desc{};
	};
} // namespace te


#endif // _VULKAN_MATERIAL_HPP_