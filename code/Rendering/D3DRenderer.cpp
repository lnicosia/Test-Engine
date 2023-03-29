#include "D3DRenderer.hpp"

namespace te
{
	D3DRenderer::D3DRenderer(): Renderer(RendererType::TE_D3D, WindowManager::TE_UNKNOWN_WINDOW)
	{
		// TODO
		throw std::runtime_error("Dirext is not implemented yet");
	}

	D3DRenderer::~D3DRenderer()
	{
		// TODO
		throw std::runtime_error("Dirext is not implemented yet");
	}

	void D3DRenderer::render()
	{
		// TODO
	}

	void D3DRenderer::renderText(const char* text, std::shared_ptr<Font> font, Point2<int> pos, int size)
	{
		// TODO
	}
}