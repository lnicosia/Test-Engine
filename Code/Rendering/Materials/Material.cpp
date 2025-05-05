#include "Material.hpp"

#include "Assets/AssetManager.hpp"

namespace te
{
	Material::Material()
	{
		texture = AssetManager::getInstance().getDefaultTexture();
	}

	Material::Material(std::shared_ptr<Texture> texture): texture{texture}
	{
	}

} // namespace te