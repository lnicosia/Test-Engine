#ifndef _SDLWINDOW_HPP_
# define _SDLWINDOW_HPP_

#include "Window.hpp"
#include "Debug/Log.hpp"

#include "SDL.h"

#include <iostream>

namespace te
{
	class SDLWindow: public Window
	{
	public:

		SDLWindow() = delete;
		explicit SDLWindow(RendererType rType);
		explicit SDLWindow(int w, int h, RendererType rType);
		~SDLWindow();
		SDL_Window* getWindow() const;

		/* Vulkan specifics */
		int	loadVulkan() override;
		int getVulkanInstanceExtensions(uint32_t* count, std::vector<const char*>& names,
			bool enableValidationLayers) override;
		int createVulkanSurface(VkInstance instance, VkSurfaceKHR* surface) override;

		/* OpenGL specifics */
		int loadOpenGL() override;

		/* Rendering API independant */
		int getFrameSize(int* w, int* h) override;

	private:

		SDL_Window* windowPtr;

		void initWindow(RendererType rType);
	};

}

#endif // _SDLWINDOW_HPP_