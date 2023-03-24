
#include <iostream>
#include <exception>
#include <algorithm>
#include <chrono>
#define _USE_MATH_DEFINES
#include <math.h>

#include "SDL.h"
#include "SDL_ttf.h"

#include "Inputs/SDLEvents.hpp"
#include "Rendering/SoftwareRenderer.hpp"
#include "Rendering/SoftwareRendering/Raycaster.hpp"

# include "stb_image.h"


int main()
{
	te::SoftwareRenderer renderer;
	te::Raycaster	raycaster(&renderer);
	raycaster.render();
	return 0;
}