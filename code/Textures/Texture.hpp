#ifndef _TEXTURE_HPP_
# define _TEXTURE_HPP_

#include <string>

namespace te
{
	class Texture
	{
	private:
		unsigned char* img;
		int w;
		int h;
		int nChannels;
		bool loaded;

	public:
		Texture() = delete;
		Texture(const std::string& path);
		~Texture();

		const int getWidth() const;
		const int getHeight() const;
		const int getChannels() const;
		const unsigned char* getImg() const;
		const bool isLoaded() const;
		void unload();

	};
}

#endif // _TEXTURE_HPP_