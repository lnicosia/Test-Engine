#ifndef _PLATFORM_HPP_
# define _PLATFORM_HPP_

#	if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#		define TE_WINDOWS
# 		define _CRT_SECURE_NO_DEPRECATE
#	elif defined(__linux__)
#		define TE_UNIX
#	elif defined(__APPLE__)
#		define TE_APPLE
#	endif

#ifdef TE_WINDOWS
# ifdef TE_COMPILING_DLL
# define TE_DLL_EXPORT __declspec(dllexport)
# define TE_EXT_DLL
# else
# define TE_DLL_EXPORT __declspec(dllimport)
# define TE_EXT_DLL extern
# endif
#else
# define TE_DLL_EXPORT
#endif


#endif // _PLATFORM_HPP_