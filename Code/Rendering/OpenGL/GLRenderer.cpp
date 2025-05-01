#include "GLRenderer.hpp"
#include "Debug/Assert.hpp"

namespace te
{
	GLRenderer::GLRenderer(): Renderer(RendererType::TE_GL, WindowAPI::TE_SDL)
	{
		// TODO
		uninplemented();

	}

	GLRenderer::GLRenderer(WindowAPI windowAPI): Renderer(RendererType::TE_GL, windowAPI)
	{
		// TODO
		uninplemented();
		if (windowAPI != WindowAPI::TE_SDL)
		{
			ThrowException("Only SDL is implemented for now");
		}
	}

	GLRenderer::~GLRenderer()
	{
		// TODO
		uninplemented();
	}

	void GLRenderer::render()
	{
		// TODO
		uninplemented();
	}

	void GLRenderer::renderText(const char* text, std::shared_ptr<Font> font, Point2<int> pos, int size)
	{
		// TODO
		uninplemented();
	}
}