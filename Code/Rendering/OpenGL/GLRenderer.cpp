#include "GLRenderer.hpp"
#include "Debug/Assert.hpp"

namespace te
{
	GLRenderer::GLRenderer(): Renderer(RendererType::TE_GL, WindowAPI::TE_SDL)
	{
		// TODO
		unimplemented();

	}

	GLRenderer::GLRenderer(WindowAPI windowAPI): Renderer(RendererType::TE_GL, windowAPI)
	{
		// TODO
		unimplemented();
		if (windowAPI != WindowAPI::TE_SDL)
		{
			ThrowException("Only SDL is implemented for now");
		}
	}

	GLRenderer::~GLRenderer()
	{
		// TODO
		unimplemented();
	}

	void GLRenderer::render()
	{
		// TODO
		unimplemented();
	}

	void GLRenderer::renderText(const char* text, std::shared_ptr<Font> font, Point2<int> pos, int size)
	{
		// TODO
		unimplemented();
	}
}