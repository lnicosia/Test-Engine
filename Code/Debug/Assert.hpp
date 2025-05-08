#ifndef _ASSERT_HPP_
# define _ASSERT_HPP_

#include "Exception.hpp"
#include "Log.hpp"

#define unimplemented() \
	ThrowException(__FILE__ ":" + std::to_string(__LINE__) + ": " + __func__ + "() is not implemented yet hehe"); \

#if defined(NDEBUG)
#define TE_ASSERT(X) (void)0
#else
#define TE_ASSERT(X) X ? (void)0 : ThrowException(__FILE__ ":" + std::to_string(__LINE__) + ": " + #X + " failed");
#endif

#endif // _ASSERT_HPP_