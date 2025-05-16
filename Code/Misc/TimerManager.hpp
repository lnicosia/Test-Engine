#ifndef _TIMER_MANAGER_HPP_
#define _TIMER_MANAGER_HPP_

#include <chrono>

namespace te
{
	class TimerManager
	{

		friend class Renderer;

		public:

			const TimerManager& getInstance() const;
			float getDeltaTimeSec() const;
			float getDeltaTimeMili() const;
			uint64_t getDeltaTimeNano() const;

		private:
			TimerManager();
			~TimerManager() = default;

			void update();

		private:

			uint64_t lastFrameTimeNano = 0;

			float deltaTimeSec = 0.0f;
			float deltaTimeMili = 0.0f;
			uint64_t deltaTimeNano = 0;
	};
}

#endif // _TIMER_MANAGER_HPP_