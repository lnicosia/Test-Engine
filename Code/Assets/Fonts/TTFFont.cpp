#include "TTFFont.hpp"
#include "Debug/Log.hpp"
#include "Debug/Exception.hpp"

namespace te
{

	TTFFont::TTFFont(const std::string& path): Font(path), ptr(nullptr)
	{
		TE_LOG(TE_RESOURCE_LOG, TE_DISPLAY, "Loading TTF font %s\n", path.c_str());
		ptr = TTF_OpenFont(path.c_str(), 12);
		if (!ptr)
			ThrowException( "Couldn't load " + path + "\n");
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
			TE_LOG(TE_RESOURCE_LOG, TE_DISPLAY, "Destroying TTF font %s\n", paths[0].string().c_str());
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