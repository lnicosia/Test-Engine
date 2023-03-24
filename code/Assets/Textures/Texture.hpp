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
		Texture(const std::string& path);
		Texture(const Texture& ref) = delete;
		Texture(Texture&& ref);
		Texture& operator=(Texture&& ref);
		Texture& operator=(const Texture& ref) = delete;
		~Texture();

		const int getWidth() const;
		const int getHeight() const;
		const int getChannels() const;

	};
}

#endif // _TEXTURE_HPP_