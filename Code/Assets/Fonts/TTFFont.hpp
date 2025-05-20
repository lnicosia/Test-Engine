#ifndef _TTF_FONT_HPP_
# define _TTF_FONT_HPP_

#include "SDL3_ttf/SDL_ttf.h"

#include "Assets/Fonts/Font.hpp"

namespace te
{
	class TTFFont: public Font
	{
	public:
		TTFFont() = delete;
		TTFFont(const std::string& path);
		TTFFont(TTFFont&& ref);
		TTFFont(const TTFFont& ref) = delete;
		TTFFont& operator=(TTFFont&& ref);
		TTFFont& operator=(const TTFFont& ref) = delete;
		~TTFFont();

		const std::string getAssetType() const override;
		TTF_Font* getPtr() const;

	private:
		TTF_Font* ptr;
	};

}

#endif // _TTF_FONT_HPP_