#include "SoftwareRenderer.hpp"
#include "Inputs/SDLEvents.hpp"
#include "Assets/Fonts/TTFFont.hpp"
#include "Assets/AssetManager.hpp"
#include "Debug/Exception.hpp"

namespace te
{
	SoftwareRenderer::SoftwareRenderer(): Renderer(RendererType::TE_SOFTWARE, WindowManager::TE_SDL)
	{

		std::shared_ptr<SDLWindow> winPtr = std::shared_ptr<SDLWindow>(
			new SDLWindow(800, 600, RendererType::TE_SOFTWARE));
		window = winPtr;
		w = window->getWidth();
		h = window->getHeight();

		LOG(TE_RENDERING_LOG, TE_LOG, "Initializing SDL renderer\n");
		SDLRendererPtr = SDL_CreateRenderer(winPtr->getWindow(), nullptr, SDL_RENDERER_SOFTWARE);
		if (!SDLRendererPtr)
			ThrowException("Could not create SDL renderer");

		LOG(TE_RENDERING_LOG, TE_LOG, "Initializing SDL texture\n");
		SDLTexturePtr = SDL_CreateTexture(SDLRendererPtr, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_STREAMING, w, h);
		if (!SDLTexturePtr)
			ThrowException("Could not create SDL texture");

		LOG(TE_RENDERING_LOG, TE_LOG, "Initializing pixel array\n");
		pixels = new uint32_t[window->getWidth() * window->getHeight()];
		if (!pixels)
			ThrowException("Could not allocate pixel array");

		AssetManager& assetManager = AssetManager::getInstance();
		uiFont = assetManager.loadAsset<TTFFont>(Logger::ROOT_DIR_PATH + "Resources/Fonts/Alice-Regular.ttf");
		
	}

	SoftwareRenderer::~SoftwareRenderer()
	{

		LOG(TE_RENDERING_LOG, TE_LOG, "Freeing pixel array\n");
		delete[] pixels;

		LOG(TE_RENDERING_LOG, TE_LOG, "Destroying SDL_Texture\n");
		SDL_DestroyTexture(SDLTexturePtr);
		LOG(TE_RENDERING_LOG, TE_LOG, "Destroying SDL_Renderer\n");
		SDL_DestroyRenderer(SDLRendererPtr);
	}

	void	SoftwareRenderer::render()
	{
		SDL_UpdateTexture(SDLTexturePtr, nullptr, pixels, sizeof(Uint32) * window->getWidth());
		SDL_RenderTexture(SDLRendererPtr, SDLTexturePtr, NULL, NULL);
		SDL_RenderPresent(SDLRendererPtr);
		frameStats.updateFrameStats();
		showFrameStats();
	}

	void	SoftwareRenderer::showFrameStats()
	{
		if (debugLevel == DebugLevel::TE_SHOW_FPS)
		{
			char	buff[10];

			snprintf(buff, 10, "%lu", frameStats.fps);
			renderText(buff, uiFont, Point2<int>(), 16);
		}
	}

	/* TODO: protections
	*/
	void	SoftwareRenderer::renderText(const char* text, std::shared_ptr<Font> font,
		Point2<int> pos, int size)
	{
		if (!font)
		{
			LOG(TE_RENDERING_LOG, TE_ERROR, "RenderText font in nullptr\n");
			return ;
		}
		std::shared_ptr<TTFFont> ttfFont = dynamic_pointer_cast<TTFFont>(font);
		if (!ttfFont)
			ThrowException("Software renderer is trying to render a " + font->getAssetType()
				+ " instead of a TTF font");

		SDL_Color color{ 255, 255, 255 };
		SDL_Surface* surf = TTF_RenderText_Blended(ttfFont->getPtr(), text, color);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(SDLRendererPtr, surf);
		int texW = 0;
		int texH = 0;
		SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
		SDL_FRect rec{
			(float)(w - texW - 10),
			(float)(h - texH - 10),
			(float)(texW),
			(float)(texH)
		};
		SDL_RenderTexture(SDLRendererPtr, texture, nullptr, &rec);
		SDL_DestroyTexture(texture);
		SDL_DestroySurface(surf);
	}
}