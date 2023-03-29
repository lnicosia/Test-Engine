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
		
	protected:
		RendererType rType;
		WindowManager wManager;
		
		std::shared_ptr<Window> window;
		std::shared_ptr<Font> uiFont;
	};

}

#endif // _RENDERER_HPP_