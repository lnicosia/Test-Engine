#include "TimerManager.hpp"

#include <set>

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

		std::set<size_t> toDelete;
		for (int64_t i = timers.size() - 1; i >= 0; i--)
		{
			if (checkTimer(timers[i]))
			{
				toDelete.insert(i);
			}
		}
		for (auto it : toDelete)
		{
			timers.erase(timers.begin() + it);
		}
	}

	void TimerManager::setTimer(std::unique_ptr<ActionWrapper>&& action,
		float duration, bool isLooping)
	{
		timers.push_back(TimerHandle(lastFrameTimeNano, duration, isLooping, std::move(action)));
	}

	bool TimerManager::checkTimer(TimerHandle& timer)
	{
		if (static_cast<float>(lastFrameTimeNano - timer.startTime) / 1000000000.f >= timer.duration)
		{
			if (timer.action)
			{
				timer.action->execute();
			}
			if (timer.isLooping)
			{
				timer.startTime = lastFrameTimeNano;
			}
			else
			{
				return true;
			}
		}
		return false;
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