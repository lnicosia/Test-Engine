#include "Font.hpp"

namespace te
{
	Font::Font(const std::string& path): Asset(path)
	{
	}

	Font::Font(Font&& ref) : Asset(std::move(ref))
	{

	}

	Font::~Font()
	{
	}
}