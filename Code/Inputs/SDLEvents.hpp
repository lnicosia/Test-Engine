#ifndef _SDLEVENTS_HPP_
# define _SDLEVENTS_HPP_

#include "SDL.h"
#include "Events.hpp"
#include "Maths/math_tmp.hpp"
#include <vector>

namespace te
{
	struct SDLEvents: public Events
	{
	public:
		SDLEvents();
		~SDLEvents();

		/**	Run the whole input pipeline: update inputs state and launch
		**	functions accordingly
		*/
		int handle() override;

		/**	Update the state of every input to one of the 5 possible values:
		**	NRE_PRESS, NRE_RELEASE, NRE_PRESSED, NRE_RELEASED, NRE_HOVERED
		*/
		int updateInputsState() override;

		/**	According to the state of each input, launch binded functions
		*/
		void processInputs() override;

		SDL_Event	e;

	};
}

#endif // _SDLEVENTS_HPP_