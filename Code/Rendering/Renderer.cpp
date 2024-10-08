#include "Renderer.hpp"
#include "Debug/Log.hpp"
#include "Assets/AssetManager.hpp"
#include "SDL.hpp"

#include "SDL_ttf.h"

namespace te
{

	Renderer::Renderer(RendererType rType, WindowManager wManager):
		window(nullptr), rType(rType), wManager(wManager),
		debugLevel(DebugLevel::TE_SHOW_FPS),
		uiFont(nullptr),
		frameStats(),
		running(true)
	{
		Logger::Init();
	}

	Renderer::~Renderer()
	{
		/* Make sure to clear assets before releasing window libraries */
		LOG(TE_RESOURCE_LOG, TE_LOG, "Clearing renderer UI font\n");
		uiFont.reset();
		LOG(TE_RESOURCE_LOG, TE_LOG, "Freeing all assets\n");
		AssetManager::getInstance().clear();

		LOG(TE_RENDERING_LOG, TE_LOG, "Releasing Window\n");
		window.reset();

	}

	std::shared_ptr<Window> Renderer::getWindow() const
	{
		return window;
	}

	const RendererType Renderer::getType() const
	{
		return rType;
	}

	const WindowManager Renderer::getWindowManager() const
	{
		return wManager;
	}
}