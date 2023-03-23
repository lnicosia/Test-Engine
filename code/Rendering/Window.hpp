#ifndef _WINDOW_HPP_
# define _WINDOW_HPP_

#include "Maths/math.hpp"

namespace te
{
	class Window
	{
	public:
		Window();
		Window(int w, int h);
		~Window();
		const int getWidth() const;
		const int getHeight() const;

	private:
		bool	resizable;
		double	ratio;
	protected:
		int w = 1600;
		int h = 900;
	};


}

#endif // _WINDOW_HPP_