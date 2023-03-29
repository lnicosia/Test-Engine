#ifndef _SDL_HPP_
# define _SDL_HPP_

namespace te
{
	class SDL {

	public:
		static void Init( void );
		static void Quit( void );

	private:
		SDL( void ) {};
		static void SetupGLOptions( void );

		static bool initialized;
	};
}

#endif // _SDL_HPP_