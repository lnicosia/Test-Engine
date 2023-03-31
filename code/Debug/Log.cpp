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

	bool Logger::initialized = false;
	std::vector<FILE*> Logger::files;
}