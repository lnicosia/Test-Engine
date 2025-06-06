#include "SoftwareRenderer.hpp"

#include "Inputs/SDLEvents.hpp"

#include "Assets/Fonts/TTFFont.hpp"
#include "Assets/AssetManager.hpp"

#include "Assets/Textures/SoftwareTexture.hpp"
#include "Rendering/SoftwareRendering/SoftwareMesh.hpp"

#include "Debug/Exception.hpp"
#include <cstring>

namespace te
{
	SoftwareRenderer::SoftwareRenderer(): Renderer(RendererType::TE_SOFTWARE, WindowAPI::TE_SDL)
	{

		std::shared_ptr<SDLWindow> winPtr = std::shared_ptr<SDLWindow>(
			new SDLWindow(800, 600, RendererType::TE_SOFTWARE));
		window = winPtr;
		w = window->getWidth();
		h = window->getHeight();

		TE_LOG(TE_RENDERING_LOG, TE_VERBOSE, "Initializing SDL renderer\n");
		SDLRendererPtr = SDL_CreateRenderer(winPtr->getWindow(), NULL);
		if (!SDLRendererPtr)
		{
			ThrowException("Could not create SDL renderer");
		}

		TE_LOG(TE_RENDERING_LOG, TE_VERBOSE, "Initializing SDL texture\n");
		SDLTexturePtr = SDL_CreateTexture(SDLRendererPtr, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_STREAMING, w, h);
		if (!SDLTexturePtr)
		{
			ThrowException("Could not create SDL texture");
		}

		TE_LOG(TE_RENDERING_LOG, TE_VERBOSE, "Initializing pixel array\n");
		pixels = new uint32_t[window->getWidth() * window->getHeight()];
		if (!pixels)
		{
			ThrowException("Could not allocate pixel array");
		}

		AssetManager& assetManager = AssetManager::getInstance();
		uiFont = assetManager.loadAsset<TTFFont>(Logger::getRootDirPath() + "Resources/Fonts/Alice-Regular.ttf");
	}

	SoftwareRenderer::~SoftwareRenderer()
	{

		TE_LOG(TE_RENDERING_LOG, TE_VERBOSE, "Freeing pixel array\n");
		delete[] pixels;

		TE_LOG(TE_RENDERING_LOG, TE_VERBOSE, "Destroying SDL_Texture\n");
		SDL_DestroyTexture(SDLTexturePtr);
		TE_LOG(TE_RENDERING_LOG, TE_VERBOSE, "Destroying SDL_Renderer\n");
		SDL_DestroyRenderer(SDLRendererPtr);
	}

	void SoftwareRenderer::render()
	{
		SDL_UpdateTexture(SDLTexturePtr, nullptr, pixels, sizeof(Uint32) * window->getWidth());
		SDL_RenderTexture(SDLRendererPtr, SDLTexturePtr, NULL, NULL);
		SDL_RenderPresent(SDLRendererPtr);
		//frameStats.updateFrameStats();
		showFrameStats();
	}

	void SoftwareRenderer::showFrameStats()
	{
		if (debugLevel == DebugLevel::TE_SHOW_FPS)
		{
			char buff[10];

			//snprintf(buff, 10, "%llu", frameStats.fps);
			renderText(buff, uiFont, Point2<int>(), 16);
		}
	}

	/* TODO: protections
	*/
	void SoftwareRenderer::renderText(const char* text, std::shared_ptr<Font> font,
		Point2<int> pos, int size)
	{
		if (!font)
		{
			TE_LOG(TE_RENDERING_LOG, TE_ERROR, "RenderText font in nullptr\n");
			return ;
		}
		std::shared_ptr<TTFFont> ttfFont = dynamic_pointer_cast<TTFFont>(font);
		if (!ttfFont)
		{
			ThrowException("Software renderer is trying to render a " + font->getAssetType()
				+ " instead of a TTF font");
		}

		SDL_Color color{ 255, 255, 255 };
		SDL_Surface* surf = TTF_RenderText_Blended(ttfFont->getPtr(), text, std::strlen(text), color);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(SDLRendererPtr, surf);
		auto texW = 0.f;
		auto texH = 0.f;
		SDL_GetTextureSize(texture, &texW, &texH);
		SDL_FRect rec{
			(w - texW - 10),
			(h - texH - 10),
			(texW),
			(texH)
		};
		SDL_RenderTexture(SDLRendererPtr, texture, nullptr, &rec);
		SDL_DestroyTexture(texture);
		SDL_DestroySurface(surf);
	}

	std::shared_ptr<MeshInternal> SoftwareRenderer::createSolidMesh(const MeshGeometry& geometry)
	{
		return std::make_shared<SoftwareMesh>();
	}

	std::shared_ptr<MeshInternal> SoftwareRenderer::loadMesh(const std::string& path)
	{
		return std::make_shared<SoftwareMesh>();
	}

	std::shared_ptr<Texture> SoftwareRenderer::loadTexture(const std::string& path)
	{
		return std::make_shared<SoftwareTexture>(path);
	}


}