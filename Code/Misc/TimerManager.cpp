#include "TimerManager.hpp"

namespace te
{
	TimerManager::TimerManager()
	{
		lastFrameTimeNano = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
	}

	const TimerManager& TimerManager::getInstance() const
	{
		return *this;
	}

	void TimerManager::update()
	{
		uint64_t now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
		deltaTimeNano = now - lastFrameTimeNano;
		lastFrameTimeNano = now;
		deltaTimeMili = static_cast<float>(deltaTimeNano) / 1000000.0f;
		deltaTimeSec = deltaTimeMili / 1000.0f;
	}

	float TimerManager::getDeltaTimeSec() const
	{
		return deltaTimeSec;
	}

	float TimerManager::getDeltaTimeMili() const
	{
		return deltaTimeMili;
	}

	uint64_t TimerManager::getDeltaTimeNano() const
	{
		return deltaTimeNano;
	}
}