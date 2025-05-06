#ifndef _EVENTS_HPP_
# define _EVENTS_HPP_

#include "Binding.hpp"

namespace te
{
	struct Events
	{
	public:
		Events();
		virtual ~Events();
		/**	Run the whole input pipeline: update inputs state and launch
		**	functions accordingly
		*/
		virtual int handle() = 0;

		/**	Update the state of every input to one of the 5 possible values:
		**	NRE_PRESS, NRE_RELEASE, NRE_PRESSED, NRE_RELEASED, NRE_HOVERED
		*/
		virtual int updateInputsState() = 0;

		/**	According to the state of each input, launch binded functions
		*/
		virtual void processInputs() = 0;

		std::vector<MouseBinding> mouseBindings;

		sml::vec2 mousePos;
		sml::vec2 mouseGlobalPos;
		sml::vec2 mouseRelativePos;
		InputState mouseState;

		std::vector<Binding> bindings;

	};

}

#endif // _EVENTS_HPP_