#ifndef _D3D_RENDERER_HPP_
# define _D3D_RENDERER_HPP_

#include "../Renderer.hpp"

namespace te
{
	class D3DRenderer: Renderer
	{
	public:
		explicit D3DRenderer();
		~D3DRenderer();

		void render() override;
		void renderText(const char* text, std::shared_ptr<Font> font,
			Point2<int> pos, int size) override;

	private:

	};

}

#endif // _D3D_RENDERER_HPP_