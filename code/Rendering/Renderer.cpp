#include "Renderer.hpp"
#include "Log.hpp"
#include "Assets/AssetManager.hpp"
#include "SDL.hpp"

#include "SDL_ttf.h"

namespace te
{

	int	initSoftwareRenderer()
	{
		// TODO
		return 0;
	}

	int	initOpenGL()
	{
		// TODO
		return 0;
	}

	int	initVulkan()
	{
		// TODO
		return 0;
	}

	int	initD3D()
	{
		// TODO
		return 0;
	}

	int	initMetal()
	{
		// TODO
		return 0;
	}

	int	destroySoftwareRenderer()
	{
		// TODO
		return 0;
	}

	int	destroyOpenGL()
	{
		// TODO
		return 0;
	}

	int	destroyVulkan()
	{
		// TODO
		return 0;
	}

	int	destroyD3D()
	{
		// TODO
		return 0;
	}

	int	destroyMetal()
	{
		// TODO
		return 0;
	}

	Renderer::Renderer(RendererType rType, WindowManager wManager):
		window(nullptr), rType(rType), wManager(wManager),
		debugLevel(DebugLevel::TE_SHOW_FPS),
		uiFont(nullptr),
		frameData(),
		running(true)
	{
		//this->initRenderer[static_cast<int>(rType)]();
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

		//this->destroyRenderer[static_cast<int>(rType)]();
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