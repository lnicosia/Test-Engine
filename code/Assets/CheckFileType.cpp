#include "CheckFileType.hpp"
#include "Platform.hpp"

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
			std::cerr << std::endl << "te: " << path << " does not exist" << std::endl;
			return false;
		}
		if (!std::filesystem::is_regular_file(path))
		{
			std::cerr << std::endl << "te: " << path << " is not a regular file" << std::endl;
			return false;
		}
		return true;
	}
}