#ifndef _ASSERT_HPP_
# define _ASSERT_HPP_

#include "Exception.hpp"
#include "Log.hpp"

// TODO

#define assert(X) X ? (void)0 : throw (std::runtime_error(""));

#define uninplemented() \
	ThrowException(__FILE__ ":" + std::to_string(__LINE__) + ": " + __func__ + "() is not implemented yet hehe"); \

#endif // _ASSERT_HPP_