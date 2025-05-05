#ifndef _TEXTURE_HPP_
# define _TEXTURE_HPP_

#include "Assets/Asset.hpp"
#include <string>

namespace te
{
	class Texture: public Asset
	{
	protected:
		int w;
		int h;
		int nChannels;

	public:
		Texture() = delete;
		Texture(const std::string& path, std::string&& textureType = "Undefined");
		Texture(const std::string& path, unsigned char* pixels, unsigned int size, std::string&& textureType);
		Texture(const Texture& ref) = delete;
		Texture(Texture&& ref);
		Texture& operator=(Texture&& ref);
		Texture& operator=(const Texture& ref) = delete;
		virtual ~Texture();

		virtual void cleanUp() = 0;

		const int getWidth() const;
		const int getHeight() const;
		const int getChannels() const;

	protected:
		const std::string getAssetType() const override;
		virtual void setup(const std::string& path) = 0;

	protected:
		/** Texture type, ie diffuse, specular, etc */
		std::string type{"Undefined"};
	};
}

#endif // _TEXTURE_HPP_