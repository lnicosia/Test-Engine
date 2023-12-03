#include "GLRenderer.hpp"
#include "Debug/Assert.hpp"

namespace te
{
	GLRenderer::GLRenderer(): Renderer(RendererType::TE_GL, WindowManager::TE_SDL)
	{
		// TODO
		uninplemented();

	}

	GLRenderer::GLRenderer(WindowManager wManage): Renderer(RendererType::TE_GL, wManager)
	{
		// TODO
		uninplemented();
		if (wManager != WindowManager::TE_SDL)
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