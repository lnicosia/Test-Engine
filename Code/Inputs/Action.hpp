#ifndef _ACTION_HPP_
# define _ACTION_HPP_

#include <functional>
#include <tuple>
#include <iostream>

/** Wrapper for any type of Action
*/
class ActionWrapper
{
public:
    ActionWrapper(){}
    virtual ~ActionWrapper(){}

    virtual void execute( void ) const {}

    ActionWrapper& operator =(const ActionWrapper& ref)
    {
        //*this = ref;
        return *this;
    }
};

template < typename ReturnType, typename ... Args>
class Action: public ActionWrapper
{
public:
    Action(): ActionWrapper(), func(), args()
    {}

    Action(std::function<ReturnType(Args...)> func, Args... args):
        ActionWrapper(), func(func), args(std::forward<Args>(args)...)
    {

    }

    ~Action() {}

    ReturnType execute( void ) const override
    {
        if (this->func)
        {
            return std::apply(this->func, this->args);
        }
    }

    operator bool() const
    {
        return this->func;
    }

    Action<ReturnType, Args...>& operator=(const Action<ReturnType, Args...>& ref)
    {
        this->func = ref.func;
        this->args = ref.args;
        return *this;
    }

private:
    std::function<ReturnType(Args...)> func;
    std::tuple<Args...> args;
};

#endif // _ACTION_HPP_