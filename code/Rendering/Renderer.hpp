#ifndef _RENDERER_HPP_
# define _RENDERER_HPP_

#include "Window.hpp"
#include "FrameData.hpp"
#include "DebugLevel.hpp"
#include "Assets/Fonts/Font.hpp"
#include "RendererType.hpp"
#include "WindowManager.hpp"

namespace te
{
	/* Renderer init functions */
	int initSoftwareRenderer();
	int initOpenGL();
	int initVulkan();
	int initD3D();
	int initMetal();
	/* Renderer destroy functions */
	int destroySoftwareRenderer();
	int destroyOpenGL();
	int destroyVulkan();
	int destroyD3D();
	int destroyMetal();

	class Renderer
	{
	public:
		explicit Renderer(RendererType rType = RendererType::TE_VULKAN, WindowManager wManager = WindowManager::TE_SDL);
		Renderer() = delete;
		~Renderer();

		virtual void render() = 0;
		virtual void renderText(const char* text, std::shared_ptr<Font> font,
			Point2<int> pos, int size) = 0;

		std::shared_ptr<Window> getWindow() const;
		const RendererType getType() const;
		const WindowManager getWindowManager() const;

		FrameData frameData;
		DebugLevel	debugLevel;

		bool running;

	private:

		std::function<int()> initRenderer[6] = { &initSoftwareRenderer, &initOpenGL, &initVulkan, &initD3D, &initMetal, std::function<int()>() };
		std::function<int()> destroyRenderer[6] = { &destroySoftwareRenderer, &destroyOpenGL, &destroyVulkan, &destroyD3D, &destroyMetal, std::function<int()>() };
		
	protected:
		RendererType rType;
		WindowManager wManager;
		
		std::shared_ptr<Window> window;
		std::shared_ptr<Font> uiFont;
	};

}

#endif // _RENDERER_HPP_