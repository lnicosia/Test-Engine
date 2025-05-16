#ifndef _TIMER_MANAGER_HPP_
#define _TIMER_MANAGER_HPP_

#include "Inputs/Action.hpp"

#include <chrono>
#include <memory>

namespace te
{
	class TimerHandle
	{
		friend class TimerManager;

		TimerHandle(uint64_t startTime, float duration, bool isLooping,
			std::unique_ptr<ActionWrapper>&& action):
			startTime(startTime), duration(duration), isLooping(isLooping),
			action(std::move(action))
		{
		}

		uint64_t startTime = 0;
		/** In seconds */
		float duration = 0;
		bool isLooping = false;

		std::unique_ptr<ActionWrapper> action{};
	};

	class TimerManager
	{

		friend class Renderer;

		public:

			const TimerManager& getInstance() const;
			float getDeltaTimeSec() const;
			float getDeltaTimeMili() const;
			uint64_t getDeltaTimeNano() const;

			void setTimer(std::unique_ptr<ActionWrapper>&& action,
				float duration, bool isLooping = false);

		private:
			TimerManager();
			~TimerManager() = default;

			void update();
			bool checkTimer(TimerHandle& timer);

		private:

			/** Time in nanoseconds */
			uint64_t lastFrameTimeNano = 0;

			float deltaTimeSec = 0.0f;
			float deltaTimeMili = 0.0f;
			uint64_t deltaTimeNano = 0;

			size_t timersCount;
			std::vector<TimerHandle> timers{};
	};
}

#endif // _TIMER_MANAGER_HPP_