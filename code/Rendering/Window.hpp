#ifndef _WINDOW_HPP_
# define _WINDOW_HPP_

#include "Maths/math.hpp"
#include "RendererType.hpp"
#include "Inputs/Events.hpp"

#include "Platform.hpp"
#ifdef TE_WINDOWS
# define VK_USE_PLATFORM_WIN32_KHR
#endif

#include "Vulkan/vulkan.h"

namespace te
{
	class Window
	{
	public:
		Window() = delete;
		Window(RendererType rType);
		Window(int w, int h, RendererType rType);
		virtual ~Window();
		const int getWidth() const;
		const int getHeight() const;

		int	handleEvents();
		std::shared_ptr<Events> events;

		/* Vulkan specifics */
		virtual int	loadVulkan() = 0;
		virtual int getVulkanInstanceExtensions(uint32_t* count, std::vector<const char*>& names,
			bool enableValidationLayers) = 0;
		virtual int createVulkanSurface(VkInstance instance, VkSurfaceKHR* surface) = 0;

		/* OpenGL specifics */
		virtual int loadOpenGL() = 0;

	private:
		bool	resizable;
		double	ratio;
		RendererType rType;

	protected:
		int w = 1600;
		int h = 900;
		
	};


}

#endif // _WINDOW_HPP_