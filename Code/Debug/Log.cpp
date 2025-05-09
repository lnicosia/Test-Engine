#include "Log.hpp"
#include "Assets/CheckFileType.hpp"
#include "Debug/Exception.hpp"
#include "Platform.hpp"

#ifdef TE_WINDOWS
# include <windows.h>
#else
# define __STDC_WANT_LIB_EXT1__ 1
# include <stdio.h>
#endif // TE_WINDOWS

namespace te
{

	bool Logger::initialized = false;
	std::vector<FILE*> Logger::files{};
	std::string Logger::ROOT_DIR_PATH;

	void Logger::Init()
	{
		static Logger instance{};
		
		if (initialized)
		{
			return;
		}

		static std::filesystem::path paths[] =
		{
			"Logs/Log.txt",
			"Logs/GeneralLog.txt",
			"Logs/ResourcesLog.txt",
			"Logs/Rendering.txt"
		};

		if (std::atexit(Quit) != 0)
		{
			ThrowException( "Failed to setup SDL cleanup function\n" );
		}

		findRootDirPath();

		std::filesystem::path folder(ROOT_DIR_PATH + "Logs");
		if (!std::filesystem::exists(folder))
		{
			if (!std::filesystem::create_directory(folder))
			{
				ThrowException("Could not create Logs directory\n");
			}
			printf("Created directory %s\n", folder.string().c_str());
		}
		else if (!std::filesystem::is_directory(folder))
		{
			ThrowException( "'Logs/' already exists and is not a directory\n" );
		}

		files.resize(TE_END_LOG - TE_ALL_LOG);
		for (int i = TE_ALL_LOG; i != TE_END_LOG; i++)
		{
			FILE* f = fopen((ROOT_DIR_PATH + paths[i].string()).c_str(), "w+");
			if (f == nullptr)
			{
				ThrowException("Could not open log file " + paths[i].string());
			}
			files[i] = f;
		}

		TE_LOG(TE_RESOURCE_LOG, TE_DISPLAY, "Project root directory = %s\n", ROOT_DIR_PATH.c_str());
		initialized = true;
	}

	void Logger::Quit()
	{
		if (!initialized)
		{
			return;
		}
		for (int i = TE_ALL_LOG; i != TE_END_LOG; i++)
		{
			fclose(files[i]);
		}
	}

	void Logger::findRootDirPath()
	{
#ifdef TE_WINDOWS
		char path[MAX_PATH] = { 0 };
		GetModuleFileNameA(nullptr, path, MAX_PATH);
		std::filesystem::path executablePath(path);
#else
		std::filesystem::path executablePath(std::filesystem::canonical("/proc/self/exe"));
#endif

		std::filesystem::path currentDirPath = executablePath.parent_path();
		bool found = false;
		bool binariesFound = false;
		bool shadersFound = false;
		bool resourcesFound = false;
		while (!found)
		{
			binariesFound = false;
			shadersFound = false;
			resourcesFound = false;
			for (const auto& it : std::filesystem::directory_iterator(currentDirPath))
			{
				if (it.path().filename() == "Binaries")
				{
					binariesFound = true;
				}
				else if (it.path().filename() == "Shaders")
				{
					shadersFound = true;
				}
				else if (it.path().filename() == "Resources")
				{
					resourcesFound = true;
				}
			}
			found = binariesFound && shadersFound && resourcesFound;
			if (!currentDirPath.has_relative_path() || found)
			{
				break;
			}
			currentDirPath = currentDirPath.parent_path();
		}
		if (!found)
		{
			//TE_LOG(TE_RESOURCE_LOG, TE_WARNING, "Could not found project root directory\n");
		}
		else
		{
			ROOT_DIR_PATH = currentDirPath.string() + (char)std::filesystem::path::preferred_separator;
		}
	}

	std::string& Logger::getRootDirPath()
	{
		return ROOT_DIR_PATH;
	}

	std::vector<FILE*>& Logger::getFiles()
	{
		return files;
	}
}