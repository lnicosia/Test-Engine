#include "SoftwareRenderer.hpp"
#include "Inputs/SDLEvents.hpp"
#include "Assets/Fonts/TTFFont.hpp"
#include "Assets/AssetManager.hpp"

namespace te
{
	SoftwareRenderer::SoftwareRenderer(): Renderer(TE_SOFTWARE_SDL)
	{
		LOG("Initializing all SDL systems");
		if (SDL_InitSubSystem(SDL_INIT_EVERYTHING) != 0)
			throw std::runtime_error( "Couldn't initialize SDL : \n" + std::string(SDL_GetError()));

		LOG("Initializing SDL_ttf");
		if (TTF_Init() == -1)
			throw std::runtime_error( "Couldn't initialize SDL_ttf : \n" + std::string(TTF_GetError()));

		std::shared_ptr<SDLWindow> winPtr = std::shared_ptr<SDLWindow>(new SDLWindow(800, 600));
		window = winPtr;
		w = window->getWidth();
		h = window->getHeight();

		events = std::shared_ptr<SDLEvents>(new SDLEvents());

		LOG("Initializing SDL renderer");
		SDLRendererPtr = SDL_CreateRenderer(winPtr->getWindow(), nullptr, SDL_RENDERER_SOFTWARE);
		if (!SDLRendererPtr)
			throw std::runtime_error("Could not create SDL renderer");

		LOG("Initializing SDL texture");
		SDLTexturePtr = SDL_CreateTexture(SDLRendererPtr, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_STREAMING, w, h);
		if (!SDLTexturePtr)
			throw std::runtime_error("Could not create SDL texture");

		LOG("Initializing pixel array");
		pixels = new uint32_t[window->getWidth() * window->getHeight()];
		if (!pixels)
			throw std::runtime_error("Could not allocate pixel array");

		AssetManager& assetManager = AssetManager::getInstance();
		uiFont = assetManager.loadAsset<TTFFont>("../../../../resources/fonts/Alice-Regular.ttf");
		
	}

	SoftwareRenderer::~SoftwareRenderer()
	{
		uiFont.reset();
		AssetManager::getInstance().clear();

		LOG("Freeing pixel array");
		delete[] pixels;

		LOG("Destroying SDL_Texture");
		SDL_DestroyTexture(SDLTexturePtr);
		LOG("Destroying SDL_Renderer");
		SDL_DestroyRenderer(SDLRendererPtr);
		LOG("Manually releasing SDL Window");
		window.reset();

		LOG("Quitting SDL TTF");
		TTF_Quit();
		/* LEAK ?? */
		LOG("Quitting SDL subsystems");
		SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
		LOG("Quitting SDL");
		SDL_Quit();
	}

	void	SoftwareRenderer::render()
	{
		SDL_UpdateTexture(SDLTexturePtr, nullptr, pixels, sizeof(Uint32) * window->getWidth());
		SDL_RenderTexture(SDLRendererPtr, SDLTexturePtr, NULL, NULL);
		SDL_RenderPresent(SDLRendererPtr);
		frameData.updateFrameStats();
		showFrameStats();
	}

	void	SoftwareRenderer::showFrameStats()
	{
		if (debugLevel == TE_SHOW_FPS)
		{
			char	buff[10];

			snprintf(buff, 10, "%llu", frameData.fps);
			renderText(buff, uiFont, Point2<int>(), 16);
		}
	}

	/* TODO: protections
	*/
	void	SoftwareRenderer::renderText(const char* text, std::shared_ptr<Font> font,
		Point2<int> pos, int size)
	{
		std::shared_ptr<TTFFont> ttfFont = dynamic_pointer_cast<TTFFont>(font);
		if (!ttfFont)
			throw std::runtime_error("Software renderer is trying to render a " + font->getAssetType()
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