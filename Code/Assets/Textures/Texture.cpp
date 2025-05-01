#include "Texture.hpp"

#include <iostream>

namespace te
{

	Texture::Texture(const std::string& path, std::string&& textureType): Asset{path}, type{textureType}
	{

	}

	Texture::Texture(const std::string& path, unsigned char* pixels, unsigned int size, std::string&& textureType):
		Asset{path}, type{textureType}
	{
	}

	Texture::Texture(Texture&& ref): Asset{std::move(ref)}, type{std::move(ref.type)}
	{

	}

	Texture& Texture::operator=(Texture&& ref)
	{
		type = std::move(ref.type);
		return *this;
	}

	Texture::~Texture()
	{

	}

	const int Texture::getWidth() const
	{
		return w;
	}

	const int Texture::getHeight() const
	{
		return h;
	}

	const int Texture::getChannels() const
	{
		return nChannels;
	}

	const std::string Texture::getAssetType() const
	{
		return "Texture";
	}
}