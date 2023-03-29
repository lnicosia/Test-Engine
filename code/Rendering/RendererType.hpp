#ifndef _RENDERER_TYPE_HPP_
# define _RENDERER_TYPE_HPP_

namespace te
{
	enum class RendererType
	{
		TE_SOFTWARE,
		TE_GL,
		TE_VULKAN,
		TE_D3D,
		TE_METAL,
		TE_UNKNOWN_RENDERER
	};
}

#endif // _RENDERER_TYPE_HPP_