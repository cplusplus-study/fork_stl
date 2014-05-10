// -*- C++ -*-

#ifndef __FORK_STL__INVOKE_BASE__
#define __FORK_STL__INVOKE_BASE__
#include <type_traits>
#include <utility>
// __get_result_type<F>
namespace xusd{
namespace base{

template<typename F>
struct __get_result_type;

template<typename R, typename... Args>
struct __get_result_type<R(Args...)>{ typedef R type; };
template<typename R, typename... Args>
struct __get_result_type<R(*)(Args...)>{ typedef R type; };
template<typename R, typename... Args>
struct __get_result_type<R(&)(Args...)>{ typedef R type; };

template<typename R,typename C, typename... Args>
struct __get_result_type<R(C::*)(Args...)>{ typedef R type; };
template<typename R,typename C, typename... Args>
struct __get_result_type<R(C::*)(Args...)const>{ typedef R type; };
template<typename R,typename C, typename... Args>
struct __get_result_type<R(C::*)(Args...)volatile>{ typedef R type; };
template<typename R,typename C, typename... Args>
struct __get_result_type<R(C::*)(Args...)const volatile>{ typedef R type; };


// __invoke(F, Args...)
template<typename F, typename... Args>
auto __invoke(F&& __f, Args&&... args)
    -> decltype(std::forward<F>(__f)(std::forward<Args>(args)...))
{
    return std::forward<F>(__f)(std::forward<Args>(args)...);
}

template<typename F, typename Arg0>
auto __invoke(F&& __f, Arg0&& obj_Or_objPtr)
    -> decltype(std::forward<Arg0>(obj_Or_objPtr)->*std::forward<F>(__f))
{
    return std::forward<Arg0>(obj_Or_objPtr)->*std::forward<F>(__f);
}
template<typename F, typename Arg0>
auto __invoke(F&& __f, Arg0&& obj_Or_objPtr)
    -> decltype(std::forward<Arg0>(obj_Or_objPtr).*std::forward<F>(__f))
{
    return std::forward<Arg0>(obj_Or_objPtr).*std::forward<F>(__f);
}

template<typename F, typename Arg0, typename... Args>
auto __invoke(F&& __f, Arg0&& obj_Or_objPtr, Args... args)
    -> decltype((std::forward<Arg0>(obj_Or_objPtr).*std::forward<F>(__f))(std::forward<Args>(args)...))
{
    return (std::forward<Arg0>(obj_Or_objPtr).*std::forward<F>(__f))(std::forward<Args>(args)...);
}

template<typename F, typename Arg0, typename... Args>
auto __invoke(F&& __f, Arg0&& obj_Or_objPtr, Args... args)
    -> decltype((std::forward<Arg0>(obj_Or_objPtr)->*(std::forward<F>(__f)))(std::forward<Args>(args)...))
{
    return (std::forward<Arg0>(obj_Or_objPtr)->*(std::forward<F>(__f)))(std::forward<Args>(args)...);
}

template<typename F, typename... Args>
struct __invoke_return_type{
    typedef decltype(__invoke(std::declval<F>(), std::declval<Args>()...)) type;
};


} // base
} // namespace

#endif

// vim: set mps+=<\:> ft=cpp tabstop=4 softtabstop=4 shiftwidth=4 expandtab cursorline colorcolumn=120:
