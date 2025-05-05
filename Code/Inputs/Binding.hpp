#ifndef _BINDING_HPP_
# define _BINDING_HPP_

#include "Action.hpp"
#include "Maths/math_tmp.hpp"

#include <string>
#include <vector>
#include <memory>

namespace te
{
	enum class InputState
	{
		TE_PRESS,
		TE_RELEASE,
		TE_PRESSED,
		TE_RELEASED,
		TE_HOVERED
	};

	class Binding
	{
	public:
		Binding(std::string name, uint32_t key1, uint32_t key2, bool editable);
		~Binding();

		//	Accessors

		std::string const	getName() const;
		uint32_t const	getKey1() const;
		uint32_t const	getKey2() const;
		InputState const	getState() const;

		void	setKey1(uint32_t key);
		void	setKey2(uint32_t key);
		void	setState(InputState state);

		//	Events
		//	Actions to perform in each event type
		std::shared_ptr<ActionWrapper>	onRelease;
		std::shared_ptr<ActionWrapper>	onPress;
		std::shared_ptr<ActionWrapper>	whenReleased;
		std::shared_ptr<ActionWrapper>	whenPressed;

		Binding& operator=(Binding const& ref);

	private:
		uint32_t	key1;
		uint32_t	key2;

		InputState	state;

		std::string const	name;

	};

	struct MouseBinding : public Binding
	{
		MouseBinding(std::string name, uint32_t key1, uint32_t key2, bool editable);
		Point2<float>	start;
	};
}

#endif // _BINDING_HPP_