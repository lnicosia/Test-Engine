#ifndef _GL_RENDERER_HPP_
# define _GL_RENDERER_HPP_

#include "Renderer.hpp"

namespace te
{
	class GLRenderer: public Renderer
	{
	public:
		GLRenderer();
		GLRenderer(WindowManager wManager);
		~GLRenderer();

		void render() override;
		void renderText(const char* text, std::shared_ptr<Font> font,
			Point2<int> pos, int size) override;

	private:

	};
}

#endif // _GL_RENDERER_HPP_