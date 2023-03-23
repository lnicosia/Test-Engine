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
		SDLWindow();
		SDLWindow(int w, int h);
		~SDLWindow();
		SDL_Window* getWindow() const;
	private:
		SDL_Window* windowPtr;
	};

}

#endif // _SDLWINDOW_HPP_