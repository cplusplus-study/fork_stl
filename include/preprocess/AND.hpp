#ifndef __FORK_STL_AND_HPP__
#define __FORK_STL_AND_HPP__
/* ****************************************************
 * Logical AND 
 *
 * MY_AND(X, Y) => if X && Y then 1 else 0 fi
 *
 * Crate: 05/26/2014 
 *      : 逻辑与
 *
 * ****************************************************/
#
#include "./BOOL.h"
#
#
#define MY_AND(x, y) MY_AND_I(x, y)
#define MY_AND_I(x, y) MY_BIT_AND(MY_BOOL(x), MY_BOOL(y))
#
#define MY_BIT_AND(x, y) MY_BIT_AND_I(x, y)
#define MY_BIT_AND_I(x, y) MY_BIT_AND_ ## x ## y
#
#define MY_BIT_AND_00 0
#define MY_BIT_AND_01 0
#define MY_BIT_AND_10 0
#define MY_BIT_AND_11 1
#
#
#endif
