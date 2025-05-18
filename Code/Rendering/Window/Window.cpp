#include "Window.hpp"
#include "Debug/Exception.hpp"

#include <stdexcept>

namespace te
{
	Window::Window(RendererType rType):
		resizable(false), ratio(w / (double)h), rType(rType)
	{
		if (rType == RendererType::TE_UNKNOWN_RENDERER)
		{
			ThrowException("Cannot create Window: renderer is null");
		}
	}

	Window::Window(int w, int h, RendererType rType):
		w(w), h(h), resizable(false), ratio(w / (double)h), rType(rType)
	{
		if (rType == RendererType::TE_UNKNOWN_RENDERER)
		{
			ThrowException("Cannot create Window: renderer is null");
		}
	}

	Window::~Window()
	{
	}

	std::string Window::getTitle() const
	{
		return title;
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