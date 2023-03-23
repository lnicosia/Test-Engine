#include "SDLWindow.hpp"

namespace te
{

	SDLWindow::SDLWindow(): Window(), windowPtr(nullptr)
	{
		windowPtr = SDL_CreateWindow("SDL Window", w, h, 0);
		if (!windowPtr)
			throw std::runtime_error("Could not create window");
	}

	SDLWindow::SDLWindow(int w, int h): Window(w, h), windowPtr(nullptr)
	{
		windowPtr = SDL_CreateWindow("SDL Window", w, h, 0);
		if (!windowPtr)
			throw std::runtime_error("Could not create window");
	}

	SDLWindow::~SDLWindow()
	{
		LOG("Destroying SDL Window");
		SDL_DestroyWindow(windowPtr);
	}

	SDL_Window* SDLWindow::getWindow() const
	{
		return windowPtr;
	}
}