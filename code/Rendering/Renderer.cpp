#include "Renderer.hpp"

namespace te
{
	Renderer::Renderer(): window(nullptr), events(nullptr),
		type(TE_UNKNOWN_RENDERER),
		debugLevel(TE_SHOW_FPS),
		uiFont(nullptr)
	{

	}
	Renderer::Renderer(RendererType type): window(nullptr), events(nullptr),
		type(type), debugLevel(TE_SHOW_FPS),
		uiFont(nullptr)
	{
	}

	Renderer::~Renderer()
	{
	}

	int	Renderer::handleEvents()
	{
		return events->handle();
	}

	std::shared_ptr<Window> Renderer::getWindow() const
	{
		return window;
	}
}