
#ifndef __FORK_STL_OVERLOAD_HPP__
#define __FORK_STL_OVERLOAD_HPP__
#include "CAT.h"

#define MY_OVERLOAD(prefix,...) MY_CAT(prefix,MY_VARIADICS_SIZE(__VA_ARGS__))

#endif
