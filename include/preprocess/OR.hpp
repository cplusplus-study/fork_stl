
#ifndef __FORK_STL_OR_HPP__
#define __FORK_STL_OR_HPP__

/* ****************************************************
 * Logical OR
 *
 * NULL_PP_OR(X, Y) => if X || Y then 1 else 0 fi
 *
 * Crate: 05/26/2014 
 *      : 逻辑或
 *
 * ****************************************************/

#include "./BOOL.h"

#
#
#define NULL_PP_OR(x, y) MY_OR_I(x, y)
#define NULL_PP_OR_I(x, y) MY_BIT_OR(MY_BOOL(x), MY_BOOL(y))
#
#
#define NULL_PP_BIT_OR(x, y) MY_BIT_OR_I(x, y)
#define NULL_PP_BIT_OR_I(x, y) MY_BIT_ ## x ## y
#
#define NULL_PP_BIT_00 0
#define NULL_PP_BIT_01 1
#define NULL_PP_BIT_10 1
#define NULL_PP_BIT_11 1
#
#endif
