#ifndef _SDLWINDOW_HPP_
# define _SDLWINDOW_HPP_

#include "SDL.h"

#include "Window.hpp"
#include "Log.hpp"
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

		int	loadVulkan() override;
		int getVulkanInstanceExtensions(uint32_t* count, const char** names) override;
		int loadOpenGL() override;

	private:

		SDL_Window* windowPtr;

		void initWindow(RendererType rType);
	};

}

#endif // _SDLWINDOW_HPP_