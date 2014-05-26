
#ifndef __FORK_STL_NOR_HPP__
#define __FORK_STL_NOR_HPP__

/* ****************************************************
 * Logical NOR
 *
 * MY_NOR(X, Y) => if X || Y then 0 else 1 fi
 *
 * Crate: 05/26/2014 
 *      : 逻辑NOR
 *
 * ****************************************************/

#include "./BOOL.h"
#
#define MY_NOR(x, y) MY_NOR_I(x, y)
#define MY_NOR_I(x, y) MY_BIT_NOR(MY_BOOL(x), MY_BOOL(y))
#
#define MY_BIT_NOR(x, y) MY_BIT_NOR_I(x, y)
#define MY_BIT_NOR_I(x, y) MY_BIT_NOR_ ## x ## y
#
#define MY_BIT_NOR_00 1
#define MY_BIT_NOR_01 0
#define MY_BIT_NOR_10 0
#define MY_BIT_NOR_11 0
#
#endif
