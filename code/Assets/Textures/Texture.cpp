#include "Texture.hpp"

#include <iostream>

namespace te
{

	Texture::Texture(const std::string& path): Asset(path)
	{

	}

	Texture::Texture(Texture&& ref): Asset(std::move(ref))
	{

	}

	Texture& Texture::operator=(Texture&& ref)
	{
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
}