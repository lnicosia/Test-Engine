#include "Renderer.hpp"
#include "Debug/Log.hpp"
#include "Assets/AssetManager.hpp"
#include "Window/SDL.hpp"

#include "SDL_ttf.h"

namespace te
{

	Renderer::Renderer(RendererType rType, WindowAPI windowAPI):
		window(nullptr), rType(rType), windowAPI(windowAPI),
		debugLevel(DebugLevel::TE_SHOW_FPS),
		uiFont(nullptr), running(true)
	{
		Logger::Init();
	}

	Renderer::~Renderer()
	{
		/* Make sure to clear assets before releasing window libraries */
		TE_LOG(TE_RESOURCE_LOG, TE_VERBOSE, "Clearing renderer UI font\n");
		uiFont.reset();
		TE_LOG(TE_RESOURCE_LOG, TE_VERBOSE, "Freeing all assets\n");
		AssetManager::getInstance().clear();

		TE_LOG(TE_RENDERING_LOG, TE_VERBOSE, "Releasing Window\n");
		window.reset();
	}

	void Renderer::render()
	{
		while (running == true)
		{
			device->frameStats.drawCallCount = 0;
			if (window->handleEvents() == 1)
			{
				running = false;
			}
			if (scene.isDirty())
			{
				device->updateDrawContext(scene);
				scene.setIsDirty(false);
			}
			device->drawFrame();
		}
	}

	std::shared_ptr<Window> Renderer::getWindow() const
	{
		return window;
	}

	std::shared_ptr<GPUDevice> Renderer::getDevice() const
	{
		return device;
	}

	const RendererType Renderer::getType() const
	{
		return rType;
	}

	const WindowAPI Renderer::getWindowAPI() const
	{
		return windowAPI;
	}

	const Scene& Renderer::getScene() const
	{
		return scene;
	}
}