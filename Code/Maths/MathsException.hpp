#ifndef _MATHS_EXCEPTION_HPP_
#define _MATHS_EXCEPTION_HPP_

#include <stdexcept>

namespace sml
{
	class MathsException : public std::runtime_error
	{
	public:
		explicit MathsException(const std::string& what_arg): std::runtime_error(what_arg) {}
	};
} // namespace sml


#endif // _MATHS_EXCEPTION_HPP_