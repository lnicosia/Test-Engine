#ifndef _WINDOW_HPP_
# define _WINDOW_HPP_

#include "Maths/math_tmp.hpp"
#include "../RendererType.hpp"
#include "Inputs/Events.hpp"

#include "Platform.hpp"
#ifdef TE_WINDOWS
# define VK_USE_PLATFORM_WIN32_KHR
#endif

#include "vulkan/vulkan.h"

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

		std::string getTitle() const;

		/* Vulkan specifics */
		virtual int	loadVulkan() = 0;
		virtual int getVulkanInstanceExtensions(uint32_t* count, std::vector<const char*>& names,
			bool enableValidationLayers) = 0;
		virtual int createVulkanSurface(VkInstance instance, VkSurfaceKHR* surface) = 0;

		/* OpenGL specifics */
		virtual int loadOpenGL() = 0;

		/** Utils */
		virtual int getFrameSize(int* w, int* h) = 0;
		virtual void hideCursor() = 0;
		virtual void showCursor() = 0;
		virtual void startRecordingMouse() = 0;
		virtual void setTitle(const std::string& newTitle) = 0;

	public:

		std::shared_ptr<Events> events{};

	private:

		bool resizable;
		double ratio;
		RendererType rType;

	protected:

		int w = 1600;
		int h = 900;
		
		std::string title;
	};


}

#endif // _WINDOW_HPP_