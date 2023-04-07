#ifndef _CHECK_FILE_TYPE_HPP_
#define _CHECK_FILE_TYPE_HPP_

#include <string>
#include <filesystem>

namespace te
{
	bool
		IsReg(const std::string& path);

	bool
		IsReg(const std::filesystem::path& path);
}

#endif // _CHECK_FILE_TYPE_HPP_