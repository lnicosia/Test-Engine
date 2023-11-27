#include "Exception.hpp"
#include "Log.hpp"
#include "ANSI.hpp"

namespace te
{
	void ThrowException(const std::string& what)
	{
		// TODO: Custom Exception implementation (with crash report for example)
		// Log to every file?
		LOG(TE_GENERAL_LOG, TE_FATAL, "%s", what.c_str());
		throw (std::runtime_error(""));
	}
}