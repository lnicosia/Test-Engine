#ifndef _RENDERER_HPP_
# define _RENDERER_HPP_

#include "Window.hpp"
#include "Inputs/Events.hpp"
#include "FrameData.hpp"
#include "DebugLevel.hpp"
#include "Assets/Fonts/Font.hpp"

namespace te
{
	enum RendererType
	{
		TE_SOFTWARE_SDL,
		TE_GL_SDL,
		TE_GL_VULKAN,
		TE_UNKNOWN_RENDERER
	};
	class Renderer
	{
	public:
		Renderer(RendererType type);
		Renderer();
		~Renderer();
		RendererType type;

		virtual void render() = 0;
		virtual void renderText(const char* text, std::shared_ptr<Font> font,
			Point2<int> pos, int size) = 0;
		int	handleEvents();

		std::shared_ptr<Window> getWindow() const;

		std::shared_ptr<Events> events;

		FrameData frameData;

		DebugLevel	debugLevel;

	private:
		
	protected:
		std::shared_ptr<Window> window;
		std::shared_ptr<Font> uiFont;
	};

}

#endif // _RENDERER_HPP_