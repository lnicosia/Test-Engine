#ifndef _SOFTWARE_RENDERER_HPP_
# define _SOFTWARE_RENDERER_HPP_

#include "SDL_ttf.h"

#include "Renderer.hpp"
#include "SDLWindow.hpp"

namespace te
{
	class SoftwareRenderer: public Renderer
	{
	public:
		/*
		* At the moment, only SDL software rendering is possible so this whole
		* class assumes that we are using an SDL window
		*/
		SoftwareRenderer();
		~SoftwareRenderer();

		void render() override;
		void renderText(const char* text, std::shared_ptr<Font> font,
			Point2<int> pos, int size) override;
		void showFrameStats();

		uint32_t*		pixels;
		int w;
		int h;

	private:
		SDL_Renderer*	SDLRendererPtr;
		SDL_Texture*	SDLTexturePtr;
	};

}

#endif // _SOFTWARE_RENDERER_HPP_