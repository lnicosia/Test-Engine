#include "Renderer.hpp"
#include "Log.hpp"
#include "Assets/AssetManager.hpp"
#include "SDL.hpp"

#include "SDL_ttf.h"

namespace te
{

	Renderer::Renderer(RendererType rType, WindowManager wManager):
		window(nullptr), rType(rType), wManager(wManager),
		debugLevel(DebugLevel::TE_SHOW_FPS),
		uiFont(nullptr),
		frameData(),
		running(true)
	{
	}

	Renderer::~Renderer()
	{
		/* Make sure to clear assets before releasing window libraries */
		LOG("Clearing renderer UI font");
		uiFont.reset();
		LOG("Freeing all assets");
		AssetManager::getInstance().clear();

		LOG("Releasing Window");
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