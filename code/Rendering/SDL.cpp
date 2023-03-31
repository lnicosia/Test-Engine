#include "SDL.hpp"
#include "Debug/Log.hpp"

#include "SDL.h"
#include "SDL_ttf.h"

#include <string>
#include <stdexcept>

namespace te
{
	void SDL::Init( void )
	{
		if (initialized)
			return;
		LOG(TE_RENDERING_LOG, TE_LOG, "Initializing SDL\n");
		if (SDL_InitSubSystem(SDL_INIT_EVERYTHING) != 0) {
			throw std::runtime_error( "Couldn't initialize SDL : \n" + std::string(SDL_GetError()) );
		}

		LOG(TE_RENDERING_LOG, TE_LOG, "Initializing SDL_ttf\n");
		if (TTF_Init() == -1)
			throw std::runtime_error( "Couldn't initialize SDL_ttf : \n" + std::string(TTF_GetError()));

		if (std::atexit(Quit) != 0)
		{
			LOG(TE_RENDERING_LOG, TE_LOG, "Quitting SDL TTF\n");
			TTF_Quit();
			LOG(TE_RENDERING_LOG, TE_LOG, "Quitting SDL subsystems\n");
			SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
			LOG(TE_RENDERING_LOG, TE_LOG, "Quitting SDL\n");
			SDL_Quit();
			throw std::runtime_error( "Failed to setup SDL cleanup function\n" );
		}

		initialized = true;
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
			LOG(TE_RENDERING_LOG, TE_LOG, "Quitting SDL TTF\n");
			TTF_Quit();
			/* LEAK ?? */
			LOG(TE_RENDERING_LOG, TE_LOG, "Quitting SDL subsystems\n");
			SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
			LOG(TE_RENDERING_LOG, TE_LOG, "Quitting SDL\n");
			SDL_Quit();
			initialized = false;
		}
	}

	bool SDL::initialized = false;
}