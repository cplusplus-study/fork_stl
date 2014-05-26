
#ifndef __FORK_STL_EQUQL_HPP__
#define __FORK_STL_EQUQL_HPP__
#include "NOT_EQUAL.h"
#include "BOOL.h"
#
#define MY_EQUAL(x, y) MY_EQUAL_I(x, y)
#define MY_EQUAL_I(x, y) MY_COMPL(MY_BOOL(MY_NOT_EQUAL(x, y)))
#
#
#endif
