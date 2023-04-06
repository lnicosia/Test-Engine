#ifndef _PLATFORM_HPP_
# define _PLATFORM_HPP_

#	if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#		define TE_WINDOWS
#	elif defined(__linux__)
#		define TE_UNIX
#	elif defined(__APPLE__)
#		define TE_APPLE
#	endif

#endif // _PLATFORM_HPP_