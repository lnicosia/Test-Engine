#include "Window.hpp"

namespace te
{
	Window::Window(): w(1600), h(900), ratio(w / (double)h), resizable(false)
	{
	}

	Window::Window(int w, int h): w(w), h(h), ratio(w / (double)h), resizable(false)
	{
	}

	Window::~Window()
	{
	}

	const int Window::getWidth() const
	{
		return w;
	}

	const int Window::getHeight() const
	{
		return h;
	}
}