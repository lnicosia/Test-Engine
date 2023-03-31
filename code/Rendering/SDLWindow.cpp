#include "SDLWindow.hpp"
#include "SDL.hpp"
#include "Inputs/SDLEvents.hpp"

#include "SDL_vulkan.h"

namespace te
{

	SDLWindow::SDLWindow(RendererType rType): Window(rType), windowPtr(nullptr)
	{
		initWindow(rType);
	}

	SDLWindow::SDLWindow(int w, int h, RendererType rType): Window(w, h, rType), windowPtr(nullptr)
	{
		initWindow(rType);
	}

	SDLWindow::~SDLWindow()
	{
		LOG(TE_RENDERING_LOG, TE_LOG, "Destroying SDL Window\n");
		SDL_DestroyWindow(windowPtr);
	}

	void	SDLWindow::initWindow(RendererType rType)
	{
		SDL::Init();
		std::string log = "Creating SDL ";

		uint32_t flags = 0;
		if (rType == RendererType::TE_VULKAN)
		{
			flags |= SDL_WINDOW_VULKAN;
			log += "Vulkan ";
		}
		else if (rType == RendererType::TE_GL)
		{
			flags |= SDL_WINDOW_OPENGL;
			log += "OpenGL ";
		}
		else if (rType == RendererType::TE_SOFTWARE)
		{
			flags |= SDL_WINDOW_OPENGL;
			log += "software ";
		}
		else
			throw std::runtime_error("SDL can only create Vulkan, OpenGL or software windows");
		log += "window";
		LOG(TE_RENDERING_LOG, TE_LOG, "%s\n", log.c_str());

		windowPtr = SDL_CreateWindow("SDL Window", w, h, flags);
		if (!windowPtr)
			throw std::runtime_error("Could not create window");

		events = std::shared_ptr<SDLEvents>(new SDLEvents());
	}

	int SDLWindow::loadVulkan()
	{
		return 0;
	}

	int SDLWindow::getVulkanInstanceExtensions(uint32_t* count, std::vector<const char*>& names)
	{
		if (SDL_Vulkan_GetInstanceExtensions(count, nullptr) == SDL_FALSE)
			throw std::runtime_error("Could not querry Vulkan instance extensions count");
		LOG(TE_RENDERING_LOG, TE_LOG, "SDL requires %d vulkan extensions\n", *count);
		names.resize(*count);
		if (SDL_Vulkan_GetInstanceExtensions(count, names.data()) == SDL_FALSE)
			throw std::runtime_error("Could not querry Vulkan instance extensions names");
		return 0;
	}

	int SDLWindow::loadOpenGL()
	{
		return 0;
	}

	SDL_Window* SDLWindow::getWindow() const
	{
		return windowPtr;
	}
}