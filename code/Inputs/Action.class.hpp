#ifndef _ACTION_CLASS_H_
#define _ACTION_CLASS_H_

#include <functional>
#include <tuple>
#include <iostream>

/** Wrapper for any type of Action
*/
class ActionWrapper
{
public:
    ActionWrapper(){}
    ~ActionWrapper(){}

    virtual void execute( void ) const {}

    ActionWrapper& operator =(const ActionWrapper& ref)
    {
        *this = ref;
        return *this;
    }
};

template < typename ... Args>
class Action: public ActionWrapper
{
public:
    Action(): ActionWrapper(), func(), args()
    {}

    Action(std::function<void(Args...)> func, Args... args):
        ActionWrapper(), func(func), args(std::forward<Args>(args)...)
    {

    }

    ~Action() {}

    void execute( void ) const override
    {
        if (this->func)
            std::apply(this->func, this->args);
    }

    operator bool() const
    {
        return this->func;
    }

    Action<Args...>& operator=(const Action<Args...>& ref)
    {
        this->func = ref.func;
        this->args = ref.args;
        return *this;
    }

private:
    std::function<void(Args...)> func;
    std::tuple<Args...> args;
};

#endif // !_ACTION_CLASS_H_