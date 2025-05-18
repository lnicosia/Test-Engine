#include "Renderer.hpp"
#include "Debug/Log.hpp"
#include "Assets/AssetManager.hpp"
#include "Window/SDL.hpp"

namespace te
{

	Renderer::Renderer(RendererType rType, WindowAPI windowAPI):
		debugLevel(DebugLevel::TE_SHOW_FPS),
		rType(rType), windowAPI(windowAPI),
		uiFont(nullptr)
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
		timerManager.update();
		if (debugLevel == DebugLevel::TE_SHOW_FPS)
		{
			timerManager.setTimer(std::make_unique<Action<void>>([this]()
			{
				updateWindowTitle();
			}), 1, true);
		}
		
		while (running == true)
		{
			timerManager.update();
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
			if (camera.bIsDirty)
			{
				camera.updateView();
				device->updateCameraContext(camera);
				device->updateDrawContext(scene);
				camera.bIsDirty = false;
			}
			device->drawFrame(camera, timerManager.getDeltaTimeSec());
		}
	}

	void Renderer::updateWindowTitle()
	{
		std::string title = window->getTitle();
		device->frameStats.fps = 1000.0f / timerManager.getDeltaTimeMili();
		// Can't use std::format because g++ is too old on WSL1 :'(
		//std::string newStats = std::format("| {:.2f}ms - {:.1f} fps", timerManager.getDeltaTimeMili(), fps);
		size_t lastDash = title.find_last_of("|");
		std::stringstream ss;
		ss << "| " << std::fixed << std::setprecision(2) << timerManager.getDeltaTimeMili() << "ms";
		ss << " - " << std::fixed << std::setprecision(1) << device->frameStats.fps << " fps";
		std::string newStats = ss.str();
		title.replace(lastDash, title.size() - lastDash, newStats);
		window->setTitle(title);
	}

	void Renderer::addBinding(const Binding& binding)
	{
		window->events->bindings.push_back(binding);
	}

	void Renderer::addMouseBinding(const MouseBinding& binding)
	{
		window->events->mouseBindings.push_back(binding);
	}

	/** Getters */
	std::shared_ptr<Window> Renderer::getWindow() const
	{
		return window;
	}

	std::shared_ptr<GPUDevice> Renderer::getDevice() const
	{
		return device;
	}

	RendererType Renderer::getType() const
	{
		return rType;
	}

	WindowAPI Renderer::getWindowAPI() const
	{
		return windowAPI;
	}

	const Scene& Renderer::getScene() const
	{
		return scene;
	}

	float Renderer::getDeltaTime() const
	{
		return timerManager.deltaTimeSec;
	}
}