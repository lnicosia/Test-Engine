#include "Shader.hpp"
#include "Assets/CheckFileType.hpp"
#include "Debug/Log.hpp"

#include <fstream>

namespace te
{
	Shader::Shader(const std::filesystem::path& path)
	{
		if (!IsReg(path))
		{
			TE_LOG(TE_RENDERING_LOG, TE_ERROR, "Invalid shader path: \"%s\"\n", path.string().c_str());
			return;
		}

		std::ifstream file;
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		file.open(path, std::ios::ate | std::ios::binary);
		if (!file.is_open())
		{
			TE_LOG(TE_RENDERING_LOG, TE_ERROR, "Could not open shader \"%s\"\n", path.string().c_str());
			return;
		}
		size_t fileSize = static_cast<size_t>(file.tellg());
		buff.clear();
		buff.resize(fileSize);
		file.seekg(0);
		file.read(buff.data(), fileSize);
	}

	Shader::~Shader()
	{
	}

}