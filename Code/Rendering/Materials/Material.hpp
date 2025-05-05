#ifndef _MATERIAL_HPP_
#define _MATERIAL_HPP_

#include "Assets/Textures/Texture.hpp"

#include <stdint.h>

namespace te
{
	enum class MaterialPass : uint8_t
	{
		MainColor,
		Transparent,
		Other
	};
	
	struct Material
	{
		Material();
		Material(std::shared_ptr<Texture> texture);

		MaterialPass passType;

		std::shared_ptr<Texture> texture;

	};
} // namespace te

#endif // _MATERIAL_HPP_