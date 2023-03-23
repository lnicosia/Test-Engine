#include "Events.hpp"

namespace te
{
	Events::Events(): bindings(), mouseBindings(),
		mousePos(), mouseGlobalPos(), mouseState(InputState::TE_RELEASED)
	{
	}

	Events::~Events()
	{
	}
}