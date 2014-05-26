#ifndef __FORK_STL_COMPL_HPP__
#define __FORK_STL_COMPL_HPP__

/* ****************************************************
 * 1位二进制取反
 *
 *  MY_COMPL(1) == 0
 *  MY_COMPL(0) == 1
 *
 * Crate: 05/26/2014 
 *      : 1位二进制取反
 *
 * ****************************************************/

#
#define MY_COMPL(X) MY_COMPL_I(X)
#define MY_COMPL_I(X) MY_COMPL_ ## X
#
#define MY_COMPL_0 1
#define MY_COMPL_1 0
#
#endif
