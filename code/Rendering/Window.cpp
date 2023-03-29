#include "Window.hpp"

#include <stdexcept>

namespace te
{
	Window::Window(RendererType rType):
		w(1600), h(900), ratio(w / (double)h), resizable(false),
		rType(rType)
	{
		if (rType == RendererType::TE_UNKNOWN_RENDERER)
			throw std::runtime_error("Cannot create Window: renderer is null");
	}

	Window::Window(int w, int h, RendererType rType):
		w(w), h(h), ratio(w / (double)h), resizable(false),
		rType(rType)
	{
		if (rType == RendererType::TE_UNKNOWN_RENDERER)
			throw std::runtime_error("Cannot create Window: renderer is null");
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

	int	Window::handleEvents()
	{
		return events->handle();
	}
}