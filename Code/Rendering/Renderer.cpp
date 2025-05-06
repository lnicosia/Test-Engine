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
			if (camera.bIsDirty)
			{
				device->updateCameraContext(camera);
				camera.bIsDirty = false;
			}
			device->drawFrame(camera);
		}
	}

	void Renderer::initCameraBindings()
	{
		Binding forward("forward", SDLK_UP, SDLK_w, true);
		forward.whenPressed = std::make_shared<Action<void>>([this]()
		{
			camera.moveForward();
		});
		window->events->bindings.push_back(forward);

		Binding backward("backward", SDLK_DOWN, SDLK_s, true);
		backward.whenPressed = std::make_shared<Action<void>>([this]()
		{
			camera.moveBackward();
		});
		window->events->bindings.push_back(backward);

		Binding left("left", SDLK_LEFT, SDLK_a, true);
		left.whenPressed = std::make_shared<Action<void>>([this]()
		{
			camera.moveLeft();
		});
		window->events->bindings.push_back(left);

		Binding right("right", SDLK_RIGHT, SDLK_d, true);
		right.whenPressed = std::make_shared<Action<void>>([this]()
		{
			camera.moveRight();
		});
		window->events->bindings.push_back(right);

		/*MouseBinding leftB("Mouse left", SDL_BUTTON_LEFT, 0, false);
		leftB.onRelease = std::make_shared<ActionWrapper>(Action<void, GameState&>(std::function<void(GameState&)>(ChangeGameState), gameState));
		window->events->mouseBindings.push_back(leftB);

		Binding updateRotate("updateRotate", SDLK_ESCAPE, 0, true);
		updateRotate.onRelease = leftB.onRelease;
		window->events->bindings.push_back(updateRotate);*/
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