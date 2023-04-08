#include "CheckFileType.hpp"
#include "Platform.hpp"
#include "Debug/Log.hpp"

#ifndef TE_WINDOWS
# include <unistd.h>
# include <sys/stat.h>
#endif

#include <iostream>

namespace te
{
	bool	IsReg(const std::string& path)
	{
		return IsReg(std::filesystem::path(path));
	}

	bool	IsReg(const std::filesystem::path& path)
	{
		if (!std::filesystem::exists(path))
		{
			LOG(TE_RESOURCE_LOG, TE_ERROR, "\"%s\" does not exist\n", path.string().c_str());
			return false;
		}
		if (!std::filesystem::is_regular_file(path))
		{
			LOG(TE_RESOURCE_LOG, TE_ERROR, "\"%s\" is not a regular file\n", path.string().c_str());
			return false;
		}
		return true;
	}
}