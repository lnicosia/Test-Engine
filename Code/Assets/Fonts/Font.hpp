#ifndef _FONT_HPP_
# define _FONT_HPP_

#include "Assets/Asset.hpp"

namespace te
{
	class Font: public Asset
	{
	public:
		Font() = delete;
		Font(const std::string& path);
		Font(const Font& ref) = delete;
		Font(Font&& ref);
		Font& operator=(const Font& ref) = delete;
		Font& operator=(Font&& ref);
		~Font();

	private:

	};
}

#endif // _FONT_HPP_