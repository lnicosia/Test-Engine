#include "FrameStats.hpp"

#include <chrono>

namespace te
{
	void	FrameStats::updateFrameStats()
	{
		static uint64_t lastUpdate = frameTime;

		using namespace std::chrono;
		uint64_t newTime = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
		uint64_t frameDuration = newTime - frameTime;

		float currFps = 1000.0f / frameDuration;
		//printf("%f fps\n", fps);

		if (frameTime - lastUpdate >= 1000)
		{
			lastUpdate = newTime;
			fps = currFps;
		}
		frameTime = newTime;
	}
}