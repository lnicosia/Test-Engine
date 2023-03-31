#ifndef _ASSERT_HPP_
# define _ASSERT_HPP_

#include "Log.hpp"

// TODO

#define assert(X) X ? (void)0 : throw (std::runtime_error(""));

#define uninplemented() throw(std::runtime_error("Not implemented yet hehe"));

#endif // _ASSERT_HPP_