#include "SoftwareTexture.hpp"
#include "Debug/Assert.hpp"

#define STB_IMAGE_IMPLEMENTATION
# include "stb_image.h"

#include <iostream>


namespace te
{
	SoftwareTexture::SoftwareTexture(const std::string& path): Texture(path), img(nullptr)
	{
		TE_LOG(TE_RESOURCE_LOG, TE_DISPLAY, "Loading software texture %s\n", path.c_str());
		this->img = stbi_load(path.c_str(), &this->w, &this->h, &this->nChannels, 0);
		if (!this->img)
		{
			std::cerr << std::endl << "Failed to load texture '" + path << " '" << std::endl;
			std::cerr << stbi_failure_reason() << std::endl;
			stbi_image_free(this->img);
			return ;
		}
	}

	SoftwareTexture::SoftwareTexture(SoftwareTexture&& ref): Texture(std::move(ref))
	{
		// TODO
		unimplemented();
	}

	void SoftwareTexture::setup(const std::string& path)
	{

	}

	void SoftwareTexture::cleanUp()
	{

	}

	SoftwareTexture& SoftwareTexture::operator=(SoftwareTexture&& ref)
	{
		return *this;
	}

	SoftwareTexture::~SoftwareTexture()
	{
		TE_LOG(TE_RESOURCE_LOG, TE_DISPLAY, "Destroying software texture %s\n", paths[0].string().c_str());
		if (this->img)
		{
			stbi_image_free(this->img);
		}
	}

	const unsigned char* SoftwareTexture::getImg() const
	{
		return img;
	}

	const std::string SoftwareTexture::getAssetType() const
	{
		return "Software texture";
	}
}