#include "GLRenderer.hpp"

namespace te
{
	GLRenderer::GLRenderer(): Renderer(RendererType::TE_GL, WindowManager::TE_SDL)
	{
		// TODO
		throw std::runtime_error("OpenGL is not implemented yet");

	}

	GLRenderer::GLRenderer(WindowManager wManage): Renderer(RendererType::TE_GL, wManager)
	{
		// TODO
		throw std::runtime_error("OpenGL is not implemented yet");
		if (wManager != WindowManager::TE_SDL)
			throw std::runtime_error("Only SDL is implemented for now");
	}

	GLRenderer::~GLRenderer()
	{
		// TODO
	}

	void GLRenderer::render()
	{
		// TODO
	}

	void GLRenderer::renderText(const char* text, std::shared_ptr<Font> font, Point2<int> pos, int size)
	{
		// TODO
	}
}