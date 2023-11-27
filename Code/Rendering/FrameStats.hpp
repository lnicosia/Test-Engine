#ifndef _FRAME_DATA_HPP_
# define _FRAME_DATA_HPP_

#include "DebugLevel.hpp"
#include <stdint.h>

namespace te
{
	struct FrameStats
	{
		uint64_t	frameTime;
		uint64_t	fps;
		uint64_t	uiFrameTime;

		void	updateFrameStats();
	};
}

#endif // _FRAME_DATA_HPP_