#include "SDLWindow.hpp"
#include "SDL.hpp"
#include "Inputs/SDLEvents.hpp"
#include "Debug/Exception.hpp"

#include "Platform.hpp"
#ifdef TE_WINDOWS
# include <windows.h>
#endif

#include "SDL3/SDL_vulkan.h"
#include "SDL3/SDL_video.h"

#include "vulkan/vulkan.h"

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
		TE_LOG(TE_RENDERING_LOG, TE_VERYVERBOSE, "Destroying SDL Window\n");
		SDL_DestroyWindow(windowPtr);
		SDL::Quit();
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
		{
			ThrowException("SDL can only create Vulkan, OpenGL or software windows");
		}
		log += "window";
		TE_LOG(TE_RENDERING_LOG, TE_VERBOSE, "%s\n", log.c_str());

		title = "Test Engine - SDL Window - Configuration type: ";
#ifdef NDEBUG
		title += " Release";
#else
		title += " Debug";
		#endif
#ifdef ENABLE_VALIDATION_LAYERS
		title += " with Vulkan Validation layers";
#endif

		title += " | 0.0ms - 0 fps";
		
		windowPtr = SDL_CreateWindow(title.c_str(), w, h, flags);
		if (!windowPtr)
		{
			ThrowException("Could not create SDL window: " + std::string(SDL_GetError()) + "\n");
		}

		events = std::make_shared<SDLEvents>();
	}

	int SDLWindow::loadVulkan()
	{
		return 0;
	}

	int SDLWindow::getVulkanInstanceExtensions(uint32_t* count, std::vector<const char*>& names,
		bool enableValidationLayers)
	{
        auto instance_extensions = SDL_Vulkan_GetInstanceExtensions(count);
        if (instance_extensions == nullptr)
        {
            ThrowException("Could not query Vulkan instance extensions count");
            return 1;
        }

		TE_LOG(TE_RENDERING_LOG, TE_VERBOSE, "SDL requires %d vulkan extensions:\n", *count);
		names.resize(*count);
        // copy the extensions to the vector
        for (auto i = 0u; i < *count; i++)
        {
            names[i] = instance_extensions[i];
        }
		for (const char* name : names)
		{
			TE_LOG(TE_RENDERING_LOG, TE_VERBOSE, "\t'%s'\n", name);
		}
		if (enableValidationLayers)
		{
			names.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			(*count)++;
		}
		return 0;
	}

	int SDLWindow::createVulkanSurface(VkInstance instance, VkSurfaceKHR* surface)
	{

/* PLATFORM SPECIFIC AND MANUAL SURFACE CREATION
#ifdef TE_WINDOWS
		VkWin32SurfaceCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		//createInfo.hwnd = glfwGetWin32Window(window);
		createInfo.hinstance = GetModuleHandle(nullptr);
		if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, surface) != VK_SUCCESS)
		{
			TE_LOG(TE_RENDERING_LOG, TE_ERROR, "Could not create SDL Vulkan surface\n");
		}
#elif TE_LINUX
#endif*/

		/* SDL 3.0 handles the platform itself */
		if (*surface != VK_NULL_HANDLE)
		{
			TE_LOG(TE_RENDERING_LOG, TE_ERROR, "Vulkan surface already exists\n");
		}
		if (!SDL_Vulkan_CreateSurface(windowPtr, instance, nullptr, surface)
			|| surface == nullptr)
		{
            if (surface == nullptr)
            {
                std::cout << "C'est d'la merde" << std::endl;
            }
			ThrowException("Could not create SDL Vulkan surface: " + std::string(SDL_GetError()));
		}

		return 0;
	}

	int SDLWindow::getFrameSize(int* w, int* h)
	{
		SDL_GetWindowSizeInPixels(this->windowPtr, w, h);
		return 0;
	}

	void SDLWindow::hideCursor()
	{
		SDL_GetGlobalMouseState(&preHideMousePos.x, &preHideMousePos.y);
		SDL_HideCursor();
		SDL_SetWindowRelativeMouseMode(windowPtr, true);
	}

	void SDLWindow::showCursor()
	{
		SDL_ShowCursor();
		SDL_SetWindowRelativeMouseMode(windowPtr, false);
		SDL_WarpMouseGlobal(preHideMousePos.x, preHideMousePos.y);
	}

	void SDLWindow::startRecordingMouse()
	{
		events->mouseStart = events->mouseRelativePos;
	}

	int SDLWindow::loadOpenGL()
	{
		return 0;
	}

	void SDLWindow::setTitle(const std::string& newTitle)
	{
		title = newTitle;
		SDL_SetWindowTitle(windowPtr, title.c_str());
	}

	SDL_Window* SDLWindow::getWindow() const
	{
		return windowPtr;
	}
}