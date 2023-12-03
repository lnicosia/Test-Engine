#include "SDLEvents.hpp"
#include <iostream>

namespace te
{
	SDLEvents::SDLEvents() : e()
	{

	}

	SDLEvents::~SDLEvents()
	{
	}

	void	SDLEvents::processInputs()
	{
		for (auto& binding : this->bindings)
		{
			switch (binding.getState())
			{
			case InputState::TE_PRESS:
			{
				if (binding.onPress)
				{
					binding.onPress->execute();
				}
				binding.setState(InputState::TE_PRESSED);
				break;
			}
			case InputState::TE_RELEASE:
			{
				if (binding.onRelease)
				{
					binding.onRelease->execute();
				}
				binding.setState(InputState::TE_RELEASED);
				break;
			}
			case InputState::TE_PRESSED:
			{
				if (binding.whenPressed)
				{
					binding.whenPressed->execute();
				}
				break;
			}
			case InputState::TE_RELEASED:
			{
				if (binding.whenReleased)
				{
					binding.whenReleased->execute();
				}
				break;
			}
			default:
				break;
			}
		}
		for (auto& mouseBinding : this->mouseBindings)
		{
			switch (mouseBinding.getState())
			{
			case InputState::TE_PRESS:
			{
				if (mouseBinding.onPress)
				{
					mouseBinding.onPress->execute();
				}
				mouseBinding.setState(InputState::TE_PRESSED);
				break;
			}
			case InputState::TE_RELEASE:
			{
				if (mouseBinding.onRelease)
				{
					mouseBinding.onRelease->execute();
				}
				mouseBinding.setState(InputState::TE_RELEASED);
				break;
			}
			case InputState::TE_PRESSED:
			{
				if (mouseBinding.whenPressed)
				{
					mouseBinding.whenPressed->execute();
				}
				break;
			}
			case InputState::TE_RELEASED:
			{
				if (mouseBinding.whenReleased)
				{
					mouseBinding.whenReleased->execute();
				}
				break;
			}
			default:
				break;
			}
		}
	}

	int	SDLEvents::updateInputsState()
	{
		if (mouseState == InputState::TE_PRESS)
			mouseState = InputState::TE_PRESSED;
		if (mouseState == InputState::TE_RELEASE)
			mouseState = InputState::TE_RELEASED;
		SDL_GetMouseState(&mousePos.x, &mousePos.y);
		SDL_GetGlobalMouseState(&mouseGlobalPos.x, &mouseGlobalPos.y);
		SDL_GetRelativeMouseState(&mouseRelativePos.x, &mouseRelativePos.y);
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_EVENT_QUIT:
			{
				return 1;
			}
			case SDL_EVENT_KEY_DOWN:
			{
				for (auto& binding : this->bindings)
				{
					if (e.key.keysym.sym == binding.getKey1()
						|| e.key.keysym.sym == binding.getKey2())
					{
						if (binding.getState() == InputState::TE_RELEASED)
						{
							binding.setState(InputState::TE_PRESS);
						}
					}
				}
				break;
			}
			case SDL_EVENT_KEY_UP:
			{
				for (auto& binding : this->bindings)
				{
					if (e.key.keysym.sym == binding.getKey1()
						|| e.key.keysym.sym == binding.getKey2())
					{
						binding.setState(InputState::TE_RELEASE);
					}
				}
				break;
			}
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
			{
				for (auto& mouseBinding : this->mouseBindings)
				{
					if (e.button.button == mouseBinding.getKey1()
						|| e.button.button == mouseBinding.getKey2())
					{
						if (mouseBinding.getState() == InputState::TE_RELEASED)
						{
							mouseBinding.setState(InputState::TE_PRESS);
							mouseBinding.start = this->mousePos;
						}
					}
				}
				if (e.button.button == SDL_BUTTON_LEFT
					&& mouseState == InputState::TE_RELEASED)
				{
					mouseState = InputState::TE_PRESS;
				}
				break;
			}
			case SDL_EVENT_MOUSE_BUTTON_UP:
			{
				for (auto& mouseBinding : this->mouseBindings)
				{
					if (e.button.button == mouseBinding.getKey1()
						|| e.button.button == mouseBinding.getKey2())
					{
						if (mouseBinding.getState() == InputState::TE_PRESSED)
						{
							mouseBinding.setState(InputState::TE_RELEASE);
						}
					}
				}
				if (e.button.button == SDL_BUTTON_LEFT
					&& mouseState == InputState::TE_PRESSED)
				{
					mouseState = InputState::TE_RELEASE;
				}
				break;
			}
			default:
				break;
			}
		}
		return (0);
	}

	int	SDLEvents::handle()
	{
		if (updateInputsState() == 1)
		{
			return 1;
		}
		processInputs();
		return 0;
	}

}