#include "Log.hpp"
#include "Assets/CheckFileType.hpp"
#include "Debug/Exception.hpp"

namespace te
{

	void Logger::Init()
	{
		if (initialized)
			return;

		static std::filesystem::path paths[] =
		{
			"Logs/Log.txt",
			"Logs/GeneralLog.txt",
			"Logs/ResourcesLog.txt",
			"Logs/Rendering.txt"
		};

		std::filesystem::path folder("Logs");
		if (!std::filesystem::exists(folder))
		{
			std::filesystem::create_directory(folder);
		}
		else if (!std::filesystem::is_directory(folder))
			ThrowException( "'Logs/' already exists and is not a directory\n" );

		for (int i = TE_ALL_LOG; i != TE_END_LOG; i++)
		{
			FILE* f = fopen(paths[i].string().c_str(), "w+");
			if (f == nullptr)
				ThrowException("Could not open log file " + paths[i].string());
			files.push_back(f);
		}
		if (std::atexit(Quit) != 0)
		{
			ThrowException( "Failed to setup SDL cleanup function\n" );
		}

		findRootDirPath();

		initialized = true;
	}

	void Logger::Quit()
	{
		if (!initialized)
			return;
		for (int i = TE_ALL_LOG; i != TE_END_LOG; i++)
		{
			fclose(files[i]);
		}
	}

	void Logger::findRootDirPath()
	{
		std::filesystem::path curr(std::filesystem::current_path());
		
		LOG(TE_RESOURCE_LOG, TE_DISPLAY, "Executable directory = %s\n", curr.string().c_str());
		bool found = false;
		bool binariesFound = false;
		bool shadersFound = false;
		bool resourcesFound = false;
		while (!found)
		{
			binariesFound = false;
			shadersFound = false;
			resourcesFound = false;
			for (const auto& it : std::filesystem::directory_iterator(curr))
			{
				if (it.path().filename() == "Binaries")
					binariesFound = true;
				else if (it.path().filename() == "Shaders")
					shadersFound = true;
				else if (it.path().filename() == "Resources")
					resourcesFound = true;
			}
			found = binariesFound && shadersFound && resourcesFound;
			if (!curr.has_relative_path() || found)
				break;
			curr = curr.parent_path();
		}
		if (!found)
		{
			LOG(TE_RESOURCE_LOG, TE_WARNING, "Could not found project root directory\n");
		}
		else
		{
			LOG(TE_RESOURCE_LOG, TE_DISPLAY, "Project root directory = %s\n", curr.string().c_str());
			ROOT_DIR_PATH = curr.string() + (char)std::filesystem::path::preferred_separator;
		}
	}

	bool Logger::initialized = false;
	std::vector<FILE*> Logger::files;
	std::string Logger::ROOT_DIR_PATH;
}