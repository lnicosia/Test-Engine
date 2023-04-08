#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include <filesystem>
#include <vector>

namespace te
{
	class Shader
	{
	public:
		Shader() = delete;
		Shader(const std::filesystem::path& path);
		~Shader();

		/* The type is char because of std::istream::read() which needs a char* pointer */
		std::vector<char> buff;
	private:

	};

}

#endif // _SHADER_HPP_