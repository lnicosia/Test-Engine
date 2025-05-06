#include "Binding.hpp"

namespace te
{
	Binding::Binding(std::string name, uint32_t key1, uint32_t key2, bool editable) :
		onRelease(), onPress(), whenReleased(), whenPressed(), 
		key1(key1), key2(key2), state(InputState::TE_RELEASED),
		name(name)
	{

	}

	MouseBinding::MouseBinding(std::string name, uint32_t key1, uint32_t key2, bool editable) :
		Binding(name, key1, key2, editable), start()
	{

	}

	Binding::~Binding()
	{
	}

	//	Accessors

	std::string const	Binding::getName() const
	{
		return name;
	}

	uint32_t const	Binding::getKey1() const
	{
		return key1;
	}

	uint32_t const	Binding::getKey2() const
	{
		return key2;
	}

	InputState const	Binding::getState() const
	{
		return state;
	}

	//	Setters

	void	Binding::setKey1(uint32_t key)
	{
		this->key1 = key;
	}

	void	Binding::setKey2(uint32_t key)
	{
		this->key2 = key;
	}

	void	Binding::setState(InputState state)
	{
		this->state = state;
	}

	Binding& Binding::operator=(Binding const& ref)
	{
		return *this;
	}
}