#ifndef __FORK_STL_COMPL_HPP__
#define __FORK_STL_COMPL_HPP__

/* ****************************************************
 * 1位二进制取反
 *
 *  NULL_PP_COMPL(1) == 0
 *  NULL_PP_COMPL(0) == 1
 *
 * Crate: 05/26/2014 
 *      : 1位二进制取反
 *
 * ****************************************************/

#
#define NULL_PP_COMPL(X) MY_COMPL_I(X)
#define NULL_PP_COMPL_I(X) MY_COMPL_ ## X
#
#define NULL_PP_COMPL_0 1
#define NULL_PP_COMPL_1 0
#
#endif
