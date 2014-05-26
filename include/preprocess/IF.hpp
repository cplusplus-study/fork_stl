#ifndef __FORK_STL_IF_HPP__
#define __FORK_STL_IF_HPP__
#include "BOOL.h"
#
#
# /**/
#define NULL_PP_IF(cont, t, f) MY_IF_I(MY_BOOL(cont), t, f)
#define NULL_PP_IF_I(bit, t, f) MY_IF_II(bit, t, f)
#define NULL_PP_IF_II(bit, t, f) MY_IF_II_ ## bit(t, f)
#
#define NULL_PP_IF_II_0(t, f) f
#define NULL_PP_IF_II_1(t, f) t 
#
#
#
# /**/
#define NULL_PP_EXPR_IF(cont, expr) MY_EXPR_IF_I(MY_BOOL(cont), expr)
#define NULL_PP_EXPR_IF_I(bit, expr) MY_EXPR_IF_II(bit, expr)
#define NULL_PP_EXPR_IF_II(bit, expr) MY_EXPR_IF_II_ ## bit(expr)
#
#define NULL_PP_EXPR_IF_II_0(expr)
#define NULL_PP_EXPR_IF_II_1(expr) expr 
#
#endif
