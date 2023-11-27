#ifndef _LOG_HPP_
# define _LOG_HPP_

#include "ANSI.hpp"

#include <iostream>
#include <vector>

static std::vector<const char*> catNames =
{
	"Log",
	"General",
	"Resource",
	"Rendering API",
	"Invalid"
};

static std::vector<const char*> verboseNames =
{
	"Fatal",
	"Error",
	"Warning",
	"Display",
	"Log",
	"Verbose",
	"VeryVerbose"
};

static std::vector<const char*> verboseColors =
{
	TE_CONSOLE_RED,
	TE_CONSOLE_RED,
	TE_CONSOLE_YELLOW,
	TE_CONSOLE_DARK_GRAY,
	TE_CONSOLE_DARK_GRAY,
	TE_CONSOLE_DARK_GRAY,
	TE_CONSOLE_DARK_GRAY
};

// TODO: Don't print verbose level below Display
// TODO: handle fatal errors

#define LOG(CATEGORY, VERBOSE_LEVEL, format, ...)	\
	fprintf(Logger::files[CATEGORY], "%s: %s: " format , catNames[CATEGORY], verboseNames[VERBOSE_LEVEL] __VA_OPT__(,) __VA_ARGS__); \
	\
	fprintf(Logger::files[TE_ALL_LOG], "%s: %s: " format, catNames[CATEGORY], verboseNames[VERBOSE_LEVEL] __VA_OPT__(,) __VA_ARGS__); \
	\
	VERBOSE_LEVEL < TE_LOG ? \
		(void)printf("%s%s: %s: " format TE_CONSOLE_RESET, verboseColors[VERBOSE_LEVEL], catNames[CATEGORY], verboseNames[VERBOSE_LEVEL] __VA_OPT__(,) __VA_ARGS__) \
		: (void)0 \

namespace te
{

	enum LogCategory
	{
		TE_ALL_LOG,
		TE_GENERAL_LOG,
		TE_RESOURCE_LOG,
		TE_RENDERING_LOG,
		TE_END_LOG
	};

	/*
	| Verbosity Level | Printed in Console? | Printed in Editor's Log? |                      Notes                       |
	|-----------------|---------------------|--------------------------|--------------------------------------------------|
	| Fatal           | Yes                 | N/A                      | Crashes the session, even if logging is disabled |
	| Error           | Yes                 | Yes                      | Log text is coloured red                         |
	| Warning         | Yes                 | Yes                      | Log text is coloured yellow                      |
	| Display         | Yes                 | Yes                      | Log text is coloured grey                        |
	| Log             | No                  | Yes                      | Log text is coloured grey                        |
	| Verbose         | No                  | No                       |                                                  |
	| VeryVerbose     | No                  | No                       |                                                  |
	*/

	enum LogLevel
	{
		TE_FATAL,
		TE_ERROR,
		TE_WARNING,
		TE_DISPLAY,
		TE_LOG,
		TE_VERBOSE,
		TE_VERYVERBOSE
	};
	class Logger
	{
	public:
		
		static void Init();
		static void Quit();

		static std::vector<FILE*> files;
		static std::string ROOT_DIR_PATH;

	private:
		Logger();
		~Logger();

		static void findRootDirPath();

		static bool initialized;

	};
}

#endif // _LOG_HPP_