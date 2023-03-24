#include "TTFFont.hpp"
#include "Log.hpp"

namespace te
{

	TTFFont::TTFFont(const std::string& path): Font(path), ptr(nullptr)
	{
		LOG("Loading TTF font " + path);
		ptr = TTF_OpenFont(path.c_str(), 12);
		if (!ptr)
			throw std::runtime_error( "Couldn't load " + path + "\n");
	}

	TTFFont::TTFFont(TTFFont&& ref): Font(std::move(ref))
	{
	}

	TTFFont& TTFFont::operator=(TTFFont&& ref)
	{
		return *this;
	}

	TTFFont::~TTFFont()
	{
		if (this->ptr)
		{
			LOG("Destroying TTF font" + paths[0].string());
			TTF_CloseFont(this->ptr);
		}
	}

	TTF_Font* TTFFont::getPtr() const
	{
		return ptr;
	}

	const std::string TTFFont::getAssetType() const
	{
		return "TTF Font";
	}
}