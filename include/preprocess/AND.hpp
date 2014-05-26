#ifndef __FORK_STL_AND_HPP__
#define __FORK_STL_AND_HPP__
/* ****************************************************
 * Logical AND 
 *
 * NULL_PP_AND(X, Y) => if X && Y then 1 else 0 fi
 *
 * Crate: 05/26/2014 
 *      : 逻辑与
 *
 * ****************************************************/
#
#include "./BOOL.h"
#
#
#define NULL_PP_AND(x, y) MY_AND_I(x, y)
#define NULL_PP_AND_I(x, y) MY_BIT_AND(MY_BOOL(x), MY_BOOL(y))
#
#define NULL_PP_BIT_AND(x, y) MY_BIT_AND_I(x, y)
#define NULL_PP_BIT_AND_I(x, y) MY_BIT_AND_ ## x ## y
#
#define NULL_PP_BIT_AND_00 0
#define NULL_PP_BIT_AND_01 0
#define NULL_PP_BIT_AND_10 0
#define NULL_PP_BIT_AND_11 1
#
#
#endif
