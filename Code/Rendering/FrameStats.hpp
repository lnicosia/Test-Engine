#ifndef _FRAME_DATA_HPP_
# define _FRAME_DATA_HPP_

#include "DebugLevel.hpp"
#include <stdint.h>
#include <cstddef>

namespace te
{
	struct FrameStats
	{
		uint64_t	frameTime = 0;
		uint64_t	fps = 0;
		uint64_t	uiFrameTime = 0;
		size_t		drawCallCount = 0;

		void	updateFrameStats();
	};
}

#endif // _FRAME_DATA_HPP_