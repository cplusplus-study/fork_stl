
#ifndef __FORK_STL_XOR_HPP__
#define __FORK_STL_XOR_HPP__
/* ****************************************************
 * Logical XOR
 *
 * MY_XOR(X, Y) => if bool(X) != bool(Y) then 1 else 0 fi
 *
 * Crate: 05/26/2014 
 *      : 逻辑异或
 *
 * ****************************************************/

#include "./BOOL.h"
#
#define MY_XOR(x, y) MY_XOR_I(x, y)
#define MY_XOR_I(x, y) MY_BIT_XOR(MY_BOOL(x), MY_BOOL(y))
#
#define MY_BIT_XOR(x, y) MY_BIT_XOR_I(x, y)
#define MY_BIT_XOR_I(x, y) MY_BIT_XOR_ ## x ## y
#
#define MY_BIT_XOR_00 0
#define MY_BIT_XOR_01 1
#define MY_BIT_XOR_10 1
#define MY_BIT_XOR_11 0

#endif
