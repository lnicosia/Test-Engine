#include "SDL.hpp"
#include "Log.hpp"

#include "SDL.h"
#include "SDL_ttf.h"

#include <string>
#include <stdexcept>

namespace te
{
	void SDL::Init( void )
	{
		if (!initialized) {
			LOG("Initializing SDL");
			if (SDL_InitSubSystem(SDL_INIT_EVERYTHING) != 0) {
				throw std::runtime_error( "Couldn't initialize SDL : \n" + std::string(SDL_GetError()) );
			}

			LOG("Initializing SDL_ttf");
			if (TTF_Init() == -1)
				throw std::runtime_error( "Couldn't initialize SDL_ttf : \n" + std::string(TTF_GetError()));

			if (std::atexit(Quit) != 0)
			{
				LOG("Quitting SDL TTF");
				TTF_Quit();
				LOG("Quitting SDL subsystems");
				SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
				LOG("Quitting SDL");
				SDL_Quit();
				throw std::runtime_error( "Failed to setup SDL cleanup function\n" );
			}

			initialized = true;
		}
	}

	void SDL::SetupGLOptions()
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	}

	void SDL::Quit( void )
	{
		if (initialized) {
			LOG("Quitting SDL TTF");
			TTF_Quit();
			/* LEAK ?? */
			LOG("Quitting SDL subsystems");
			SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
			LOG("Quitting SDL");
			SDL_Quit();
			initialized = false;
		}
	}

	bool SDL::initialized = false;
}