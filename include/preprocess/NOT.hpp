
#ifndef __FORK_STL_NOT_HPP__
#define __FORK_STL_NOT_HPP__
/* ****************************************************
 * Logical NOT
 *
 * MY_OR(X) => if X then 0 else 1 fi
 *
 * Crate: 05/26/2014 
 *      : 逻辑否
 *
 * ****************************************************/

#include "./COMPL.h"
#include "./BOOL.h"
#
#define MY_NOT(X) MY_NOT_I(X)
#define MY_NOT_I(X) MY_COMPL(MY_BOOL(X))
#
#endif

