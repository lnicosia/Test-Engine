#ifndef _SDLEVENTS_CLASS_H_
#define _SDLEVENTS_CLASS_H_

#include "Binding.class.hpp"
#include "SDL.h"
#include "math.hpp"
#include <vector>

struct SDLEvents
{
public:
	SDLEvents();
	~SDLEvents();

	/**	Run the whole input pipeline: update inputs state and launch
	**	functions accordingly
	*/
	int
		handle();

	/**	Update the state of every input to one of the 5 possible values:
	**	NRE_PRESS, NRE_RELEASE, NRE_PRESSED, NRE_RELEASED, NRE_HOVERED
	*/
	int
		updateInputsState();

	/**	According to the state of each input, launch binded functions
	*/
	void
		processInputs();

	std::vector<MouseBinding> mouseBindings;

	Point2<float>	mousePos;
	Point2<float>	mouseGlobalPos;
	Point2<float>	mouseRelativePos;
	InputState	mouseState;

	std::vector<Binding>	bindings;

	SDL_Event	e;

};

#endif // !_SDLEVENTS_CLASS_H_