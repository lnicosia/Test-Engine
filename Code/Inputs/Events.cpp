#include "Events.hpp"

namespace te
{
	Events::Events(): mouseBindings(),
		mousePos(), mouseGlobalPos(), mouseRelativePos(), 
		mouseState(InputState::TE_RELEASED), bindings()
	{
	}

	Events::~Events()
	{
	}
}