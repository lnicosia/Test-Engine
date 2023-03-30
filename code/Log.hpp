#ifndef _LOG_HPP_
# define _LOG_HPP_

#include <iostream>
#include <vector>

static std::vector<const char*> catNames =
{
	"Log",
	"General",
	"Resource",
	"Invalid"
};

// TODO: Not thread safe!!
// TODO: can be improved -> single fprintf and printf
// TODO: Either print LogLevel or change color according to it

#define LOG(CATEGORY, ...)	fprintf(Logger::files[CATEGORY], "%s: ", catNames[CATEGORY]); \
							fprintf(Logger::files[CATEGORY], __VA_ARGS__ ); \
							fprintf(Logger::files[TE_ALL_LOG], "%s: ", catNames[CATEGORY]); \
							fprintf(Logger::files[TE_ALL_LOG], __VA_ARGS__ ); \
							printf("%s: ", catNames[CATEGORY]); printf(__VA_ARGS__);\


namespace te
{
	enum LogCategory
	{
		TE_ALL_LOG,
		TE_GENERAL_LOG,
		TE_RESOURCE_LOG,
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

	enum class LogLevel
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

	private:
		Logger();
		~Logger();

		static bool initialized;

	};
}

#endif // _LOG_HPP_