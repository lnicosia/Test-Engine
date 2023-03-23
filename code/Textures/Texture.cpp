#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
# include "stb_image.h"

#include <iostream>

namespace te
{
	Texture::Texture(const std::string& path): img(0), w(0), h(0), nChannels(0), loaded(false)
	{
		img = stbi_load(path.c_str(),
			&w, &h, &nChannels, 0);
		if (!img)
		{
			std::cerr << std::endl << "Failed to load texture '" + path << " '" << std::endl;
			std::cerr << stbi_failure_reason() << std::endl;
			stbi_image_free(img);
			return ;
		}
		loaded = true;
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

	const unsigned char* Texture::getImg() const
	{
		return img;
	}

	const bool Texture::isLoaded() const
	{
		return loaded;
	}

	void Texture::unload()
	{
		stbi_image_free(img);
		this->loaded = false;
	}
}