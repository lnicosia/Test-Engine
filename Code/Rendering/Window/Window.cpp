#include "Window.hpp"
#include "Debug/Exception.hpp"

#include <stdexcept>

namespace te
{
	Window::Window(RendererType rType):
		resizable(false), ratio(w / (double)h),
		rType(rType), w(1600), h(900)
	{
		if (rType == RendererType::TE_UNKNOWN_RENDERER)
		{
			ThrowException("Cannot create Window: renderer is null");
		}
	}

	Window::Window(int w, int h, RendererType rType):
		resizable(false), ratio(w / (double)h),
		rType(rType), w(w), h(h)
	{
		if (rType == RendererType::TE_UNKNOWN_RENDERER)
		{
			ThrowException("Cannot create Window: renderer is null");
		}
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