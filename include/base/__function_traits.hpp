// -*- C++ -*-

#ifndef __FORK_STL__FUNCTION_TRAITS__
#define __FORK_STL__FUNCTION_TRAITS__
#include <type_traits>
#include <tuple>
namespace xusd{
namespace base{

template<typename T>
struct function_traits;

template<typename R, typename... Args>
struct function_traits<R(Args...)>{
    static constexpr size_t arity = sizeof...(Args);
    using return_type = R;
    using signature = R(Args...);
    template<size_t N>
    struct arg{
        static_assert(N<arity, "N is too large");
        using type = typename std::tuple_element<N, typename std::tuple<Args...>>::type;
    };
};
template<typename R, typename... Args>
constexpr size_t function_traits<R(Args...)>::arity;


template<typename C, typename R, typename... Args>
struct function_traits<R(C::*)(Args...)>:public function_traits<R(C&, Args...)>{};
template<typename C, typename R, typename... Args>
struct function_traits<R(C::*)(Args...) const>:public function_traits<R(C&, Args...)>{};
template<typename C, typename R, typename... Args>
struct function_traits<R(C::*)(Args...) volatile>:public function_traits<R(C&, Args...)>{};
template<typename C, typename R, typename... Args>
struct function_traits<R(C::*)(Args...) const volatile>:public function_traits<R(C&, Args...)>{};
template<typename C, typename R>
struct function_traits<R(C::*)>:public function_traits<R(C&)>{};


template <typename F>
struct function_traits<F&>:function_traits<F>{ };
template <typename F>
struct function_traits<F&&>:function_traits<F>{ };
template <typename F>
struct function_traits<F*>:function_traits<F>{ };

//functor, lambda etc.
template <typename F>
struct function_traits{
private:
    using _call_type = function_traits<decltype(&F::operator())>;
public:
    static constexpr size_t arity = _call_type::arity - 1;
    using return_type = typename _call_type::return_type;
    using signature = typename _call_type::signature;
    template<size_t N>
    struct arg{
        static_assert(N<arity, "N is too large");
        using type = typename _call_type::template arg<N+1>::type;
    };

};



} //namespace base
} //namespace xusd
#endif
